#include <time.h>
#include <sys/syscall.h>
#include <unistd.h>
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

// Threading implementation

void os_thread_init(Thread *t, Thread_Proc proc) {
    if (!t || !proc) return;
    
    memset(t, 0, sizeof(Thread));
    t->proc = proc;
    t->started = false;
    t->joinable = false;
}

void os_thread_destroy(Thread *t) {
    if (!t) return;
    
    if (t->joinable) {
        os_thread_join(t);
    }
    
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
    if (!t || t->started) return;
    
    int result = pthread_create(&t->handle, NULL, thread_wrapper, t);
    if (result == 0) {
        t->started = true;
        t->joinable = true;
    }
}

void os_thread_join(Thread *t) {
    if (!t || !t->joinable) return;
    
    pthread_join(t->handle, NULL);
    t->joinable = false;
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
    
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->condition, NULL);
    sem->state = initial_state;
}

void os_binary_semaphore_destroy(Binary_Semaphore *sem) {
    if (!sem) return;
    
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->condition);
    memset(sem, 0, sizeof(Binary_Semaphore));
}

void os_binary_semaphore_wait(Binary_Semaphore *sem) {
    if (!sem) return;
    
    pthread_mutex_lock(&sem->mutex);
    while (!sem->state) {
        pthread_cond_wait(&sem->condition, &sem->mutex);
    }
    sem->state = false;
    pthread_mutex_unlock(&sem->mutex);
}

void os_binary_semaphore_signal(Binary_Semaphore *sem) {
    if (!sem) return;
    
    pthread_mutex_lock(&sem->mutex);
    sem->state = true;
    pthread_cond_signal(&sem->condition);
    pthread_mutex_unlock(&sem->mutex);
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
