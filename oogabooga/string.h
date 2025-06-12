#ifndef OOGABOOGA_STRING_H
#define OOGABOOGA_STRING_H

#include <stdint.h>

typedef struct string { uint64_t count; uint8_t *data; } string;
typedef struct Allocator Allocator;

typedef struct String_Builder {
    union { struct {uint64_t count; uint8_t *buffer;}; string result; };
    uint64_t buffer_capacity;
    Allocator allocator;
} String_Builder;

uint64_t length_of_null_terminated_string(const char *cstring);
string    alloc_string(Allocator allocator, uint64_t count);
void      dealloc_string(Allocator allocator, string s);
string    talloc_string(uint64_t count);
string    string_concat(string left, string right, Allocator allocator);
char*     convert_to_null_terminated_string(string s, Allocator allocator);
char*     temp_convert_to_null_terminated_string(string s);
bool      strings_match(string a, string b);
string    string_view(string s, uint64_t start_index, uint64_t count);
int64_t   string_find_from_left(string s, string sub);
int64_t   string_find_from_right(string s, string sub);
bool      string_starts_with(string s, string sub);
string    string_copy(string s, Allocator allocator);
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
