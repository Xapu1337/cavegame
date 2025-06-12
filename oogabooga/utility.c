#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Type definitions (if not already available)
#ifndef _COMMON_TYPES_DEFINED
#define _COMMON_TYPES_DEFINED
typedef uint64_t u64;
typedef int64_t s64;
typedef uint32_t u32;
typedef int32_t s32;
typedef float f32;
typedef double f64;
typedef float float32;
typedef double float64;
#endif

// Missing macros
#ifndef local_persist
#define local_persist static
#endif

#define TAU32 (2.0f * PI32)
#define TAU64 (2.0 * PI32)
#define RAD_PER_DEG (PI32 / 180.0)
#define DEG_PER_RAD (180.0 / PI32)

#define to_radians(degrees) ((degrees)*RAD_PER_DEG)
#define to_degrees(radians) ((radians)*DEG_PER_RAD)

// Basic math utility functions
u64 max(u64 a, u64 b) { return a > b ? a : b; }
u64 min(u64 a, u64 b) { return a < b ? a : b; }
s64 max_s64(s64 a, s64 b) { return a > b ? a : b; }
s64 min_s64(s64 a, s64 b) { return a < b ? a : b; }
f32 max_f32(f32 a, f32 b) { return a > b ? a : b; }
f32 min_f32(f32 a, f32 b) { return a < b ? a : b; }
f64 max_f64(f64 a, f64 b) { return a > b ? a : b; }
f64 min_f64(f64 a, f64 b) { return a < b ? a : b; }

// This is a very niche sort algorithm.
// I use it for Z sorting quads.
// help_buffer should be same size as collection.
// This only works with integers, and it will use the first number_of_bits in the integer
// at sort_value_offset_in_item for sorting.
// There is a cost of memory as we need to double the buffer we're sorting BUT the performance
// gain is very promising.
// At 21 bits I'm able to sort a completely randomized collection of 100k integers at around
// 8m cycles (or 2.5-2.6ms on my shitty laptop i5-11300H)
void radix_sort(void *collection, void *help_buffer, u64 item_count, u64 item_size, u64 sort_value_offset_in_item, u64 number_of_bits) {
    local_persist const int RADIX = 256;
    local_persist const int BITS_PER_PASS = 8;
    
    const int PASS_COUNT = ((number_of_bits + BITS_PER_PASS - 1) / BITS_PER_PASS);
    const u64 HALF_RANGE_OF_VALUE_BITS = 1ULL << (number_of_bits - 1);

    u64 count[RADIX];
    u64 prefix_sum[RADIX];

    for (u32 pass = 0; pass < PASS_COUNT; ++pass) {
        u32 shift = pass * BITS_PER_PASS;

        memset(count, 0, sizeof(count));

        for (u64 i = 0; i < item_count; ++i) {
        	uint8_t *item = (uint8_t*)collection + i * item_size;
        	
            u64 sort_value = *(u64*)(item + sort_value_offset_in_item);
            sort_value += HALF_RANGE_OF_VALUE_BITS; // We treat the value as a signed integer
            
            u32 digit = (sort_value >> shift) & (RADIX-1);
            ++count[digit];
        }

        prefix_sum[0] = 0;
        for (u32 i = 1; i < RADIX; ++i) {
            prefix_sum[i] = prefix_sum[i - 1] + count[i - 1];
        }

        for (u64 i = 0; i < item_count; ++i) {
        	uint8_t *item = (uint8_t*)collection + i * item_size;
        	
            u64 sort_value = *(u64*)(item + sort_value_offset_in_item);
            sort_value += HALF_RANGE_OF_VALUE_BITS; // We treat the value as a signed integer
            
            u32 digit = (sort_value >> shift) & (RADIX-1);
            memcpy((uint8_t*)help_buffer + prefix_sum[digit] * item_size, item, item_size);
            ++prefix_sum[digit];
        }

        memcpy(collection, help_buffer, item_count * item_size);
    }
}

