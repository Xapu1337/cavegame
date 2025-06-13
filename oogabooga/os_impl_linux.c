#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sched.h>
/*
 * Linux OS Implementation for Oogabooga Engine
 * 
 * This file implements platform-specific functionality for Linux.
 * It's included in the Unity build via oogabooga.c
 */

// Memory management
#define VIRTUAL_MEMORY_BASE ((void*)0x0000690000000000ULL)

// heap_alloc redefinition removed - conflicts with memory.c

// heap_dealloc redefinition removed - conflicts with memory.c

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  condition;
    bool            state;
} Linux_Binary_Semaphore;

// Threading implementation

void os_thread_init(Thread *t, Thread_Proc proc) {
    if (!t || !proc) return;

    memset(t, 0, sizeof(Thread));
    t->proc = proc;
    t->id = 0;
    t->initial_context = context;
    t->temporary_storage_size = KB(10);
}

void os_thread_destroy(Thread *t) {
    if (!t) return;
    os_thread_join(t);
    memset(t, 0, sizeof(Thread));
}

static void* thread_wrapper(void* arg) {
    Thread* t = (Thread*)arg;
    if (t && t->proc) {
        t->proc(t);
    }
    return NULL;
}

void os_thread_start(Thread *t) {
    if (!t) return;

    pthread_t thread;
    int result = pthread_create(&thread, NULL, thread_wrapper, t);
    if (result == 0) {
        t->os_handle = (Thread_Handle)thread;
        t->id = (u64)thread;
    }
}

void os_thread_join(Thread *t) {
    if (!t || !t->os_handle) return;

    pthread_join((pthread_t)t->os_handle, NULL);
    t->os_handle = NULL;
}

// Mutex implementation
Mutex_Handle OsMakeMutex(void) {
    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
    if (!mutex) {
        return NULL;
    }
    
    int result = pthread_mutex_init(mutex, NULL);
    if (result != 0) {
        free(mutex);
        return NULL;
    }
    
    return (Mutex_Handle)mutex;
}

void os_destroy_mutex(Mutex_Handle m) {
    if (!m) return;
    
    pthread_mutex_t* mutex = (pthread_mutex_t*)m;
    pthread_mutex_destroy(mutex);
    free(mutex);
}

void OsLockMutex(Mutex_Handle m) {
    if (!m) return;
    
    pthread_mutex_t* mutex = (pthread_mutex_t*)m;
    pthread_mutex_lock(mutex);
}

void OsUnlockMutex(Mutex_Handle m) {
    if (!m) return;
    
    pthread_mutex_t* mutex = (pthread_mutex_t*)m;
    pthread_mutex_unlock(mutex);
}

// Binary semaphore implementation

void os_binary_semaphore_init(Binary_Semaphore *sem, bool initial_state) {
    if (!sem) return;
    Linux_Binary_Semaphore *data = malloc(sizeof(*data));
    pthread_mutex_init(&data->mutex, NULL);
    pthread_cond_init(&data->condition, NULL);
    data->state = initial_state;
    sem->os_event = data;
}

void os_binary_semaphore_destroy(Binary_Semaphore *sem) {
    if (!sem) return;
    Linux_Binary_Semaphore *data = sem->os_event;
    if (data) {
        pthread_mutex_destroy(&data->mutex);
        pthread_cond_destroy(&data->condition);
        free(data);
    }
    sem->os_event = NULL;
}

void os_binary_semaphore_wait(Binary_Semaphore *sem) {
    if (!sem) return;
    Linux_Binary_Semaphore *data = sem->os_event;
    pthread_mutex_lock(&data->mutex);
    while (!data->state) {
        pthread_cond_wait(&data->condition, &data->mutex);
    }
    data->state = false;
    pthread_mutex_unlock(&data->mutex);
}

void os_binary_semaphore_signal(Binary_Semaphore *sem) {
    if (!sem) return;
    Linux_Binary_Semaphore *data = sem->os_event;
    pthread_mutex_lock(&data->mutex);
    data->state = true;
    pthread_cond_signal(&data->condition);
    pthread_mutex_unlock(&data->mutex);
}

// Sleep and timing
void os_sleep(uint32_t ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void os_yield_thread(void) {
    sched_yield();
}

void os_high_precision_sleep(double ms) {
    struct timespec ts;
    long long ns = (long long)(ms * 1000000.0);
    ts.tv_sec = ns / 1000000000;
    ts.tv_nsec = ns % 1000000000;
    nanosleep(&ts, NULL);
}

// File I/O
void os_write_string_to_stdout(string s) {
    if (s.data && s.count > 0) {
        write(STDOUT_FILENO, s.data, s.count);
    }
}

bool os_write_entire_file_s(string path, string data) {
    char* null_terminated_path = TempConvertToNullTerminatedString(path);
    if (!null_terminated_path) return false;
    
    int fd = open(null_terminated_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return false;
    }
    
    ssize_t bytes_written = write(fd, data.data, data.count);
    close(fd);
    
    return (bytes_written == (ssize_t)data.count);
}

bool os_read_entire_file_s(string path, string *result, Allocator allocator) {
    if (!result) return false;
    
    char* null_terminated_path = TempConvertToNullTerminatedString(path);
    if (!null_terminated_path) return false;
    
    int fd = open(null_terminated_path, O_RDONLY);
    if (fd == -1) {
        return false;
    }
    
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        close(fd);
        return false;
    }
    
    u64 file_size = file_stat.st_size;
    uint8_t* buffer = (uint8_t*)Alloc(allocator, file_size);
    if (!buffer) {
        close(fd);
        return false;
    }
    
    ssize_t bytes_read = read(fd, buffer, file_size);
    close(fd);
    
    if (bytes_read != (ssize_t)file_size) {
        Dealloc(allocator, buffer);
        return false;
    }
    
    result->data = buffer;
    result->count = file_size;
    return true;
}

