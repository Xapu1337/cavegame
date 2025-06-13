#include "gfx_interface.h"
#include "drawing.h"
#include "memory.h"
#include <stdio.h>

void gfx_init(void) {
    printf("[INFO]: Software renderer initialized.\n");
    draw_frame_init(&drawFrame);
    DrawFrameReset(&drawFrame);
}

void GfxUpdate(void) {
    gfx_render_draw_frame_to_window(&drawFrame);
    DrawFrameReset(&drawFrame);
}

void gfx_render_draw_frame(Draw_Frame *frame, Gfx_Image *target) {
    (void)frame;
    (void)target;
}

void gfx_render_draw_frame_to_window(Draw_Frame *frame) {
    (void)frame;
}

void gfx_init_image(Gfx_Image *image, void *data, bool render_target) {
    (void)render_target;
    size_t size = (size_t)image->width * image->height * image->channels;
    image->gfx_handle = Alloc(image->allocator, size);
    if (data) {
        memcpy(image->gfx_handle, data, size);
    }
}

void gfx_set_image_data(Gfx_Image *image, u32 x, u32 y, u32 w, u32 h, void *data) {
    (void)image; (void)x; (void)y; (void)w; (void)h; (void)data;
}

void gfx_read_image_data(Gfx_Image *image, u32 x, u32 y, u32 w, u32 h, void *output) {
    (void)image; (void)x; (void)y; (void)w; (void)h; (void)output;
}

void gfx_deinit_image(Gfx_Image *image) {
    if (image->gfx_handle) {
        Dealloc(image->allocator, image->gfx_handle);
        image->gfx_handle = NULL;
    }
}

Gfx_Image *load_png_from_disk(string path, Allocator allocator) {
    (void)path;
    (void)allocator;
    return NULL;
}
