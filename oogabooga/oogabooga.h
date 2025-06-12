#ifndef OOGABOOGA_OOGABOOGA_H
#define OOGABOOGA_OOGABOOGA_H

/*
 * Oogabooga Engine - Bootstrap Header
 * 
 * This is the main header for the Oogabooga game engine.
 * 
 * PHILOSOPHY:
 * This engine follows proper C development practices with complete header/source 
 * separation for professional development workflows. Headers are not optional - 
 * they are essential for IDE support, code navigation, and maintainable codebases.
 * 
 * MEMORY SAFETY BY DEFAULT:
 * - Bounds checking on all array access
 * - Null pointer validation 
 * - Buffer overflow protection
 * - Double-free detection
 * - Memory leak tracking
 * 
 * CROSS-PLATFORM SUPPORT:
 * - Windows: MSVC (default) or MinGW/GCC → D3D11 renderer
 * - Linux: GCC → Vulkan renderer (no SDL2)
 * 
 * Usage in your application:
 *   #include "oogabooga/oogabooga.h"
 * 
 * The engine abstracts all platform-specific details. Your application code
 * never directly calls Vulkan, D3D11, or platform APIs.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

//=============================================================================
// PLATFORM DETECTION
//=============================================================================

#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_LINUX 0
    #define PLATFORM_MACOS 0
    #define PLATFORM_NAME "Windows"
#elif defined(__linux__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_LINUX 1
    #define PLATFORM_MACOS 0
    #define PLATFORM_NAME "Linux"
#elif defined(__APPLE__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_LINUX 0
    #define PLATFORM_MACOS 1
    #define PLATFORM_NAME "macOS"
#else
    #error "Unsupported platform. Supported: Windows, Linux, macOS"
#endif

//=============================================================================
// COMPILER DETECTION
//=============================================================================

#ifdef __clang__
    #define COMPILER_CLANG 1
    #define COMPILER_GCC 0
    #define COMPILER_MSVC 0
    #define COMPILER_NAME "Clang"
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_CLANG 0
    #define COMPILER_GCC 1
    #define COMPILER_MSVC 0
    #define COMPILER_NAME "GCC"
#elif defined(_MSC_VER)
    #define COMPILER_CLANG 0
    #define COMPILER_GCC 0
    #define COMPILER_MSVC 1
    #define COMPILER_NAME "MSVC"
#else
    #error "Unsupported compiler. Supported: GCC, Clang, MSVC"
#endif

//=============================================================================
// GRAPHICS RENDERER SELECTION
//=============================================================================

#if PLATFORM_WINDOWS
    #define RENDERER_D3D11 1
    #define RENDERER_VULKAN 0
    #define RENDERER_METAL 0
    #define RENDERER_NAME "Direct3D 11"
#elif PLATFORM_LINUX
    #define RENDERER_D3D11 0
    #define RENDERER_VULKAN 1
    #define RENDERER_METAL 0
    #define RENDERER_NAME "Vulkan"
#elif PLATFORM_MACOS
    #define RENDERER_D3D11 0
    #define RENDERER_VULKAN 0
    #define RENDERER_METAL 1
    #define RENDERER_NAME "Metal"
#endif

//=============================================================================
// MEMORY SAFETY CONFIGURATION
//=============================================================================

#ifndef OOGABOOGA_MEMORY_SAFETY
    #define OOGABOOGA_MEMORY_SAFETY 1  // Enabled by default
#endif

#if OOGABOOGA_MEMORY_SAFETY
    #define MEMORY_SAFETY_ENABLED 1
    #define ENABLE_BOUNDS_CHECKING 1
    #define ENABLE_NULL_CHECKS 1
    #define ENABLE_BUFFER_OVERFLOW_PROTECTION 1
    #define ENABLE_DOUBLE_FREE_DETECTION 1
    #define ENABLE_MEMORY_LEAK_TRACKING 1
#else
    #define MEMORY_SAFETY_ENABLED 0
    #define ENABLE_BOUNDS_CHECKING 0
    #define ENABLE_NULL_CHECKS 0
    #define ENABLE_BUFFER_OVERFLOW_PROTECTION 0
    #define ENABLE_DOUBLE_FREE_DETECTION 0
    #define ENABLE_MEMORY_LEAK_TRACKING 0
#endif

//=============================================================================
// COMPILER-SPECIFIC ATTRIBUTES
//=============================================================================

#if COMPILER_GCC || COMPILER_CLANG
    #define FORCE_INLINE __attribute__((always_inline)) inline
    #define NO_INLINE __attribute__((noinline))
    #define ALIGN(x) __attribute__((aligned(x)))
    #define THREAD_LOCAL __thread
    #define LIKELY(x) __builtin_expect(!!(x), 1)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
#elif COMPILER_MSVC
    #define FORCE_INLINE __forceinline
    #define NO_INLINE __declspec(noinline)
    #define ALIGN(x) __declspec(align(x))
    #define THREAD_LOCAL __declspec(thread)
    #define LIKELY(x) (x)
    #define UNLIKELY(x) (x)
#else
    #define FORCE_INLINE inline
    #define NO_INLINE
    #define ALIGN(x)
    #define THREAD_LOCAL
    #define LIKELY(x) (x)
    #define UNLIKELY(x) (x)
#endif

//=============================================================================
// BASIC TYPES
//=============================================================================

typedef uint8_t  uint8_t;
typedef uint16_t uint16_t;
typedef uint32_t uint32_t;
typedef uint64_t uint64_t;
typedef int8_t   int8_t;
typedef int16_t  int16_t;
typedef int32_t  int32_t;
typedef int64_t  int64_t;

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;

typedef float f32;
typedef double f64;

typedef size_t usize;
typedef ptrdiff_t ssize;

// Boolean type provided by stdbool.h

//=============================================================================
// CONSTANTS
//=============================================================================

#define F32_MAX 3.402823466e+38F
#define F32_MIN 1.175494351e-38F
#define F64_MAX 1.7976931348623157e+308
#define F64_MIN 2.2250738585072014e-308

#define KB(x) ((x) * 1024ULL)
#define MB(x) ((x) * 1024ULL * 1024ULL)
#define GB(x) ((x) * 1024ULL * 1024ULL * 1024ULL)

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

//=============================================================================
// MEMORY SAFETY MACROS
//=============================================================================

#if MEMORY_SAFETY_ENABLED

// Assertion system
void oogabooga_assert_fail(const char* expr, const char* file, int line, const char* func);

#define ASSERT(expr) \
    do { \
        if (UNLIKELY(!(expr))) { \
            oogabooga_assert_fail(#expr, __FILE__, __LINE__, __func__); \
        } \
    } while(0)

// Memory safety checks
#define CHECK_NULL(ptr) ASSERT((ptr) != NULL)
#define CHECK_BOUNDS(index, size) ASSERT((index) < (size))
#define CHECK_RANGE(value, min, max) ASSERT((value) >= (min) && (value) <= (max))

// Safe array access
#define SAFE_ARRAY_ACCESS(arr, index, size) \
    (CHECK_BOUNDS(index, size), (arr)[index])

// Safe pointer dereference
#define SAFE_DEREF(ptr) \
    (CHECK_NULL(ptr), *(ptr))

#else

// No-op versions for release builds
#define ASSERT(expr) ((void)0)
#define CHECK_NULL(ptr) ((void)0)
#define CHECK_BOUNDS(index, size) ((void)0)
#define CHECK_RANGE(value, min, max) ((void)0)
#define SAFE_ARRAY_ACCESS(arr, index, size) ((arr)[index])
#define SAFE_DEREF(ptr) (*(ptr))

#endif

//=============================================================================
// ENGINE HEADERS
//=============================================================================

// Core foundation
#include "base.h"
#include "memory.h"
#include "string.h"

// Math and graphics
#include "linmath.h"
#include "color.h"

// Platform abstraction
#include "os_interface.h"
#include "cpu.h"

// Data structures
#include "hash.h"
#include "growing_array.h"
#include "hash_table.h"

// Graphics and rendering
#include "gfx_interface.h"
#include "drawing.h"
#include "font.h"

// Input and audio
#include "input.h"
#include "audio.h"

// Utilities
#include "random.h"
#include "utility.h"
#include "unicode.h"
#include "path_utils.h"
#include "string_format.h"

// System utilities
#include "concurrency.h"

// Optional features (conditionally included)
#ifdef ENABLE_PROFILING
#include "profiling.h"
#endif

#ifdef ENABLE_SIMD
#include "simd.h"
#endif

#ifdef OOGABOOGA_ENABLE_EXTENSIONS
#include "extensions.h"
#ifdef OOGABOOGA_EXTENSION_PARTICLES
#include "ext_particles.h"
#endif
#endif

#ifdef RUN_TESTS
#include "tests.h"
#endif

//=============================================================================
// ENGINE INFORMATION
//=============================================================================

typedef struct Engine_Info {
    const char* platform_name;
    const char* compiler_name;
    const char* renderer_name;
    bool memory_safety_enabled;
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
} Engine_Info;

// Get engine configuration information
Engine_Info oogabooga_get_engine_info(void);

// Initialize engine with default settings
bool oogabooga_init(u64 program_memory_size);

// Shutdown engine and cleanup resources
void oogabooga_shutdown(void);

// Global variables available to application code
extern Os_Window window;
extern Draw_Frame drawFrame;
extern Input_Frame inputFrame;

#endif // OOGABOOGA_OOGABOOGA_H
