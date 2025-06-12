#ifndef OOGABOOGA_UTILITY_H
#define OOGABOOGA_UTILITY_H

#include <stdint.h>

void radix_sort(void *collection, void *help_buffer, uint64_t item_count, uint64_t item_size, uint64_t sort_value_offset_in_item, uint64_t number_of_bits);
void merge_sort(void *collection, void *help_buffer, uint64_t item_count, uint64_t item_size, int (*compare)(const void *, const void *));

#endif
