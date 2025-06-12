#ifndef OOGABOOGA_RANDOM_H
#define OOGABOOGA_RANDOM_H

#include <stdint.h>

uint64_t peek_random(void);
uint64_t get_random(void);
float get_random_float32(void);
double get_random_float64(void);
float get_random_float32_in_range(float min, float max);
double get_random_float64_in_range(double min, double max);
int64_t get_random_int_in_range(int64_t min, int64_t max);

#endif
