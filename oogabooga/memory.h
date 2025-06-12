#ifndef OOGABOOGA_MEMORY_H
#define OOGABOOGA_MEMORY_H

#include <stdint.h>
#include <stdbool.h>

// Forward declarations
typedef struct Allocator Allocator;
typedef struct Arena Arena;
typedef struct Heap_Block Heap_Block;
typedef struct Heap_Allocation_Metadata Heap_Allocation_Metadata;
typedef struct Heap_Search_Result Heap_Search_Result;

// Initialization allocator
Allocator get_initialization_allocator(void);

// Heap allocator functions
void heap_init(void);
Allocator GetHeapAllocator(void);
void heap_dealloc(void *p);

// Temporary storage functions
void temporary_storage_init(uint64_t arena_size);
void* talloc(uint64_t size);
void ResetTemporaryStorage(void);
Allocator GetTemporaryAllocator(void);

// Arena functions
Arena make_arena(uint64_t size);

// Utility functions
bool is_pointer_in_program_memory(void *p);
bool is_pointer_in_stack(void* p);
bool is_pointer_in_static_memory(void* p);
bool is_pointer_valid(void *p);

// Internal functions (for debugging/testing)
uint64_t get_heap_block_size_excluding_metadata(Heap_Block *block);
uint64_t get_heap_block_size_including_metadata(Heap_Block *block);
void sanity_check_block(Heap_Block *block);

#endif
