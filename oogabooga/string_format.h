#ifndef OOGABOOGA_STRING_FORMAT_H
#define OOGABOOGA_STRING_FORMAT_H

#include "string.h"
#include "base.h"

string string_printf(Allocator allocator, const char *fmt, ...);
string sprints(Allocator allocator, const string fmt, ...);
string tprintf(const char *fmt, ...);

#endif
