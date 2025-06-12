#ifndef OOGABOOGA_FONT_H
#define OOGABOOGA_FONT_H

#include "string.h"
#include "base.h"

typedef struct Gfx_Font Gfx_Font;
Gfx_Font *load_font_from_disk(string path, Allocator allocator);

#endif
