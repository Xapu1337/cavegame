#ifndef OOGABOOGA_DRAWING_H
#define OOGABOOGA_DRAWING_H

#include "string.h"
#include <stdint.h>

// Forward declare math types from linmath
typedef union Vector2f32 Vector2;
typedef union Vector4f32 Vector4;
typedef struct Matrix4 Matrix4;
typedef struct Gfx_Image Gfx_Image;
typedef struct Gfx_Font Gfx_Font;
typedef struct Draw_Quad Draw_Quad;
typedef struct Draw_Frame Draw_Frame;

Draw_Quad* draw_rect(Vector2 position, Vector2 size, Vector4 color);
Draw_Quad* draw_rect_xform(Matrix4 xform, Vector2 size, Vector4 color);
void draw_text(Gfx_Font *font, string text, uint32_t raster_height, Vector2 position, Vector2 scale, Vector4 color);
void draw_frame_reset(Draw_Frame *frame);

#endif
