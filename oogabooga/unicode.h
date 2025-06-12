#ifndef OOGABOOGA_UNICODE_H
#define OOGABOOGA_UNICODE_H

#include <stdint.h>

int utf16_to_utf32(const uint16_t *utf16, uint64_t length, uint32_t *utf32);

#endif
