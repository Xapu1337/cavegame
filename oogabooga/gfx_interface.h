#ifndef OOGABOOGA_GFX_INTERFACE_H
#define OOGABOOGA_GFX_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include "linmath.h"
#include "string.h"

typedef struct Gfx_Image Gfx_Image;
typedef struct Gfx_Font Gfx_Font;
typedef struct Draw_Frame Draw_Frame;
typedef struct Gfx_Shader_Extension {
    void *ps; void *cbuffer; uint64_t cbuffer_size;
} Gfx_Shader_Extension;

void gfx_init(void);
void GfxUpdate(void);
void gfx_render_draw_frame(Draw_Frame *frame, Gfx_Image *target);
void gfx_render_draw_frame_to_window(Draw_Frame *frame);

Gfx_Image *load_png_from_disk(string path, Allocator allocator);
void delete_image(Gfx_Image *image);

#endif
