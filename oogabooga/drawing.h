#ifndef OOGABOOGA_DRAWING_H
#define OOGABOOGA_DRAWING_H

#include "string.h"
#include "linmath.h"
#include "gal.h"
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

// Drawing constants and types
#define VERTEX_USER_DATA_COUNT 8

// Quad types for different rendering modes
typedef enum {
    QUAD_TYPE_REGULAR = 0,
    QUAD_TYPE_CIRCLE,
    QUAD_TYPE_TEXT,
    QUAD_TYPE_COUNT
} Quad_Type;

// Filter modes (aliases for GAL types)
#define GFX_FILTER_MODE_NEAREST GAL_FILTER_MODE_NEAREST
#define GFX_FILTER_MODE_LINEAR GAL_FILTER_MODE_LINEAR
typedef GAL_Filter_Mode Gfx_Filter_Mode;

// Forward declarations for GAL-based types
typedef struct Gal_Image Gal_Image;
typedef struct Gal_Font Gal_Font;
typedef struct Gal_Shader_Extension {
    void *ps; void *cbuffer; uint64_t cbuffer_size;
} Gal_Shader_Extension;

typedef struct Draw_Quad {
    // BEWARE !! These are in ndc
    Vector2 bottom_left, top_left, top_right, bottom_right;
    // r, g, b, a
    Vector4 color;
    Gal_Image *image;
    Gfx_Filter_Mode image_min_filter;
    Gfx_Filter_Mode image_mag_filter;
    s32 z;
    uint8_t type;
    bool has_scissor;
    // x1, y1, x2, y2
    Vector4 uv;
    Vector4 scissor;
    
    Vector4 userdata[VERTEX_USER_DATA_COUNT]; // #Volatile do NOT change this to a pointer
} Draw_Quad;

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
	
	Gal_Shader_Extension shader_extension;
	
	Gal_Image *bound_images[MAX_BOUND_IMAGES];
	int highest_bound_slot_index;
	
} Draw_Frame;

// Draw frame initialization functions
void draw_frame_init(Draw_Frame* frame);
void draw_frame_init_reserve(Draw_Frame* frame, uint64_t number_of_quads_to_reserve);

// Function declarations
Draw_Quad* draw_rect(Vector2 position, Vector2 size, Vector4 color);
Draw_Quad* DrawRectXform(Matrix4 xform, Vector2 size, Vector4 color);
void draw_text(Gal_Font *font, string text, u32 rasterHeight, Vector2 position, Vector2 scale, Vector4 color);
void DrawFrameReset(Draw_Frame *frame);

// Global draw frame
extern Draw_Frame drawFrame;

#endif
