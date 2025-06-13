#ifndef OOGABOOGA_DRAWING_H
#define OOGABOOGA_DRAWING_H

#include "string.h"
#include "linmath.h"
#include "gfx_interface.h"
#include <stdint.h>
#include <stdbool.h>

// Stack size constants
#define SCISSOR_STACK_MAX 4096
#define Z_STACK_MAX 4096
#define MAX_BOUND_IMAGES 16

// Platform-specific deprecation macro
#if defined(_MSC_VER)
    #define DEPRECATED(proc, msg) __declspec(deprecated(msg)) proc
#elif defined(__GNUC__)
    #define DEPRECATED(proc, msg) __attribute__((deprecated(msg))) proc 
#else
    #define DEPRECATED(proc, msg) proc
#endif

// Basic types
#include "utility.h"

typedef struct Draw_Frame {
	Matrix4 projection;
	// #Cleanup
	union {
		DEPRECATED(Matrix4 view, "Use Draw_Frame.cameraXform instead");
		Matrix4 cameraXform;
	};
	
	void *cbuffer;
	
	uint64_t scissor_count;
	Vector4 scissor_stack[SCISSOR_STACK_MAX];
	
	Draw_Quad *quad_buffer;
	
	uint64_t z_count;
	int32_t z_stack[Z_STACK_MAX];
	bool enable_z_sorting;
	
	Gfx_Shader_Extension shader_extension;
	
	Gfx_Image *bound_images[MAX_BOUND_IMAGES];
	int highest_bound_slot_index;
	
} Draw_Frame;

Draw_Quad* draw_rect(Vector2 position, Vector2 size, Vector4 color);
Draw_Quad* DrawRectXform(Matrix4 xform, Vector2 size, Vector4 color);
void draw_text(Gfx_Font *font, string text, u32 rasterHeight, Vector2 position, Vector2 scale, Vector4 color);
void DrawFrameReset(Draw_Frame *frame);

#endif
