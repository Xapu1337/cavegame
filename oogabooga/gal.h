#ifndef OOGABOOGA_GAL_H
#define OOGABOOGA_GAL_H

/**
 * Graphics Abstraction Layer (GAL) for Oogabooga Engine
 * 
 * This header defines a generic interface for graphics operations that can be
 * implemented by different backends (OpenGL, Vulkan, D3D11, Software, etc.).
 */

#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include "memory.h"
#include "utility.h"

// Forward declarations
typedef struct Draw_Frame Draw_Frame;
typedef struct GAL_Renderer GAL_Renderer;

// Common enums and types for all renderers
typedef enum GAL_Result {
    GAL_RESULT_SUCCESS = 0,
    GAL_RESULT_ERROR_INITIALIZATION_FAILED,
    GAL_RESULT_ERROR_INVALID_PARAMETER,
    GAL_RESULT_ERROR_OUT_OF_MEMORY,
    GAL_RESULT_ERROR_NOT_IMPLEMENTED,
    GAL_RESULT_ERROR_UNSUPPORTED_FEATURE,
    GAL_RESULT_ERROR_DEVICE_LOST,
    GAL_RESULT_ERROR_FORMAT_NOT_SUPPORTED,
    GAL_RESULT_ERROR_UNKNOWN
} GAL_Result;

typedef enum GAL_Backend {
    GAL_BACKEND_OPENGL,
    GAL_BACKEND_VULKAN,
    GAL_BACKEND_D3D11,
    GAL_BACKEND_SOFTWARE,
    GAL_BACKEND_METAL,
    GAL_BACKEND_UNKNOWN,
    GAL_BACKEND_AUTO = 100  // Special value for auto-detection
} GAL_Backend;

typedef enum GAL_Filter_Mode {
    GAL_FILTER_MODE_NEAREST,
    GAL_FILTER_MODE_LINEAR,
} GAL_Filter_Mode;

typedef enum GAL_Format {
    GAL_FORMAT_R8G8B8A8_UNORM,
    GAL_FORMAT_R8G8B8_UNORM,
    GAL_FORMAT_R8_UNORM,
    GAL_FORMAT_D24_UNORM_S8_UINT,
    GAL_FORMAT_UNKNOWN
} GAL_Format;

typedef enum GAL_Primitive_Type {
    GAL_PRIMITIVE_TYPE_TRIANGLE_LIST,
    GAL_PRIMITIVE_TYPE_TRIANGLE_STRIP,
    GAL_PRIMITIVE_TYPE_LINE_LIST,
    GAL_PRIMITIVE_TYPE_LINE_STRIP,
    GAL_PRIMITIVE_TYPE_POINT_LIST
} GAL_Primitive_Type;

// Resource handles - opaque pointers to implementation-specific resources
typedef void* GAL_Buffer_Handle;
typedef void* GAL_Texture_Handle;
typedef void* GAL_Shader_Handle;
typedef void* GAL_RenderTarget_Handle;

typedef struct GAL_Window_Desc {
    const char* title;
    uint32_t width;
    uint32_t height;
    bool fullscreen;
    bool resizable;
    bool vsync;
    void* existing_window;  // Optional, platform-specific window handle
} GAL_Window_Desc;

typedef struct GAL_Texture_Desc {
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    GAL_Format format;
    bool is_render_target;
    void* initial_data;     // Optional
    GAL_Filter_Mode filter_mode;
} GAL_Texture_Desc;

// Compatibility types for existing drawing system
typedef struct Gal_Image {
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    GAL_Format format;
    GAL_Texture_Handle gal_handle; // GAL texture handle
    Allocator allocator;
} Gal_Image;

typedef struct Gal_Font {
    // Font implementation details would go here
    void* font_data;
    uint32_t size;
    Allocator allocator;
} Gal_Font;

// Abstract renderer interface
struct GAL_Renderer {
    // Identification
    GAL_Backend backend;
    const char* name;
    uint32_t api_version;
    
    // Logging functions
    void (*log_verbose)(const char* fmt, ...);
    void (*log_info)(const char* fmt, ...);
    void (*log_warning)(const char* fmt, ...);
    void (*log_error)(const char* fmt, ...);
    
    // Core functions
    GAL_Result (*initialize)(void);
    void (*shutdown)(void);
    GAL_Result (*create_window)(GAL_Window_Desc* desc);
    void (*destroy_window)(void);
    void (*update)(void);
    void (*present)(void);
    
    // Resource management
    GAL_Texture_Handle (*create_texture)(GAL_Texture_Desc* desc, Allocator allocator);
    void (*update_texture)(GAL_Texture_Handle texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data);
    void (*destroy_texture)(GAL_Texture_Handle texture, Allocator allocator);
    
    // Rendering functions
    void (*begin_frame)(void);
    void (*end_frame)(void);
    void (*render_draw_frame)(Draw_Frame* frame, GAL_Texture_Handle target);
    void (*render_draw_frame_to_window)(Draw_Frame* frame);
    void (*clear_render_target)(GAL_RenderTarget_Handle target, float r, float g, float b, float a);
    
    // Advanced functions
    GAL_Result (*compile_shader)(string source_code, uint64_t cbuffer_size, void** shader_object);
    void (*destroy_shader)(void* shader_object);
    void (*reserve_vertex_buffer)(uint64_t bytes);
    
    // Internal implementation data
    void* implementation_data;
};

// Global functions
GAL_Result gal_initialize(GAL_Backend preferred_backend);
void gal_shutdown(void);
GAL_Renderer* gal_get_renderer(void);

// Helper functions that dispatch to the active renderer
#define gal_log_verbose(...) gal_get_renderer()->log_verbose(__VA_ARGS__)
#define gal_log_info(...) gal_get_renderer()->log_info(__VA_ARGS__)
#define gal_log_warning(...) gal_get_renderer()->log_warning(__VA_ARGS__)
#define gal_log_error(...) gal_get_renderer()->log_error(__VA_ARGS__)

// GAL utility functions for image management
GAL_Result gal_create_image(Gal_Image* image, uint32_t width, uint32_t height, uint32_t channels, void* data, bool render_target, Allocator allocator);
void gal_destroy_image(Gal_Image* image);
void gal_update_image_data(Gal_Image* image, uint32_t x, uint32_t y, uint32_t w, uint32_t h, void* data);
void gal_read_image_data(Gal_Image* image, uint32_t x, uint32_t y, uint32_t w, uint32_t h, void* output);

// GAL rendering functions for the drawing system
void gal_init(void);
void gal_update(void);
void gal_render_draw_frame(Draw_Frame* frame, Gal_Image* target);
void gal_render_draw_frame_to_window(Draw_Frame* frame);

// Utility function for loading images
Gal_Image* load_png_from_disk(string path, Allocator allocator);
void delete_image(Gal_Image* image);

#endif // OOGABOOGA_GAL_H
