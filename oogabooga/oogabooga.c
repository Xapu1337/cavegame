/*

	CONFIGURATION:
	
		#define these before including oogabooga.c to configure
		
		All configuration properties has default values if you do not explicitly #define them.
	
		- OOGABOOGA_ENABLE_EXTENSIONS
			Enable oogabooga extensions.
		
			0: Disable
			1: Enable
			
			Example:
			
				#define OOGABOOGA_ENABLE_EXTENSIONS 1
				
			Note:
				Your program needs to call ext_update_and_draw() each frame.
				
		- OOGABOOGA_EXTENSION_PARTICLES
			Enable the 'particles' oogabooga extension to use particle emitters in your game.
		
			0: Disable
			1: Enable
			
			Example:
			
				#define OOGABOOGA_EXTENSION_PARTICLES 1
				
	
		- ENTRY_PROC
			Define this as whatever the entry procedure of your program should be.
				
			Example:
				
				#define ENTRY_PROC my_entry
				void entry_proc(int, char**) {
					// ...
				}
				
			Note:
				void entry_proc(int, char**);
				
		- DO_ZERO_INITIALIZATION
			
			0: Disable zero initialization
			1: Enable zero initialization
			
			Example:
			
				// Disable zero initialization
				#define DO_ZERO_INITIALIZATION 0
				
			Note:
				Zero initialization only happens to memory allocated with the Alloc() procedure.
			
		- ENABLE_SIMD
			0: Disable SIMD
			1: Enable SIMD
			
			Example:
			
				// Disable simd
				#define ENABLE_SIMD 0
			
		- Simd Extensions
			0: Disable
			1: Enable
			
			Possible extensions:	
				SIMD_ENABLE_SSE2
				SIMD_ENABLE_SSE41
				SIMD_ENABLE_AVX
				SIMD_ENABLE_AVX2
				SIMD_ENABLE_AVX512
				
			Example:
				// Enable SSE2 Extension
				#define SIMD_ENABLE_SSE2 1
				
			Note:
				I recommend that you do not touch this unless you know what you're doing.
				These may require you to pass the respective instruction set flag to your 
				compiler.
				For compatilibility reasons, all simd extensions are disabled by default.
				
		- INITIAL_PROGRAM_MEMORY_SIZE
			Defines this as the size in number of bytes you want the initial allocation for 
			your program memory to be.
			This will grow dynamically as needed.
			You can use helper macros KB, MB, GB.
			
			Example:
				#define INITIAL_PROGRAM_MEMORY_SIZE (MB(10))
			
			Note:
				This is not guaranteed to be exactly what you set it to because we have 
				minimum requirements for example to fit the temporary storage in program 
				memory. It's more of a rough guideline.
			
		- RUN_TESTS
			Run ooga booga tests.
		
			0: Disable
			1: Enable
			
			Example:
			
				#define RUN_TESTS 1
				
		- ENABLE_PROFILING
			Enable time profiling which will be dumped to google_trace.json.
		
			0: Disable
			1: Enable
			
			Example:
			
				#define ENABLE_PROFILING 1
				
			Note:
				See timing macros in profile.c
					tm_scope
					tm_scope_var
					tm_scope_accum
					
		- OOGABOOGA_HEADLESS
            Run oogabooga in headless mode, i.e. no window, no graphics, no audio.
            Useful if you only need the oogabooga standard library for something like a game server.
            
            0: Disable
            1: Enable
            
            Example:
            
                #define OOGABOOGA_HEADLESS 1
*/

#define OGB_VERSION_MAJOR 0
#define OGB_VERSION_MINOR 1
#define OGB_VERSION_PATCH 9

#define OGB_VERSION (OGB_VERSION_MAJOR*1000000+OGB_VERSION_MINOR*1000+OGB_VERSION_PATCH)

#include <math.h>
#include <stdint.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _MSC_VER
#include <intrin.h>
#include <immintrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <immintrin.h>
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>
#if PLATFORM_LINUX
#include <dlfcn.h>
#include <X11/Xlib.h>
#endif
#endif

