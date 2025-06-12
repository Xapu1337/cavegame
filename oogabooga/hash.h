#ifndef OOGABOOGA_HASH_H
#define OOGABOOGA_HASH_H

#include <stdint.h>
#include "string.h"

uint64_t djb2_hash(string s);
uint64_t string_get_hash(string s);
uint64_t pointer_get_hash(void *p);
uint64_t float64_get_hash(double x);
uint64_t float32_get_hash(float x);

#endif
