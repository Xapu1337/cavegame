#ifndef OOGABOOGA_OS_INTERFACE_H
#define OOGABOOGA_OS_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

// Basic types
#include "utility.h"

// Forward declare vector types (defined in linmath.h)
typedef union Vector2f32 Vector2;
typedef union Vector3f32 Vector3;
typedef union Vector4f32 Vector4;

// String type

// Basic allocator types

typedef void*(*Allocator_Proc)(uint64_t, void*, Allocator_Message, void*);


// Platform-specific types
#ifdef _WIN32
    typedef HWND Window_Handle;
#elif defined(__linux__)
    typedef void* Window_Handle; // Will be a pointer to X11 Window or Wayland surface
#else
    typedef void* Window_Handle;
#endif

// Deprecation macro
#if defined(_MSC_VER)
    #define DEPRECATED(proc, msg) __declspec(deprecated(msg)) proc
#elif defined(__GNUC__)
    #define DEPRECATED(proc, msg) __attribute__((deprecated(msg))) proc 
#else
    #define DEPRECATED(proc, msg) proc
#endif

// Monitor structure
typedef struct Os_Monitor {
    uint64_t refresh_rate;
    uint64_t resolution_x;
    uint64_t resolution_y;
    uint64_t work_area_x;
    uint64_t work_area_y;
    uint64_t work_area_width;
    uint64_t work_area_height;
    uint64_t dpi;
    uint64_t dpi_y;
    string name;
} Os_Monitor;

// Window structure  
typedef struct Os_Window {
    // Keep in mind that setting these in runtime is potentially slow since they might trigger win32 calls!
    string title;
    union { int32_t width;  int32_t pixel_width;  };
    union { int32_t height; int32_t pixel_height; };
    DEPRECATED(int32_t scaled_width, "Use point_width instead, which is 72th of an inch");
    DEPRECATED(int32_t scaled_height, "Use point_height instead, which is 72th of an inch");
    int32_t point_width; // 72th of an inch
    int32_t point_height; // 72th of an inch
    union { int32_t x; int32_t pixel_x; };
    union { int32_t y; int32_t pixel_y; };
    int32_t point_x; // 72th of an inch
    int32_t point_y; // 72th of an inch
    Vector4 clearColor;
    bool enable_vsync;
    bool fullscreen;
    bool allow_resize;
    bool force_topmost;
    u32 dpi;
    double point_size_in_pixels;
    
    bool should_close;
    
    // readonly
    bool _initialized;
    Window_Handle _os_handle;
    bool is_being_dragged;
    Os_Monitor *monitor;
} Os_Window;

// Forward declarations
typedef struct Draw_Frame Draw_Frame;
typedef struct Input_Frame Input_Frame;

// Timing functions
double OsGetElapsedSeconds(void);

// Window and OS update functions
void OsUpdate(void);

typedef struct Thread Thread;
typedef void (*Thread_Proc)(Thread*);
void OsThreadInit(Thread *t, Thread_Proc proc);
void OsThreadDestroy(Thread *t);
void OsThreadStart(Thread *t);
void OsThreadJoin(Thread *t);

typedef void* Mutex_Handle;
Mutex_Handle OsMakeMutex(void);
void OsDestroyMutex(Mutex_Handle m);
void OsLockMutex(Mutex_Handle m);
void OsUnlockMutex(Mutex_Handle m);

typedef struct Binary_Semaphore Binary_Semaphore;
void OsBinarySemaphoreInit(Binary_Semaphore *sem, bool initial_state);
void OsBinarySemaphoreDestroy(Binary_Semaphore *sem);
void OsBinarySemaphoreWait(Binary_Semaphore *sem);
void OsBinarySemaphoreSignal(Binary_Semaphore *sem);

void OsSleep(u32 ms);
void OsYieldThread(void);
void OsHighPrecisionSleep(double ms);

typedef void* File;
void OsWriteStringToStdout(string s);
bool OsWriteEntireFileS(string path, string data);
bool OsReadEntireFileS(string path, string *result, Allocator allocator);

// Debug utilities
string *OsGetStackTrace(u64 *traceCount, Allocator allocator);
void DumpStackTrace(void);

#endif