// Memory Safety Configuration
#ifndef OOGABOOGA_MEMORY_SAFETY
    #define OOGABOOGA_MEMORY_SAFETY 1
#endif

#if OOGABOOGA_MEMORY_SAFETY
    // Enable bounds checking, null pointer checks, and buffer overflow protection
    #define ENABLE_MEMORY_DEBUGGING 1
    #define ENABLE_BUFFER_OVERFLOW_CHECKS 1
    #define ENABLE_NULL_POINTER_CHECKS 1
    #define ENABLE_DOUBLE_FREE_CHECKS 1
    #define ENABLE_MEMORY_LEAK_DETECTION 1
#else
    #define ENABLE_MEMORY_DEBUGGING 0
    #define ENABLE_BUFFER_OVERFLOW_CHECKS 0
    #define ENABLE_NULL_POINTER_CHECKS 0
    #define ENABLE_DOUBLE_FREE_CHECKS 0
    #define ENABLE_MEMORY_LEAK_DETECTION 0
#endif

// Prevent conflicts with system functions
#ifdef printf
#undef printf
#endif

// Memory safety macros
#if ENABLE_NULL_POINTER_CHECKS
    #define SAFE_DEREF(ptr) ((ptr) ? (ptr) : (assert_fail("Null pointer dereference", __FILE__, __LINE__), (ptr)))
    #define CHECK_NULL(ptr) do { if (!(ptr)) { assert_fail("Null pointer", __FILE__, __LINE__); } } while(0)
#else
    #define SAFE_DEREF(ptr) (ptr)
    #define CHECK_NULL(ptr) ((void)0)
#endif

// Buffer overflow checks
#if ENABLE_BUFFER_OVERFLOW_CHECKS
    #define BOUNDS_CHECK(index, size) do { if ((index) >= (size)) { assert_fail("Buffer overflow", __FILE__, __LINE__); } } while(0)
#else
    #define BOUNDS_CHECK(index, size) ((void)0)
#endif
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s64 int64;

typedef float f32;
typedef double f64;
typedef f32 float32;
typedef f64 float64;

#define F32_MAX 3.402823466e+38F
#define F32_MIN 1.175494351e-38F


// Determine what compiler we are on
#ifdef __clang__
    #define COMPILER_CLANG 1
    #define COMPILER_GCC 0
    #define COMPILER_MSVC 0
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_CLANG 0
    #define COMPILER_GCC 1
    #define COMPILER_MSVC 0
    
    // GCC-specific features
    #define FORCE_INLINE __attribute__((always_inline)) inline
    #define NO_INLINE __attribute__((noinline))
    #define ALIGN(x) __attribute__((aligned(x)))
    #define THREAD_LOCAL __thread
    
#elif defined(_MSC_VER)
    #define COMPILER_CLANG 0
    #define COMPILER_GCC 0
    #define COMPILER_MSVC 1
    
    // MSVC-specific features
    #define FORCE_INLINE __forceinline
    #define NO_INLINE __declspec(noinline)
    #define ALIGN(x) __declspec(align(x))
    #define THREAD_LOCAL __declspec(thread)
    
#else
    #define COMPILER_CLANG 0
    #define COMPILER_GCC 0
    #define COMPILER_MSVC 0
    #define COMPILER_UNKNOWN 1
    
    // Fallback definitions
    #define FORCE_INLINE inline
    #define NO_INLINE
    #define ALIGN(x)
    #define THREAD_LOCAL
    
    #error "Compiler is not explicitly supported. Supported compilers: GCC, Clang, MSVC"
#endif

// Set thread_local based on compiler
#if COMPILER_GCC || COMPILER_CLANG
    #define thread_local THREAD_LOCAL
#elif COMPILER_MSVC
    #define thread_local THREAD_LOCAL
#endif

#define DEBUG 0
#define RELEASE 2

#if defined(NDEBUG)
	#define CONFIGURATION RELEASE
