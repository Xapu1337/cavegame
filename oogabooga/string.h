#ifndef OOGABOOGA_STRING_H
#define OOGABOOGA_STRING_H

#include <stdint.h>
#include "base.h"

typedef struct string { uint64_t count; uint8_t *data; } string;

// String literal macro
#define STR(s) ((string){ LengthOfNullTerminatedString((const char*)s), (uint8_t*)s })

typedef struct String_Builder {
    union { struct {uint64_t count; uint8_t *buffer;}; string result; };
    uint64_t buffer_capacity;
    Allocator allocator;
} String_Builder;

uint64_t LengthOfNullTerminatedString(const char *cstring);
string    AllocString(Allocator allocator, uint64_t count);
void      DeallocString(Allocator allocator, string s);
string    talloc_string(uint64_t count);
string    StringConcat(string left, string right, Allocator allocator);
char*     ConvertToNullTerminatedString(string s, Allocator allocator);
char*     TempConvertToNullTerminatedString(string s);
bool      StringsMatch(string a, string b);
string    string_view(string s, uint64_t startIndex, uint64_t count);
int64_t   StringFindFromLeft(string s, string sub);
int64_t   string_find_from_right(string s, string sub);
bool      string_starts_with(string s, string sub);
string    StringCopy(string s, Allocator allocator);
void      string_builder_reserve(String_Builder *b, uint64_t required_capacity);
void      string_builder_init_reserve(String_Builder *b, uint64_t reserved_capacity, Allocator allocator);
void      string_builder_init(String_Builder *b, Allocator allocator);
void      string_builder_deinit(String_Builder *b);
void      string_builder_append(String_Builder *b, string s);
string    string_builder_get_string(String_Builder b);
string    string_replace_all(string s, string old, string new, Allocator allocator);
string    string_trim_left(string s);
string    string_trim_right(string s);
string    string_trim(string s);

#endif
