#ifndef GAL_IMPLEMENTATION_INCLUDED
#define GAL_IMPLEMENTATION_INCLUDED

#include "gal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Forward declarations for drawing system
extern Draw_Frame drawFrame;
extern void DrawFrameReset(Draw_Frame* frame);

// Forward declarations for renderer implementations "gal.h"
#include "gal_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Forward declarations for renderer implementations - conditional based on build flags
#if defined(RENDERER_OPENGL) && RENDERER_OPENGL
extern GAL_Renderer opengl_create_renderer(void);
#endif
#if defined(RENDERER_VULKAN) && RENDERER_VULKAN
extern GAL_Renderer vulkan_create_renderer(void);
#endif
#if defined(RENDERER_D3D11) && RENDERER_D3D11
extern GAL_Renderer d3d11_create_renderer(void);
#endif
#if defined(RENDERER_SOFTWARE) && RENDERER_SOFTWARE
extern GAL_Renderer software_create_renderer(void);
#endif

// Global state
static struct {
    GAL_Renderer* active_renderer;
    GAL_Backend active_backend;
    bool initialized;
    bool attempted_backends[5]; // Track which backends we've attempted to use
} g_gal = {0};

// Default logging functions
static void default_log_verbose(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[VERBOSE] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void default_log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[INFO] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void default_log_warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[WARNING] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void default_log_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

// Create a fallback renderer with no functionality
static GAL_Renderer create_fallback_renderer(void) {
    GAL_Renderer renderer = {0};
    
    renderer.backend = GAL_BACKEND_UNKNOWN;
    renderer.name = "Fallback Renderer";
    renderer.api_version = 0;
    
    // Set up logging functions
    renderer.log_verbose = default_log_verbose;
    renderer.log_info = default_log_info;
    renderer.log_warning = default_log_warning;
    renderer.log_error = default_log_error;
    
    // All other functions return errors
    
    return renderer;
}

// Attempt to initialize a specific backend
static GAL_Result attempt_initialize_backend(GAL_Backend backend) {
    if (g_gal.attempted_backends[backend]) {
        default_log_verbose("Backend %d already attempted, skipping", backend);
        return GAL_RESULT_ERROR_UNSUPPORTED_FEATURE;
    }
    
    g_gal.attempted_backends[backend] = true;
    
    GAL_Renderer renderer = {0};
    bool backend_available = false;
    
    // Try to create the requested backend
    switch (backend) {
        case GAL_BACKEND_OPENGL:
            #if defined(RENDERER_OPENGL) && RENDERER_OPENGL
                renderer = opengl_create_renderer();
                backend_available = true;
            #else
                default_log_warning("OpenGL renderer not available");
            #endif
            break;
            
        case GAL_BACKEND_VULKAN:
            #if defined(RENDERER_VULKAN) && RENDERER_VULKAN
                renderer = vulkan_create_renderer();
                backend_available = true;
            #else
                default_log_warning("Vulkan renderer not available");
            #endif
            break;
            
        case GAL_BACKEND_D3D11:
            #if defined(_WIN32) && defined(RENDERER_D3D11) && RENDERER_D3D11
                renderer = d3d11_create_renderer();
                backend_available = true;
            #else
                default_log_warning("D3D11 renderer not available");
            #endif
            break;
            
        case GAL_BACKEND_SOFTWARE:
            #if defined(RENDERER_SOFTWARE) && RENDERER_SOFTWARE
                renderer = software_create_renderer();
                backend_available = true;
            #else
                default_log_warning("Software renderer not available");
            #endif
            break;
            
        default:
            default_log_error("Unknown renderer type: %d", backend);
            return GAL_RESULT_ERROR_UNSUPPORTED_FEATURE;
    }
    
    if (!backend_available) {
        return GAL_RESULT_ERROR_UNSUPPORTED_FEATURE;
    }
    
    // Initialize the renderer
    if (renderer.initialize) {
        GAL_Result result = renderer.initialize();
        if (result != GAL_RESULT_SUCCESS) {
            default_log_error("Failed to initialize %s renderer: %d", renderer.name, result);
            
            // Clean up if the renderer has a shutdown function
            if (renderer.shutdown) {
                renderer.shutdown();
            }
            
            return result;
        }
    }
    
    // Copy the renderer to the active renderer
    *g_gal.active_renderer = renderer;
    g_gal.active_backend = backend;
    
    return GAL_RESULT_SUCCESS;
}

// Initialize the GAL with the specified backend, with fallback chain
GAL_Result gal_initialize(GAL_Backend preferred_backend) {
    // Return immediately if already initialized
    if (g_gal.initialized) {
        default_log_info("GAL already initialized with %s renderer", 
                          g_gal.active_renderer ? g_gal.active_renderer->name : "unknown");
        return GAL_RESULT_SUCCESS;
    }
    
    // Clear attempted backends
    memset(g_gal.attempted_backends, 0, sizeof(g_gal.attempted_backends));
    
    // Allocate memory for the active renderer
    g_gal.active_renderer = malloc(sizeof(GAL_Renderer));
    if (!g_gal.active_renderer) {
        default_log_error("Failed to allocate memory for renderer");
        return GAL_RESULT_ERROR_OUT_OF_MEMORY;
    }
    
    // Attempt the preferred backend first
    GAL_Result result = attempt_initialize_backend(preferred_backend);
    
    // For explicit backend builds, don't try fallbacks - fail if the requested backend doesn't work
    if (result != GAL_RESULT_SUCCESS) {
        default_log_error("Failed to initialize requested backend %s", 
                          gal_backend_to_string(preferred_backend));
        free(g_gal.active_renderer);
        g_gal.active_renderer = NULL;
        g_gal.initialized = false;
        return result;
    }
    
    // Mark as initialized
    g_gal.initialized = true;
    
    // Log the selected renderer
    default_log_info("GAL initialized with %s renderer", g_gal.active_renderer->name);
    
    return GAL_RESULT_SUCCESS;
}

// Shutdown the GAL
void gal_shutdown(void) {
    if (!g_gal.initialized) {
        return; // Nothing to do
    }
    
    if (g_gal.active_renderer) {
        // Call the renderer's shutdown function if available
        if (g_gal.active_renderer->shutdown) {
            g_gal.active_renderer->shutdown();
        }
        
        // Free the renderer
        free(g_gal.active_renderer);
        g_gal.active_renderer = NULL;
    }
    
    g_gal.initialized = false;
    default_log_info("GAL shutdown complete");
}

// Get the active renderer
GAL_Renderer* gal_get_renderer(void) {
    if (!g_gal.initialized || !g_gal.active_renderer) {
        static GAL_Renderer fallback = {0};
        fallback = create_fallback_renderer();
        return &fallback;
    }
    
    return g_gal.active_renderer;
}

// ================ GAL UTILITY FUNCTIONS FOR DRAWING SYSTEM ================

// Initialize GAL with auto-detection (only called by build-specified backend)
void gal_init(void) {
    // The build system specifies which backend to use via compile flags
    GAL_Backend backend = GAL_BACKEND_SOFTWARE; // Default fallback
    
    #if defined(RENDERER_OPENGL) && RENDERER_OPENGL
        backend = GAL_BACKEND_OPENGL;
    #elif defined(RENDERER_VULKAN) && RENDERER_VULKAN
        backend = GAL_BACKEND_VULKAN;
    #elif defined(RENDERER_D3D11) && RENDERER_D3D11
        backend = GAL_BACKEND_D3D11;
    #elif defined(RENDERER_SOFTWARE) && RENDERER_SOFTWARE
        backend = GAL_BACKEND_SOFTWARE;
    #endif
    
    GAL_Result result = gal_initialize(backend);
    if (result != GAL_RESULT_SUCCESS) {
        default_log_error("Failed to initialize GAL with specified backend, result: %d", result);
    }
    
    // Create a default window
    GAL_Window_Desc window_desc = {
        .title = "Oogabooga Engine",
        .width = 1280,
        .height = 720,
        .resizable = true,
        .vsync = true
    };
    
    GAL_Renderer *renderer = gal_get_renderer();
    if (renderer && renderer->create_window) {
        result = renderer->create_window(&window_desc);
        if (result != GAL_RESULT_SUCCESS) {
            default_log_error("Failed to create window");
        }
    }
}

// Update GAL renderer (called each frame)
void gal_update(void) {
    GAL_Renderer *renderer = gal_get_renderer();
    if (!renderer) return;
    
    // Update the renderer
    if (renderer->update) renderer->update();
    
    // Render the current frame to window
    gal_render_draw_frame_to_window(&drawFrame);
    
    // Reset for next frame  
    DrawFrameReset(&drawFrame);
}

// Create a Gal_Image from texture description
GAL_Result gal_create_image(Gal_Image* image, uint32_t width, uint32_t height, uint32_t channels, void* data, bool render_target, Allocator allocator) {
    if (!image) {
        return GAL_RESULT_ERROR_INVALID_PARAMETER;
    }
    
    GAL_Renderer *renderer = gal_get_renderer();
    if (!renderer || !renderer->create_texture) {
        return GAL_RESULT_ERROR_NOT_IMPLEMENTED;
    }
    
    // Set up texture description
    GAL_Texture_Desc desc = {
        .width = width,
        .height = height,
        .channels = channels,
        .format = (channels == 4) ? GAL_FORMAT_R8G8B8A8_UNORM : 
                  (channels == 3) ? GAL_FORMAT_R8G8B8_UNORM : GAL_FORMAT_R8_UNORM,
        .is_render_target = render_target,
        .initial_data = data,
        .filter_mode = GAL_FILTER_MODE_LINEAR
    };
    
    // Create the texture
    GAL_Texture_Handle texture = renderer->create_texture(&desc, allocator);
    if (!texture) {
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    
    // Fill the Gal_Image structure
    image->width = width;
    image->height = height;
    image->channels = channels;
    image->format = desc.format;
    image->gal_handle = texture;
    image->allocator = allocator;
    
    return GAL_RESULT_SUCCESS;
}

// Destroy a Gal_Image
void gal_destroy_image(Gal_Image* image) {
    if (!image || !image->gal_handle) {
        return;
    }
    
    GAL_Renderer *renderer = gal_get_renderer();
    if (renderer && renderer->destroy_texture) {
        renderer->destroy_texture(image->gal_handle, image->allocator);
    }
    
    image->gal_handle = NULL;
}

// Update image data
void gal_update_image_data(Gal_Image* image, uint32_t x, uint32_t y, uint32_t w, uint32_t h, void* data) {
    if (!image || !image->gal_handle || !data) {
        return;
    }
    
    GAL_Renderer *renderer = gal_get_renderer();
    if (renderer && renderer->update_texture) {
        renderer->update_texture(image->gal_handle, x, y, w, h, data);
    }
}

// Read image data (placeholder implementation)
void gal_read_image_data(Gal_Image* image, uint32_t x, uint32_t y, uint32_t w, uint32_t h, void* output) {
    // This would need to be implemented per-backend
    // For now, just log that it's not implemented
    if (image && output) {
        default_log_warning("gal_read_image_data not yet implemented");
    }
}

// Render draw frame to target
void gal_render_draw_frame(Draw_Frame* frame, Gal_Image* target) {
    GAL_Renderer *renderer = gal_get_renderer();
    if (!renderer || !frame || !target) return;
    
    if (renderer->render_draw_frame) {
        renderer->render_draw_frame(frame, target->gal_handle);
    }
}

// Render draw frame to window
void gal_render_draw_frame_to_window(Draw_Frame* frame) {
    GAL_Renderer *renderer = gal_get_renderer();
    if (!renderer || !frame) return;
    
    if (renderer->begin_frame) renderer->begin_frame();
    
    if (renderer->render_draw_frame_to_window) {
        renderer->render_draw_frame_to_window(frame);
    }
    
    if (renderer->end_frame) renderer->end_frame();
    if (renderer->present) renderer->present();
}

// Load PNG from disk (placeholder implementation)
Gal_Image* load_png_from_disk(string path, Allocator allocator) {
    // This would need to be implemented using stb_image or similar
    // For now, return NULL and log
    default_log_warning("load_png_from_disk not yet implemented for path: %.*s", (int)path.count, path.data);
    return NULL;
}

// Delete image (wrapper for gal_destroy_image)
void delete_image(Gal_Image* image) {
    if (image) {
        gal_destroy_image(image);
        // Note: The image struct itself should be freed by the caller if it was allocated
    }
}

#endif // GAL_IMPLEMENTATION_INCLUDED