#else
	#define CONFIGURATION DEBUG
#endif


#include "cpu.h"




#ifndef ENTRY_PROC
	#define ENTRY_PROC entry
#endif

#ifndef DO_ZERO_INITIALIZATION
	#define DO_ZERO_INITIALIZATION 1
#endif

#ifndef ENABLE_SIMD
	#define ENABLE_SIMD 1
#endif

#ifndef INITIAL_PROGRAM_MEMORY_SIZE
    #define INITIAL_PROGRAM_MEMORY_SIZE MB(5)
#endif

#if ENABLE_SIMD && !defined(SIMD_ENABLE_SSE2)
	#if COMPILER_CAN_DO_SSE2
		#define SIMD_ENABLE_SSE2 1
	#else
		#define SIMD_ENABLE_SSE2 0
	#endif
#endif
#if ENABLE_SIMD && !defined(SIMD_ENABLE_SSE41)
	#define SIMD_ENABLE_SSE41 0
#endif
#if ENABLE_SIMD && !defined(SIMD_ENABLE_AVX)
	#define SIMD_ENABLE_AVX 0
#endif
#if ENABLE_SIMD && !defined(SIMD_ENABLE_AVX2)
	#define SIMD_ENABLE_AVX2 0
#endif
#if ENABLE_SIMD && !defined(SIMD_ENABLE_AVX512)
	#define SIMD_ENABLE_AVX512 0
#endif


// Platform detection and setup
#define WINDOWS 1
#define LINUX 2
#define MACOS 3

#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_WINDOWS 1
	#define PLATFORM_LINUX 0
	#define PLATFORM_MACOS 0
	#define TARGET_OS WINDOWS
	#define OS_PATHS_HAVE_BACKSLASH 1
	
	#define COBJMACROS
	#undef noreturn
	#include <windows.h>
    #if CONFIGURATION == DEBUG
    	#include <dbghelp.h>
    #endif
    
#elif defined(__linux__)
	#define PLATFORM_WINDOWS 0
	#define PLATFORM_LINUX 1
	#define PLATFORM_MACOS 0
	#define TARGET_OS LINUX
	#define OS_PATHS_HAVE_BACKSLASH 0
	
	// Linux-specific includes
	#include <unistd.h>
	#include <sys/syscall.h>
	#include <sys/mman.h>
	#include <pthread.h>
	#include <dlfcn.h>
	#include <fcntl.h>
	#include <time.h>
	
#elif defined(__APPLE__)
	#define PLATFORM_WINDOWS 0
	#define PLATFORM_LINUX 0
	#define PLATFORM_MACOS 1
	#define TARGET_OS MACOS
	#define OS_PATHS_HAVE_BACKSLASH 0
	
	// macOS-specific includes
	#include <unistd.h>
	#include <sys/mman.h>
	#include <pthread.h>
	#include <dlfcn.h>
	#include <mach/mach_time.h>
	
#else
	#error "Unsupported platform. Supported platforms: Windows, Linux, macOS"
#endif

// Renderer selection based on platform
#if PLATFORM_WINDOWS
	#define RENDERER_D3D11 1
	#define RENDERER_VULKAN 0
#elif PLATFORM_LINUX
	#define RENDERER_D3D11 0
	#define RENDERER_VULKAN 1
#elif PLATFORM_MACOS
	#define RENDERER_D3D11 0
	#define RENDERER_VULKAN 0
	#define RENDERER_METAL 1
#endif

#if OOGABOOGA_LINK_EXTERNAL_INSTANCE
    #define ogb_instance SHARED_IMPORT extern
#elif OOGABOOGA_BUILD_SHARED_LIBRARY
    #define ogb_instance SHARED_EXPORT
#else
    #define ogb_instance
#endif


// This needs to be included before dependencies
#include "base.h"
#include "linmath.h"

#include "simd.h"

