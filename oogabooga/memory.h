#ifndef OOGABOOGA_MEMORY_H
#define OOGABOOGA_MEMORY_H

#include <stdint.h>

typedef struct Allocator Allocator;
void   temporary_storage_init(uint64_t arena_size);
void*  talloc(uint64_t size);
void   reset_temporary_storage(void);
Allocator get_temporary_allocator(void);

#endif