void merge_sort(void *collection, void *help_buffer, u64 item_count, u64 item_size, int (*compare)(const void *, const void *)) {
    uint8_t *items = (uint8_t *)collection;
    uint8_t *buffer = (uint8_t *)help_buffer;

    for (u64 width = 1; width < item_count; width *= 2) {
        for (u64 i = 0; i < item_count; i += 2 * width) {
            u64 left = i;
            u64 right = (i + width < item_count) ? (i + width) : item_count;
            u64 end = (i + 2 * width < item_count) ? (i + 2 * width) : item_count;

            u64 left_index = left;
            u64 right_index = right;
            u64 k = left;

            while (left_index < right && right_index < end) {
                if (compare(items + left_index * item_size, items + right_index * item_size) <= 0) {
                    memcpy(buffer + k * item_size, items + left_index * item_size, item_size);
                    left_index++;
                } else {
                    memcpy(buffer + k * item_size, items + right_index * item_size, item_size);
                    right_index++;
                }
                k++;
            }

            while (left_index < right) {
                memcpy(buffer + k * item_size, items + left_index * item_size, item_size);
                left_index++;
                k++;
            }

            while (right_index < end) {
                memcpy(buffer + k * item_size, items + right_index * item_size, item_size);
                right_index++;
                k++;
            }

            for (u64 j = left; j < end; j++) {
                memcpy(items + j * item_size, buffer + j * item_size, item_size);
            }
        }
    }
}

inline bool bytes_match(void *a, void *b, u64 count) { return memcmp(a, b, count) == 0; }

#define swap(a, b, type) { type t = a; a = b; b = t;  }


// This isn't really linmath but just putting it here for now
#define clamp(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

#define lerpf lerpf32
f32 lerpf32(f32 from, f32 to, f32 x) {
	return (to-from)*x+from;
}
f64 lerpf64(f64 from, f64 to, f64 x) {
	return (to-from)*x+from;
}
s64 lerpi(s64 from, s64 to, f64 x) {
	return (s64)((round((f64)to-(f64)from)*x)+from);
}

#define smerpf smerpf32
f32 smerpf32(f32 from, f32 to, f32 t) {
	float32 smooth = t * t * (3.0 - 2.0 * t);
	return lerpf(from, to, smooth);
}
f64 smerpf64(f64 from, f64 to, f64 t) {
	float64 smooth = t * t * (3.0 - 2.0 * t);
	return lerpf(from, to, smooth);
}
s64 smerpi(s64 from, s64 to, f64 t) {
	float64 smooth = t * t * (3.0 - 2.0 * t);
	return lerpi(from, to, smooth);
}
// I don't know how to describe this one I just made this in desmos and it has been useful for cool stuff:
// https://www.desmos.com/calculator/r2etlhi2ej
float32 sine_oscillate_n_waves_normalized(float32 v, float32 n) {
	return (sin((n*2*PI32*((v)-(1/(n*4))))+1))/2;
}


float64 string_to_float(string s, bool *success) {

    *success = true;
    float64 result = 0.0;
    float64 sign = 1.0;
    bool has_decimal = false;
    float64 decimal_place = 0.1;

    u64 i = 0;
    // Skip leading spaces
    while (i < s.count && (s.data[i] == ' ' || s.data[i] == '\t')) {
        i++;
    }

    // Handle optional sign
    if (i < s.count && s.data[i] == '-') {
        sign = -1.0;
        i++;
    } else if (i < s.count && s.data[i] == '+') {
        i++;
    }

    while (i < s.count) {
        if (s.data[i] >= '0' && s.data[i] <= '9') {
            if (has_decimal) {
                result += (s.data[i] - '0') * decimal_place;
                decimal_place *= 0.1;
            } else {
                result = result * 10.0 + (s.data[i] - '0');
            }
        } else if (s.data[i] == '.') {
            if (has_decimal) {
                // Multiple decimal points are invalid
                *success = false;
                return 0.0;
            }
            has_decimal = true;
        } else {
            *success = false;
            return 0.0;
        }
        i++;
    }

    return result * sign;
}

s64 string_to_int(string s, bool *success) {
    *success = true;
    s64 result = 0;
    s64 sign = 1;

    u64 i = 0;
    // Skip leading spaces
    while (i < s.count && (s.data[i] == ' ' || s.data[i] == '\t')) {
        i++;
    }

    // Handle optional sign
    if (i < s.count && s.data[i] == '-') {
        sign = -1;
        i++;
    } else if (i < s.count && s.data[i] == '+') {
        i++;
    }

    while (i < s.count) {
        if (s.data[i] >= '0' && s.data[i] <= '9') {
            result = result * 10 + (s.data[i] - '0');
        } else {
            *success = false;
            return 0;
        }
        i++;
    }

    return result * sign;
}