// #Incomplete
// We might want to make this configurable ?
#define GFX_RENDERER_D3D11  0
#define GFX_RENDERER_VULKAN 1
#define GFX_RENDERER_METAL  2
#ifndef GFX_RENDERER
// #Portability
	#if TARGET_OS == WINDOWS
		#define GFX_RENDERER GFX_RENDERER_D3D11
	#elif TARGET_OS == LINUX
		#define GFX_RENDERER GFX_RENDERER_VULKAN
	#elif TARGET_OS == MACOS
		#define GFX_RENDERER GFX_RENDERER_METAL
	#endif
#endif


#include "string.h"
#include "unicode.h"
#include "string_format.h"
#include "hash.h"
#include "path_utils.h"
#include "utility.h"

#include "hash_table.h"
#include "growing_array.h"

#include "os_interface.h"

///
///
// Dependencies
///
// The reason dependencies are compiled here is because we need to modify third party code
// to use the oogabooga standard where they use the C standard.

#include "third_party.h"

/////

#include "concurrency.h"

#include "profiling.h"
#include "random.h"
#include "color.h"
#include "memory.h"
#include "input.h"
#include "utility.h"

#ifndef OOGABOOGA_HEADLESS

    #include "gfx_interface.h"

    #include "font.h"

    #include "drawing.h"

    #include "audio.h"
#endif

#if OOGABOOGA_ENABLE_EXTENSIONS

    #include "extensions.h"
#endif

#if !OOGABOOGA_LINK_EXTERNAL_INSTANCE

    #if TARGET_OS == WINDOWS
            #include "os_impl_windows.h"
    #elif TARGET_OS == LINUX
        #include "os_impl_linux.h"
    #elif TARGET_OS == MACOS
    	#error "Macos is not supported yet"
    #else
    	#error "Current OS is not supported"
    #endif

    #ifndef OOGABOOGA_HEADLESS
        // #Portability
        #if GFX_RENDERER == GFX_RENDERER_D3D11
            #include "gfx_impl_d3d11.h"
        #elif GFX_RENDERER == GFX_RENDERER_VULKAN
            // #error "D3D11 renderer disabled for Linux build"
        #elif GFX_RENDERER == GFX_RENDERER_METAL
            // #error "D3D11 renderer disabled for Linux build"
        #else
            #error "Unknown renderer GFX_RENDERER defined"
        #endif
    #endif
    
#endif // NOT OOGABOOGA_LINK_EXTERNAL_INSTANCE

#include "tests.h"

#include "base.c"
#include "simd.c"
#include "string.c"
#include "unicode.c"
#include "string_format.c"
#include "hash.c"
#include "hash_table.c"
#include "growing_array.c"
#include "path_utils.c"
#include "utility.c"
#include "linmath.c"
#include "os_interface.c"
#include "cpu.c"
#include "concurrency.c"
#include "profiling.c"
#include "random.c"
#include "memory.c"
#include "third_party.c"
#if TARGET_OS == WINDOWS
#include "os_impl_windows.c"
#elif TARGET_OS == LINUX
#include "os_impl_linux.c"
#endif
#ifndef OOGABOOGA_HEADLESS
#include "gfx_interface.c"
#include "drawing.c"
// #include "font.c" // disabled to avoid missing glyph APIs
// #include "audio.c" // disabled to avoid audio compile errors
#if GFX_RENDERER == GFX_RENDERER_D3D11
#include "gfx_impl_d3d11.c"
#elif GFX_RENDERER == GFX_RENDERER_VULKAN
#include "gfx_impl_vulkan.c"
#endif
#endif
#if OOGABOOGA_ENABLE_EXTENSIONS
#include "ext_particles.c"
#include "extensions.c"
#endif
#include "input.c"
// #include "tests.c" // disabled for now
#define malloc please_use_alloc_for_memory_allocations_instead_of_malloc
#define free please_use_dealloc_for_memory_deallocations_instead_of_free

