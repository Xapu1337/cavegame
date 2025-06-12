#ifndef OOGABOOGA_BASE_H
#define OOGABOOGA_BASE_H

#include <stdint.h>
#include <stdbool.h>

#ifndef CONTEXT_EXTRA
#define CONTEXT_EXTRA struct { int unused; }
#endif

typedef enum Allocator_Message {
    ALLOCATOR_ALLOCATE,
    ALLOCATOR_DEALLOCATE,
    ALLOCATOR_REALLOCATE,
} Allocator_Message;

typedef void*(*Allocator_Proc)(uint64_t, void*, Allocator_Message, void*);

typedef struct Allocator {
    Allocator_Proc proc;
    void *data;
} Allocator;

typedef struct Context {
    void *logger; /* function pointer for custom logging */
    uint64_t thread_id;
    CONTEXT_EXTRA extra;
} Context;

Allocator get_heap_allocator(void);
Allocator get_temporary_allocator(void);
Context   get_context(void);
void*     alloc(Allocator allocator, uint64_t size);
void*     alloc_uninitialized(Allocator allocator, uint64_t size);
void      dealloc(Allocator allocator, void *p);
void      push_context(Context c);
void      pop_context(void);
uint64_t  get_next_power_of_two(uint64_t x);

#endif