// CPU and system info
u32 os_get_core_count(void) {
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    return (cores > 0) ? (u32)cores : 1;
}

// High-resolution timing
f64 os_get_current_time_in_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (f64)ts.tv_sec + (f64)ts.tv_nsec / 1000000000.0;
}

double OsGetElapsedSeconds(void) {
    static f64 start = 0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    f64 now = (f64)ts.tv_sec + (f64)ts.tv_nsec / 1000000000.0;
    if (start == 0) start = now;
    return now - start;
}

void OsUpdate(void) {
    // Simple sleep to avoid busy loop in example
    struct timespec ts = {0, 1000000};
    nanosleep(&ts, NULL);
}

void os_lock_program_memory_pages(void *start, u64 size) {
    (void)start;
    (void)size;
}

void os_unlock_program_memory_pages(void *start, u64 size) {
    (void)start;
    (void)size;
}

void* os_reserve_next_memory_pages(u64 size) {
    assert(size % os.page_size == 0, "size not page aligned");
    uint8_t *end = (uint8_t*)program_memory_next + size;
    uint8_t *tail = (uint8_t*)program_memory + program_memory_capacity;
    if (end > tail) {
        u64 minimum = (u64)(end - (uint8_t*)program_memory);
        u64 new_size = get_next_power_of_two(minimum);
        if (!os_grow_program_memory(new_size)) return NULL;
        tail = (uint8_t*)program_memory + program_memory_capacity;
    }
    void *p = program_memory_next;
    program_memory_next = end;
    return p;
}

File os_file_open_s(string path, Os_Io_Open_Flags flags) {
    char *p = TempConvertToNullTerminatedString(path);
    const char *mode = (flags & O_WRITE) ? ((flags & O_READ) ? "w+" : "w") : "r";
    return (File)fopen(p, mode);
}

bool os_file_write_string(File f, string s) {
    return fwrite(s.data, 1, s.count, (FILE*)f) == s.count;
}

void os_file_close(File f) {
    if (f) fclose((FILE*)f);
}

// Stack traces using glibc backtrace API
#include <execinfo.h>

string *os_get_stack_trace(u64 *trace_count, Allocator allocator) {
    const int max_frames = 64;
    void *frames[max_frames];
    int captured = backtrace(frames, max_frames);
    char **symbols = backtrace_symbols(frames, captured);
    string *result = (string *)Alloc(allocator, sizeof(string) * captured);
    for (int i = 0; i < captured; ++i) {
        size_t len = strlen(symbols[i]);
        char *buf = (char *)Alloc(allocator, len + 1);
        memcpy(buf, symbols[i], len + 1);
        result[i].data = (uint8_t *)buf;
        result[i].count = len;
    }
    free(symbols);
    *trace_count = captured;
    return result;
}

void* os_get_stack_base(void) {
    return NULL;
}

void* os_get_stack_limit(void) {
    return NULL;
}

bool os_grow_program_memory(u64 new_size) {
    if (program_memory_capacity >= new_size) return true;
    u64 aligned = align_next(new_size, os.page_size);
    size_t offset = (uint8_t*)program_memory_next - (uint8_t*)program_memory;
    void *res = mremap(program_memory, program_memory_capacity,
                       aligned, 0); /* try to grow in place */
    if (res == MAP_FAILED) {
        void *addr = (uint8_t*)program_memory + program_memory_capacity;
        size_t extra = aligned - program_memory_capacity;
        void *extra_map = mmap(addr, extra, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
                              -1, 0);
        if (extra_map == MAP_FAILED) return false;
    } else {
        program_memory = res;
    }
    program_memory_capacity = aligned;
    program_memory_next = (uint8_t*)program_memory + offset;
    return true;
}

void os_init(u64 program_memory_size) {
    os.page_size = (u64)sysconf(_SC_PAGESIZE);
    os.granularity = os.page_size;
    os.crt_vsnprintf = vsnprintf;
    context.thread_id = (u64)syscall(SYS_gettid);
    program_memory_capacity = align_next(program_memory_size, os.page_size);
    program_memory = mmap(NULL, program_memory_capacity,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (program_memory == MAP_FAILED) {
        fprintf(stderr, "Failed to allocate program memory\n");
        exit(1);
    }
    program_memory_next = program_memory;
}

// Process info
u32 os_get_process_id(void) {
    return (u32)getpid();
}

u32 os_get_thread_id(void) {
    return (u32)syscall(SYS_gettid);
}

// Debug and error handling
void os_debug_break(void) {
    raise(SIGTRAP);
}

void os_exit_with_error_code(s32 error_code) {
    exit(error_code);
}
