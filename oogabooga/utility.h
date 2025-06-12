#ifndef OOGABOOGA_UTILITY_H
#define OOGABOOGA_UTILITY_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Type definitions
typedef uint64_t u64;
typedef int64_t s64;
typedef uint32_t u32;
typedef int32_t s32;
typedef float f32;
typedef double f64;

// Function declarations (definitions are in utility.c)
u64 max(u64 a, u64 b);
u64 min(u64 a, u64 b);
s64 max_s64(s64 a, s64 b);
s64 min_s64(s64 a, s64 b);
f32 max_f32(f32 a, f32 b);
f32 min_f32(f32 a, f32 b);
f64 max_f64(f64 a, f64 b);
f64 min_f64(f64 a, f64 b);

// Utility function for byte comparison
bool bytes_match(void *a, void *b, u64 count);

void radix_sort(void *collection, void *help_buffer, uint64_t item_count, uint64_t item_size, uint64_t sort_value_offset_in_item, uint64_t number_of_bits);
void merge_sort(void *collection, void *help_buffer, uint64_t item_count, uint64_t item_size, int (*compare)(const void *, const void *));

#endif
