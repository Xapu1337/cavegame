#ifndef OOGABOOGA_GAL_D3D11_C
#define OOGABOOGA_GAL_D3D11_C

/**
 * Direct3D 11 Backend Implementation for Graphics Abstraction Layer (GAL)
 */

#ifdef RENDERER_D3D11
#if RENDERER_D3D11

#include "gal.h"
#include "base.h"
#include "memory.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

// D3D11 implementation data
typedef struct D3D11_Renderer_Data {
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapchain;
    ID3D11RenderTargetView* main_render_target_view;
    ID3D11Texture2D* depth_stencil_buffer;
    ID3D11DepthStencilView* depth_stencil_view;
    ID3D11DepthStencilState* depth_stencil_state;
    ID3D11RasterizerState* rasterizer_state;
    ID3D11BlendState* blend_state;
    HWND window_handle;
    uint32_t window_width;
    uint32_t window_height;
    bool vsync_enabled;
} D3D11_Renderer_Data;

static D3D11_Renderer_Data* d3d11_data = NULL;

// Forward declarations
static GAL_Result d3d11_initialize(void);
static void d3d11_shutdown(void);
static GAL_Result d3d11_create_window(GAL_Window_Desc* desc);
static void d3d11_destroy_window(void);
static void d3d11_update(void);
static void d3d11_present(void);
static void d3d11_begin_frame(void);
static void d3d11_end_frame(void);
static void d3d11_render_draw_frame(Draw_Frame* frame, GAL_Texture_Handle target);
static void d3d11_render_draw_frame_to_window(Draw_Frame* frame);
static GAL_Texture_Handle d3d11_create_texture(GAL_Texture_Desc* desc, Allocator allocator);
static void d3d11_update_texture(GAL_Texture_Handle texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data);
static void d3d11_destroy_texture(GAL_Texture_Handle texture, Allocator allocator);

