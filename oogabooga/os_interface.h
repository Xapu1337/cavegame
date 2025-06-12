#ifndef OOGABOOGA_OS_INTERFACE_H
#define OOGABOOGA_OS_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Thread Thread;
typedef void (*Thread_Proc)(Thread*);
void os_thread_init(Thread *t, Thread_Proc proc);
void os_thread_destroy(Thread *t);
void os_thread_start(Thread *t);
void os_thread_join(Thread *t);

typedef void* Mutex_Handle;
Mutex_Handle os_make_mutex(void);
void os_destroy_mutex(Mutex_Handle m);
void os_lock_mutex(Mutex_Handle m);
void os_unlock_mutex(Mutex_Handle m);

typedef struct Binary_Semaphore Binary_Semaphore;
void os_binary_semaphore_init(Binary_Semaphore *sem, bool initial_state);
void os_binary_semaphore_destroy(Binary_Semaphore *sem);
void os_binary_semaphore_wait(Binary_Semaphore *sem);
void os_binary_semaphore_signal(Binary_Semaphore *sem);

void os_sleep(uint32_t ms);
void os_yield_thread(void);
void os_high_precision_sleep(double ms);

typedef struct File File;
void os_write_string_to_stdout(string s);
bool os_write_entire_file_s(string path, string data);
bool os_read_entire_file_s(string path, string *result, Allocator allocator);

#endif
