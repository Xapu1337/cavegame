#ifndef OOGABOOGA_GROWING_ARRAY_H
#define OOGABOOGA_GROWING_ARRAY_H

#include <stdint.h>
#include "base.h"

void  growing_array_init(void **array, uint64_t block_size_in_bytes, Allocator allocator);
void  growing_array_init_reserve(void **array, uint64_t block_size_in_bytes, uint64_t count_to_reserve, Allocator allocator);
void  growing_array_deinit(void **array);
void* growing_array_add_empty(void **array);
void  growing_array_reserve(void **array, uint64_t count_to_reserve);
void  growing_array_resize(void **array, uint64_t new_count);
void  growing_array_pop(void **array);

#endif
