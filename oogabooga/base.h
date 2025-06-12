#ifndef OOGABOOGA_BASE_H
#define OOGABOOGA_BASE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Forward declarations
typedef struct Context Context;
typedef struct string string;

// Utility macros
#define FIRST_ARG(arg1, ...) arg1
#define SECOND_ARG(arg1, arg2, ...) arg2

// Print macros
#define print(...) _Generic((FIRST_ARG(__VA_ARGS__)), \
                           string:  prints, \
                           default: printf \
                          )(__VA_ARGS__)

// Forward declarations for print functions
void prints(const string fmt, ...);
string tprintf(const char *fmt, ...);

// Allocator system types
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

// Function declarations
Allocator GetHeapAllocator(void);
Allocator GetTemporaryAllocator(void);
Context   get_context(void);
void*     Alloc(Allocator allocator, uint64_t size);
void*     alloc_uninitialized(Allocator allocator, uint64_t size);
void      Dealloc(Allocator allocator, void *p);
void      push_context(Context c);
void      pop_context(void);
uint64_t  get_next_power_of_two(uint64_t x);

#endif