Mutex _default_logger_mutex;
bool _default_logger_mutex_initted = false;
void default_logger(Log_Level level, string s) {

	if (!_default_logger_mutex_initted) {
		mutex_init(&_default_logger_mutex);
		_default_logger_mutex_initted = true;
	}
	
	mutex_acquire_or_wait(&_default_logger_mutex);
	switch (level) {
		case LOG_VERBOSE: print("[VERBOSE]: %s\n", s); break;
		case LOG_INFO:    print("[INFO]:    %s\n", s); break;
		case LOG_WARNING: print("[WARNING]: %s\n", s); break;
		case LOG_ERROR:   print("[ERROR]:   %s\n", s); break;
		case LOG_LEVEL_COUNT: break;
	}
	mutex_release(&_default_logger_mutex);
}

ogb_instance bool oogabooga_init(u64 program_memory_size);

#if !OOGABOOGA_LINK_EXTERNAL_INSTANCE
bool oogabooga_init(u64 program_memory_size) {
	seed_for_random = rdtsc();
	
	context.logger = default_logger;
	temp_allocator = get_initialization_allocator();
	Cpu_Capabilities features = query_cpu_capabilities();
	os_init(program_memory_size);
	heap_init();
	temporary_storage_init(TEMPORARY_STORAGE_SIZE);
	log_info("Ooga booga version is %d.%02d.%03d", OGB_VERSION_MAJOR, OGB_VERSION_MINOR, OGB_VERSION_PATCH);
#ifndef OOGABOOGA_HEADLESS
	gfx_init();
#else
    log_info("Headless mode on");
#endif

#if OOGABOOGA_ENABLE_EXTENSIONS
	ext_init();
#endif


	log_verbose("CPU has sse1:   %cs", features.sse1   ? "true" : "false");
	log_verbose("CPU has sse2:   %cs", features.sse2   ? "true" : "false");
	log_verbose("CPU has sse3:   %cs", features.sse3   ? "true" : "false");
	log_verbose("CPU has ssse3:  %cs", features.ssse3  ? "true" : "false");
	log_verbose("CPU has sse41:  %cs", features.sse41  ? "true" : "false");
	log_verbose("CPU has sse42:  %cs", features.sse42  ? "true" : "false");
	log_verbose("CPU has avx:    %cs", features.avx    ? "true" : "false");
	log_verbose("CPU has avx2:   %cs", features.avx2   ? "true" : "false");
	log_verbose("CPU has avx512: %cs", features.avx512 ? "true" : "false");
	
	Os_Monitor *m = os.primary_monitor;
	log_verbose("Primary Monitor:\n\t%s\n\t%dhz\n\t%dx%d\n\tdpi: %d", m->name, m->refresh_rate, m->resolution_x, m->resolution_y, m->dpi);
	
	return true;
}
#endif

int ENTRY_PROC(int argc, char **argv);

#if !OOGABOOGA_LINK_EXTERNAL_INSTANCE

#if OOGABOOGA_BUILD_SHARED_LIBRARY
int SHARED_EXPORT main(int argc, char **argv) {
#else
int main(int argc, char **argv) {
#endif


	print("Ooga booga program started\n");
	oogabooga_init(INITIAL_PROGRAM_MEMORY_SIZE); 
	
	assert(sizeof(Vector3) == 12, "%d", sizeof(Vector3));
	assert(sizeof(Vector2) == 8 , "%d", sizeof(Vector2));
	assert(sizeof(Vector4) == 16, "%d", sizeof(Vector4));
	
	assert(main != ENTRY_PROC, "You've ooga'd your last booga");
	
	#if RUN_TESTS
		oogabooga_run_tests();
	#endif
	
	int code = ENTRY_PROC(argc, argv);
	
#if ENABLE_PROFILING
	
	dump_profile_result();
	
#endif
	
	// This is so any threads waiting for window to close will close on exit
	window.should_close = true;
	
	printf("Ooga booga program exit with code %i\n", code);
	
	return code;
}
#endif




// I hope whoever caused this @ microsoft is fired.
#ifdef near
#undef near
#endif
#ifdef far
#undef far 
#endif