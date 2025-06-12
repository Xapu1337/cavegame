#ifndef OOGABOOGA_BASE_H
#define OOGABOOGA_BASE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Context Context;
typedef struct Allocator Allocator;

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
