#ifndef OOGABOOGA_CONCURRENCY_H
#define OOGABOOGA_CONCURRENCY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Spinlock Spinlock;
void      spinlock_init(Spinlock *l);
void      spinlock_acquire_or_wait(Spinlock *l);
bool      spinlock_acquire_or_wait_timeout(Spinlock *l, double timeout_seconds);
void      spinlock_release(Spinlock *l);

typedef struct Mutex Mutex;
void      mutex_init(Mutex *m);
void      mutex_destroy(Mutex *m);
void      mutex_acquire_or_wait(Mutex *m);
void      mutex_release(Mutex *m);

#endif
