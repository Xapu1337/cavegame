#include "gal.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

// Forward reference to the draw frame
typedef struct Draw_Frame Draw_Frame;
extern Draw_Frame drawFrame;

// Software renderer implementation data
typedef struct {
    SDL_Window* window;
    SDL_Surface* window_surface;
    SDL_Surface* render_surface;
    uint32_t screen_width;
    uint32_t screen_height;
    bool initialized;
    bool window_created;
} Software_Data;

// Static instance of the implementation data
static Software_Data software_data = {0};

// ================ LOGGING FUNCTIONS ================

static void software_log_verbose(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[SOFTWARE][VERBOSE] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void software_log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[SOFTWARE][INFO] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void software_log_warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[SOFTWARE][WARNING] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void software_log_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[SOFTWARE][ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

// Check SDL errors and log them
static void check_sdl_errors(const char* function) {
    const char* error_str = SDL_GetError();
    if (error_str && *error_str) {
        software_log_error("SDL Error in %s: %s", function, error_str);
        SDL_ClearError();
    }
}

// ================ CORE FUNCTIONS ================

static GAL_Result software_initialize(void) {
    software_log_info("Initializing Software renderer");
    
    if (software_data.initialized) {
        software_log_warning("Software renderer already initialized");
        return GAL_RESULT_SUCCESS;
    }
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        software_log_error("SDL initialization failed: %s", SDL_GetError());
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    software_log_verbose("SDL video subsystem initialized");
    
    software_data.initialized = true;
    return GAL_RESULT_SUCCESS;
}

static void software_shutdown(void) {
    software_log_info("Shutting down Software renderer");
    
    if (!software_data.initialized) {
        software_log_warning("Software renderer not initialized");
        return;
    }
    
    // Free render surface
    if (software_data.render_surface) {
        SDL_FreeSurface(software_data.render_surface);
        software_data.render_surface = NULL;
    }
    
    // Destroy SDL window
    if (software_data.window) {
        SDL_DestroyWindow(software_data.window);
        software_data.window = NULL;
    }
    
    // Quit SDL
    SDL_Quit();
    
    software_data.initialized = false;
    software_log_info("Software renderer shutdown complete");
}

static GAL_Result software_create_window(GAL_Window_Desc* desc) {
    if (!software_data.initialized) {
        software_log_error("Software renderer not initialized");
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    
    if (!desc) {
        software_log_error("Invalid window description");
        return GAL_RESULT_ERROR_INVALID_PARAMETER;
    }
    
    // Set up window flags
    uint32_t flags = SDL_WINDOW_SHOWN;
    if (desc->resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    if (desc->fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    
    // Create the window if needed
    if (!software_data.window) {
        software_data.window = SDL_CreateWindow(
            desc->title ? desc->title : "Oogabooga Engine (Software)",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            desc->width, desc->height,
            flags
        );
        
        if (!software_data.window) {
            software_log_error("Failed to create SDL window: %s", SDL_GetError());
            return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
        }
        
        software_data.window_created = true;
        software_log_verbose("SDL window created: %dx%d", desc->width, desc->height);
    }
    
    // Store window dimensions
    software_data.screen_width = desc->width;
    software_data.screen_height = desc->height;
    
    // Get the window surface
    software_data.window_surface = SDL_GetWindowSurface(software_data.window);
    if (!software_data.window_surface) {
        software_log_error("Failed to get window surface: %s", SDL_GetError());
        if (software_data.window_created) {
            SDL_DestroyWindow(software_data.window);
            software_data.window = NULL;
        }
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    
    // Create a render surface with the same format as the window
    software_data.render_surface = SDL_CreateRGBSurface(
        0, desc->width, desc->height,
        software_data.window_surface->format->BitsPerPixel,
        software_data.window_surface->format->Rmask,
        software_data.window_surface->format->Gmask,
        software_data.window_surface->format->Bmask,
        software_data.window_surface->format->Amask
    );
    
    if (!software_data.render_surface) {
        software_log_error("Failed to create render surface: %s", SDL_GetError());
        if (software_data.window_created) {
            SDL_DestroyWindow(software_data.window);
            software_data.window = NULL;
        }
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    
    software_log_info("Window created: %dx%d", software_data.screen_width, software_data.screen_height);
    return GAL_RESULT_SUCCESS;
}

static void software_destroy_window(void) {
    if (!software_data.window) {
        return;
    }
    
    // Free render surface
    if (software_data.render_surface) {
        SDL_FreeSurface(software_data.render_surface);
        software_data.render_surface = NULL;
    }
    
    // Destroy window
    SDL_DestroyWindow(software_data.window);
    software_data.window = NULL;
    software_data.window_surface = NULL; // Window surface is freed by SDL_DestroyWindow
    
    software_log_verbose("Window destroyed");
}

static void software_update(void) {
    // Process SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            software_log_info("Received quit event");
            // You might want to set a flag or call a quit function here
        }
        // Other event handling can be added here
    }
}

static void software_present(void) {
    if (!software_data.window || !software_data.window_surface || !software_data.render_surface) {
        return;
    }
    
    // Blit the render surface to the window surface
    if (SDL_BlitSurface(software_data.render_surface, NULL, software_data.window_surface, NULL) != 0) {
        software_log_error("Failed to blit surface: %s", SDL_GetError());
        return;
    }
    
    // Update the window
    if (SDL_UpdateWindowSurface(software_data.window) != 0) {
        software_log_error("Failed to update window surface: %s", SDL_GetError());
        return;
    }
}

// ================ RESOURCE MANAGEMENT ================

static GAL_Texture_Handle software_create_texture(GAL_Texture_Desc* desc, Allocator allocator) {
    if (!desc) {
        software_log_error("Invalid texture description");
        return NULL;
    }
    
    // Create a software texture (SDL_Surface)
    uint32_t rmask, gmask, bmask, amask;
    
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
    
    int depth = 32; // Always use 32 bits for consistency
    int pitch = desc->width * 4; // 4 bytes per pixel (RGBA)
    
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
        desc->initial_data ? desc->initial_data : NULL,
        desc->width,
        desc->height,
        depth,
        pitch,
        rmask, gmask, bmask, amask
    );
    
    if (!surface) {
        software_log_error("Failed to create texture surface: %s", SDL_GetError());
        return NULL;
    }
    
    // If we don't have initial data, we need to create a copy
    if (!desc->initial_data) {
        SDL_Surface* temp_surface = SDL_CreateRGBSurface(
            0, desc->width, desc->height, depth,
            rmask, gmask, bmask, amask
        );
        
        if (!temp_surface) {
            software_log_error("Failed to create temporary surface: %s", SDL_GetError());
            SDL_FreeSurface(surface);
            return NULL;
        }
        
        // Fill with black transparent
        SDL_FillRect(temp_surface, NULL, SDL_MapRGBA(temp_surface->format, 0, 0, 0, 0));
        
        // Free the original surface
        SDL_FreeSurface(surface);
        surface = temp_surface;
    }
    
    software_log_verbose("Created texture %dx%d with %d channels", desc->width, desc->height, desc->channels);
    return (GAL_Texture_Handle)surface;
}

static void software_update_texture(GAL_Texture_Handle texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data) {
    if (!texture || !data) {
        return;
    }
    
    SDL_Surface* surface = (SDL_Surface*)texture;
    if (x >= (uint32_t)surface->w || y >= (uint32_t)surface->h) {
        return;
    }
    
    // Create a temporary surface from the provided data
    SDL_Surface* temp_surface = SDL_CreateRGBSurfaceFrom(
        data,
        width, height,
        surface->format->BitsPerPixel,
        width * (surface->format->BitsPerPixel / 8),
        surface->format->Rmask,
        surface->format->Gmask,
        surface->format->Bmask,
        surface->format->Amask
    );
    
    if (!temp_surface) {
        software_log_error("Failed to create temporary surface for update: %s", SDL_GetError());
        return;
    }
    
    // Create destination rectangle
    SDL_Rect dest_rect = { (int)x, (int)y, (int)width, (int)height };
    
    // Blit the temporary surface to our texture
    if (SDL_BlitSurface(temp_surface, NULL, surface, &dest_rect) != 0) {
        software_log_error("Failed to update texture: %s", SDL_GetError());
    }
    
    // Free temporary surface
    SDL_FreeSurface(temp_surface);
}

static void software_destroy_texture(GAL_Texture_Handle texture, Allocator allocator) {
    if (!texture) {
        return;
    }
    
    SDL_Surface* surface = (SDL_Surface*)texture;
    SDL_FreeSurface(surface);
    software_log_verbose("Destroyed texture");
}

// ================ RENDERING FUNCTIONS ================

static void software_begin_frame(void) {
    if (!software_data.render_surface) {
        return;
    }
    
    // Clear the render surface to black
    SDL_FillRect(software_data.render_surface, NULL, 
                SDL_MapRGBA(software_data.render_surface->format, 0, 0, 0, 255));
}

static void software_end_frame(void) {
    // Nothing to do here for software rendering
}

static void software_render_draw_frame(Draw_Frame* frame, GAL_Texture_Handle target) {
    if (!frame || !target) {
        return;
    }
    
    // In a real implementation, this would render the draw frame to the target texture
    // For now, just log that we're rendering
    software_log_verbose("Rendering draw frame to texture target");
}

static void software_render_draw_frame_to_window(Draw_Frame* frame) {
    if (!frame || !software_data.render_surface) {
        return;
    }
    
    // In a real implementation, this would render the draw frame to the window
    // For now, just log that we're rendering
    software_log_verbose("Rendering draw frame to window");
    
    // Simple test pattern to prove rendering is working
    // Draw some colored rectangles
    if (software_data.render_surface) {
        SDL_Rect rect;
        
        // Red rectangle in top-left corner
        rect.x = 10;
        rect.y = 10;
        rect.w = 100;
        rect.h = 100;
        SDL_FillRect(software_data.render_surface, &rect, 
                    SDL_MapRGBA(software_data.render_surface->format, 255, 0, 0, 255));
        
        // Green rectangle in top-right corner
        rect.x = software_data.screen_width - 110;
        rect.y = 10;
        rect.w = 100;
        rect.h = 100;
        SDL_FillRect(software_data.render_surface, &rect, 
                    SDL_MapRGBA(software_data.render_surface->format, 0, 255, 0, 255));
        
        // Blue rectangle in bottom-left corner
        rect.x = 10;
        rect.y = software_data.screen_height - 110;
        rect.w = 100;
        rect.h = 100;
        SDL_FillRect(software_data.render_surface, &rect, 
                    SDL_MapRGBA(software_data.render_surface->format, 0, 0, 255, 255));
        
        // White rectangle in bottom-right corner
        rect.x = software_data.screen_width - 110;
        rect.y = software_data.screen_height - 110;
        rect.w = 100;
        rect.h = 100;
        SDL_FillRect(software_data.render_surface, &rect, 
                    SDL_MapRGBA(software_data.render_surface->format, 255, 255, 255, 255));
    }
}

static void software_clear_render_target(GAL_RenderTarget_Handle target, float r, float g, float b, float a) {
    SDL_Surface* surface = (SDL_Surface*)target;
    if (!surface) {
        return;
    }
    
    // Convert float colors [0.0-1.0] to Uint8 [0-255]
    Uint8 r_byte = (Uint8)(r * 255.0f);
    Uint8 g_byte = (Uint8)(g * 255.0f);
    Uint8 b_byte = (Uint8)(b * 255.0f);
    Uint8 a_byte = (Uint8)(a * 255.0f);
    
    // Clear the surface
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r_byte, g_byte, b_byte, a_byte));
}

// ================ ADVANCED FUNCTIONS ================

static GAL_Result software_compile_shader(string source_code, uint64_t cbuffer_size, void** shader_object) {
    // Software renderer doesn't use shaders
    software_log_warning("Shader compilation not supported in software renderer");
    return GAL_RESULT_ERROR_UNSUPPORTED_FEATURE;
}

static void software_destroy_shader(void* shader_object) {
    // Software renderer doesn't use shaders
}

static void software_reserve_vertex_buffer(uint64_t bytes) {
    // Software renderer doesn't use vertex buffers
    software_log_warning("Vertex buffers not supported in software renderer");
}

// Create the software renderer
GAL_Renderer software_create_renderer(void) {
    GAL_Renderer renderer = {0};
    
    renderer.backend = GAL_BACKEND_SOFTWARE;
    renderer.name = "Software Renderer";
    renderer.api_version = 1;
    
    // Set up logging functions
    renderer.log_verbose = software_log_verbose;
    renderer.log_info = software_log_info;
    renderer.log_warning = software_log_warning;
    renderer.log_error = software_log_error;
    
    // Core functions
    renderer.initialize = software_initialize;
    renderer.shutdown = software_shutdown;
    renderer.create_window = software_create_window;
    renderer.destroy_window = software_destroy_window;
    renderer.update = software_update;
    renderer.present = software_present;
    
    // Resource management
    renderer.create_texture = software_create_texture;
    renderer.update_texture = software_update_texture;
    renderer.destroy_texture = software_destroy_texture;
    
    // Rendering functions
    renderer.begin_frame = software_begin_frame;
    renderer.end_frame = software_end_frame;
    renderer.render_draw_frame = software_render_draw_frame;
    renderer.render_draw_frame_to_window = software_render_draw_frame_to_window;
    renderer.clear_render_target = software_clear_render_target;
    
    // Advanced functions
    renderer.compile_shader = software_compile_shader;
    renderer.destroy_shader = software_destroy_shader;
    renderer.reserve_vertex_buffer = software_reserve_vertex_buffer;
    
    // Implementation data
    renderer.implementation_data = &software_data;
    
    return renderer;
}