// Logging functions
static void d3d11_log_verbose(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[D3D11 VERBOSE] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void d3d11_log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[D3D11 INFO] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void d3d11_log_warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[D3D11 WARNING] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void d3d11_log_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[D3D11 ERROR] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

// Implementation functions
static GAL_Result d3d11_initialize(void) {
    d3d11_log_info("Initializing Direct3D 11 renderer");
    
    // Allocate implementation data
    d3d11_data = (D3D11_Renderer_Data*)calloc(1, sizeof(D3D11_Renderer_Data));
    if (!d3d11_data) {
        d3d11_log_error("Failed to allocate D3D11 renderer data");
        return GAL_RESULT_ERROR_OUT_OF_MEMORY;
    }
    
    d3d11_log_info("Direct3D 11 renderer initialized successfully");
    return GAL_RESULT_SUCCESS;
}

static void d3d11_shutdown(void) {
    d3d11_log_info("Shutting down Direct3D 11 renderer");
    
    if (d3d11_data) {
        // Release D3D11 resources
        if (d3d11_data->blend_state) d3d11_data->blend_state->Release();
        if (d3d11_data->rasterizer_state) d3d11_data->rasterizer_state->Release();
        if (d3d11_data->depth_stencil_state) d3d11_data->depth_stencil_state->Release();
        if (d3d11_data->depth_stencil_view) d3d11_data->depth_stencil_view->Release();
        if (d3d11_data->depth_stencil_buffer) d3d11_data->depth_stencil_buffer->Release();
        if (d3d11_data->main_render_target_view) d3d11_data->main_render_target_view->Release();
        if (d3d11_data->swapchain) d3d11_data->swapchain->Release();
        if (d3d11_data->context) d3d11_data->context->Release();
        if (d3d11_data->device) d3d11_data->device->Release();
        
        free(d3d11_data);
        d3d11_data = NULL;
    }
    
    d3d11_log_info("Direct3D 11 renderer shutdown complete");
}

static GAL_Result d3d11_create_window(GAL_Window_Desc* desc) {
    d3d11_log_info("Creating D3D11 window: %dx%d", desc->width, desc->height);
    
    if (!d3d11_data) {
        d3d11_log_error("D3D11 renderer not initialized");
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    
    d3d11_data->window_width = desc->width;
    d3d11_data->window_height = desc->height;
    d3d11_data->vsync_enabled = desc->vsync;
    
    // TODO: Implement actual D3D11 window creation and device initialization
    d3d11_log_warning("D3D11 window creation not fully implemented yet");
    
    return GAL_RESULT_SUCCESS;
}

static void d3d11_destroy_window(void) {
    d3d11_log_info("Destroying D3D11 window");
    // TODO: Implement D3D11 window destruction
}

static void d3d11_update(void) {
    // TODO: Implement D3D11 update logic
}

static void d3d11_present(void) {
    if (d3d11_data && d3d11_data->swapchain) {
        d3d11_data->swapchain->Present(d3d11_data->vsync_enabled ? 1 : 0, 0);
    }
}

static void d3d11_begin_frame(void) {
    // TODO: Implement D3D11 frame begin
}

static void d3d11_end_frame(void) {
    // TODO: Implement D3D11 frame end
}

static void d3d11_render_draw_frame(Draw_Frame* frame, GAL_Texture_Handle target) {
    d3d11_log_warning("D3D11 render_draw_frame not implemented yet");
    // TODO: Implement D3D11 rendering
}

static void d3d11_render_draw_frame_to_window(Draw_Frame* frame) {
    d3d11_log_warning("D3D11 render_draw_frame_to_window not implemented yet");
    // TODO: Implement D3D11 window rendering
}

static GAL_Texture_Handle d3d11_create_texture(GAL_Texture_Desc* desc, Allocator allocator) {
    d3d11_log_warning("D3D11 create_texture not implemented yet");
    // TODO: Implement D3D11 texture creation
    return NULL;
}

static void d3d11_update_texture(GAL_Texture_Handle texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data) {
    d3d11_log_warning("D3D11 update_texture not implemented yet");
    // TODO: Implement D3D11 texture update
}

static void d3d11_destroy_texture(GAL_Texture_Handle texture, Allocator allocator) {
    d3d11_log_warning("D3D11 destroy_texture not implemented yet");
    // TODO: Implement D3D11 texture destruction
}

// Create D3D11 renderer instance
GAL_Renderer* d3d11_create_renderer(void) {
    GAL_Renderer* renderer = (GAL_Renderer*)calloc(1, sizeof(GAL_Renderer));
    if (!renderer) {
        return NULL;
    }
    
    renderer->backend = GAL_BACKEND_D3D11;
    renderer->name = "Direct3D 11";
    renderer->api_version = 11;
    
    // Set logging functions
    renderer->log_verbose = d3d11_log_verbose;
    renderer->log_info = d3d11_log_info;
    renderer->log_warning = d3d11_log_warning;
    renderer->log_error = d3d11_log_error;
    
    // Set core functions
    renderer->initialize = d3d11_initialize;
    renderer->shutdown = d3d11_shutdown;
    renderer->create_window = d3d11_create_window;
    renderer->destroy_window = d3d11_destroy_window;
    renderer->update = d3d11_update;
    renderer->present = d3d11_present;
    
    // Set resource management functions
    renderer->create_texture = d3d11_create_texture;
    renderer->update_texture = d3d11_update_texture;
    renderer->destroy_texture = d3d11_destroy_texture;
    
    // Set rendering functions
    renderer->begin_frame = d3d11_begin_frame;
    renderer->end_frame = d3d11_end_frame;
    renderer->render_draw_frame = d3d11_render_draw_frame;
    renderer->render_draw_frame_to_window = d3d11_render_draw_frame_to_window;
    
    // TODO: Set advanced functions when implemented
    renderer->compile_shader = NULL;
    renderer->destroy_shader = NULL;
    renderer->reserve_vertex_buffer = NULL;
    renderer->clear_render_target = NULL;
    
    renderer->implementation_data = d3d11_data;
    
    return renderer;
}

#else
#error "Direct3D 11 renderer not enabled"
#endif // RENDERER_D3D11

#else
#error "Direct3D 11 renderer requires Windows"
#endif // _WIN32

#endif // OOGABOOGA_GAL_D3D11_C
