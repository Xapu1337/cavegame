#ifndef GAL_OPENGL_IMPLEMENTATION_INCLUDED
#define GAL_OPENGL_IMPLEMENTATION_INCLUDED

#include "gal.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

// Forward reference to the draw frame
typedef struct Draw_Frame Draw_Frame;
extern Draw_Frame drawFrame;

// OpenGL implementation data
typedef struct {
    SDL_Window* window;
    SDL_GLContext gl_context;
    uint32_t screen_width;
    uint32_t screen_height;
    bool vsync_enabled;
    bool initialized;
} OpenGL_Data;

// Static instance of the implementation data
static OpenGL_Data opengl_data = {0};

// ================ LOGGING FUNCTIONS ================

static void opengl_log_verbose(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[OPENGL][VERBOSE] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void opengl_log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[OPENGL][INFO] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void opengl_log_warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[OPENGL][WARNING] ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

static void opengl_log_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[OPENGL][ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

// Check SDL errors and log them
static void check_sdl_errors(const char* function) {
    const char* error_str = SDL_GetError();
    if (error_str && *error_str) {
        opengl_log_error("SDL Error in %s: %s", function, error_str);
        SDL_ClearError();
    }
}

// ================ CORE FUNCTIONS ================

static GAL_Result opengl_initialize(void) {
    opengl_log_info("Initializing OpenGL renderer with verbose logging");
    
    if (opengl_data.initialized) {
        opengl_log_warning("OpenGL renderer already initialized");
        return GAL_RESULT_SUCCESS;
    }
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        opengl_log_error("SDL initialization failed: %s", SDL_GetError());
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    opengl_log_verbose("SDL video subsystem initialized");
    
    opengl_data.initialized = true;
    return GAL_RESULT_SUCCESS;
}

static void opengl_shutdown(void) {
    opengl_log_info("Shutting down OpenGL renderer");
    
    if (!opengl_data.initialized) {
        opengl_log_warning("OpenGL renderer not initialized");
        return;
    }
    
    // Destroy OpenGL context
    if (opengl_data.gl_context) {
        SDL_GL_DeleteContext(opengl_data.gl_context);
        opengl_data.gl_context = NULL;
    }
    
    // Destroy SDL window if we created it
    if (opengl_data.window) {
        SDL_DestroyWindow(opengl_data.window);
        opengl_data.window = NULL;
    }
    
    // Quit SDL
    SDL_Quit();
    
    opengl_data.initialized = false;
    opengl_log_info("OpenGL renderer shutdown complete");
}

static GAL_Result opengl_create_window(GAL_Window_Desc* desc) {
    if (!opengl_data.initialized) {
        opengl_log_error("OpenGL renderer not initialized");
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    
    if (!desc) {
        opengl_log_error("Invalid window description");
        return GAL_RESULT_ERROR_INVALID_PARAMETER;
    }
    
    // Set GL attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    check_sdl_errors("SDL_GL_SetAttribute");
    opengl_log_verbose("SDL GL attributes set");
    
    // Set up window flags
    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (desc->resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    if (desc->fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    
    // If we have an existing window, use that
    if (desc->existing_window) {
        opengl_data.window = (SDL_Window*)desc->existing_window;
        opengl_log_verbose("Using existing SDL window");
    } else {
        // Otherwise create a new window
        opengl_data.window = SDL_CreateWindow(
            desc->title ? desc->title : "Oogabooga Engine",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            desc->width, desc->height,
            flags
        );
        
        if (!opengl_data.window) {
            opengl_log_error("Failed to create SDL window: %s", SDL_GetError());
            return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
        }
        opengl_log_verbose("SDL window created: %dx%d", desc->width, desc->height);
    }
    
    // Store window dimensions
    opengl_data.screen_width = desc->width;
    opengl_data.screen_height = desc->height;
    
    // Create OpenGL context
    opengl_data.gl_context = SDL_GL_CreateContext(opengl_data.window);
    if (!opengl_data.gl_context) {
        opengl_log_error("Failed to create OpenGL context: %s", SDL_GetError());
        if (!desc->existing_window) {
            SDL_DestroyWindow(opengl_data.window);
            opengl_data.window = NULL;
        }
        return GAL_RESULT_ERROR_INITIALIZATION_FAILED;
    }
    opengl_log_verbose("OpenGL context created");
    
    // Set vsync
    opengl_data.vsync_enabled = desc->vsync;
    if (desc->vsync && SDL_GL_SetSwapInterval(1) != 0) {
        opengl_log_warning("Failed to enable vsync: %s", SDL_GetError());
        opengl_data.vsync_enabled = false;
    } else if (desc->vsync) {
        opengl_log_verbose("VSync enabled");
    }
    
    // Print OpenGL information
    opengl_log_info("Window created: %dx%d", opengl_data.screen_width, opengl_data.screen_height);
    opengl_log_info("OpenGL Version: %s", glGetString(GL_VERSION));
    opengl_log_info("OpenGL Vendor: %s", glGetString(GL_VENDOR));
    opengl_log_info("OpenGL Renderer: %s", glGetString(GL_RENDERER));
    
    return GAL_RESULT_SUCCESS;
}

static void opengl_destroy_window(void) {
    opengl_log_info("Destroying window");
    
    if (!opengl_data.initialized) {
        opengl_log_warning("OpenGL renderer not initialized");
        return;
    }
    
    // Destroy OpenGL context
    if (opengl_data.gl_context) {
        SDL_GL_DeleteContext(opengl_data.gl_context);
        opengl_data.gl_context = NULL;
    }
    
    // Destroy SDL window
    if (opengl_data.window) {
        SDL_DestroyWindow(opengl_data.window);
        opengl_data.window = NULL;
    }
    
    opengl_log_info("Window destroyed");
}

static void opengl_update(void) {
    if (!opengl_data.initialized || !opengl_data.window || !opengl_data.gl_context) {
        opengl_log_error("OpenGL renderer not properly initialized");
        return;
    }
    
    opengl_log_verbose("Updating");
    
    // Process SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                opengl_data.screen_width = event.window.data1;
                opengl_data.screen_height = event.window.data2;
                glViewport(0, 0, opengl_data.screen_width, opengl_data.screen_height);
                opengl_log_verbose("Window resized to %dx%d", opengl_data.screen_width, opengl_data.screen_height);
            }
        } else if (event.type == SDL_QUIT) {
            opengl_log_info("Quit event received");
            // You might want to handle this differently depending on your application structure
        }
    }
}

static void opengl_present(void) {
    if (!opengl_data.initialized || !opengl_data.window || !opengl_data.gl_context) {
        opengl_log_error("OpenGL renderer not properly initialized");
        return;
    }
    
    opengl_log_verbose("Presenting frame");
    SDL_GL_SwapWindow(opengl_data.window);
}

// ================ RESOURCE MANAGEMENT ================

static GAL_Texture_Handle opengl_create_texture(GAL_Texture_Desc* desc, Allocator allocator) {
    if (!opengl_data.initialized) {
        opengl_log_error("OpenGL renderer not initialized");
        return NULL;
    }
    
    if (!desc) {
        opengl_log_error("Invalid parameters");
        return NULL;
    }
    
    opengl_log_verbose("Creating texture: %dx%d, %d channels, render target: %s",
                      desc->width, desc->height, desc->channels, desc->is_render_target ? "yes" : "no");
    
    // For now, just allocate memory for software fallback
    // In a real implementation, we'd create OpenGL textures and framebuffers
    size_t size = (size_t)desc->width * desc->height * desc->channels;
    void* buffer = Alloc(allocator, size);
    
    if (!buffer) {
        opengl_log_error("Failed to allocate texture memory");
        return NULL;
    }
    
    // Initialize with initial data if provided
    if (desc->initial_data) {
        memcpy(buffer, desc->initial_data, size);
    } else {
        memset(buffer, 0, size);
    }
    
    opengl_log_verbose("Texture created with software fallback");
    return buffer;
}

static void opengl_update_texture(GAL_Texture_Handle texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data) {
    if (!opengl_data.initialized || !texture || !data) {
        opengl_log_error("Invalid parameters");
        return;
    }
    
    opengl_log_verbose("Updating texture at (%d,%d) with size %dx%d", x, y, width, height);
    
    // In a real implementation, we'd update the OpenGL texture
    opengl_log_warning("Texture update not fully implemented");
}

static void opengl_destroy_texture(GAL_Texture_Handle texture, Allocator allocator) {
    if (!opengl_data.initialized || !texture) {
        opengl_log_error("Invalid parameters");
        return;
    }
    
    opengl_log_verbose("Destroying texture");
    
    // Free the software buffer
    Dealloc(allocator, texture);
    opengl_log_verbose("Texture destroyed");
}

// ================ RENDERING FUNCTIONS ================

static void opengl_begin_frame(void) {
    if (!opengl_data.initialized || !opengl_data.window || !opengl_data.gl_context) {
        opengl_log_error("OpenGL renderer not properly initialized");
        return;
    }
    
    opengl_log_verbose("Beginning frame");
    
    // Clear the screen (dark gray)
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void opengl_end_frame(void) {
    if (!opengl_data.initialized || !opengl_data.window || !opengl_data.gl_context) {
        opengl_log_error("OpenGL renderer not properly initialized");
        return;
    }
    
    opengl_log_verbose("Ending frame");
    
    // No additional work needed here, presenting is done in opengl_present
}

static void opengl_render_draw_frame(Draw_Frame* frame, GAL_Texture_Handle target) {
    if (!opengl_data.initialized || !frame) {
        opengl_log_error("Invalid parameters");
        return;
    }
    
    opengl_log_verbose("Rendering draw frame to texture target");
    
    // Not implemented yet
    opengl_log_warning("Rendering to texture not implemented yet");
}

static void opengl_render_draw_frame_to_window(Draw_Frame* frame) {
    if (!opengl_data.initialized || !frame || !opengl_data.window || !opengl_data.gl_context) {
        opengl_log_error("Invalid parameters or renderer not properly initialized");
        return;
    }
    
    opengl_log_verbose("Rendering draw frame to window");
    
    // Very simple rendering - just for demonstration
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, opengl_data.screen_width, opengl_data.screen_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Draw a simple triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex2f((float)opengl_data.screen_width/2.0f, 100.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(100.0f, (float)opengl_data.screen_height-100.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex2f((float)opengl_data.screen_width-100.0f, (float)opengl_data.screen_height-100.0f);
    glEnd();
    opengl_log_verbose("Triangle drawn");
}

static void opengl_clear_render_target(GAL_RenderTarget_Handle target, float r, float g, float b, float a) {
    if (!opengl_data.initialized) {
        opengl_log_error("OpenGL renderer not initialized");
        return;
    }
    
    opengl_log_verbose("Clearing render target");
    
    // If the target is NULL, we clear the default framebuffer (window)
    if (!target) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }
    
    // Otherwise, we need to bind the framebuffer and then clear it
    // Not implemented yet
    opengl_log_warning("Clearing specific render target not implemented yet");
}

// ================ ADVANCED FUNCTIONS ================

static GAL_Result opengl_compile_shader(string source_code, uint64_t cbuffer_size, void** shader_object) {
    if (!opengl_data.initialized || !source_code.data || !shader_object) {
        opengl_log_error("Invalid parameters");
        return GAL_RESULT_ERROR_INVALID_PARAMETER;
    }
    
    opengl_log_verbose("Compiling shader, buffer size: %llu", cbuffer_size);
    opengl_log_warning("Shader compilation not implemented yet");
    
    // Not implemented
    *shader_object = NULL;
    return GAL_RESULT_ERROR_NOT_IMPLEMENTED;
}

static void opengl_destroy_shader(void* shader_object) {
    if (!opengl_data.initialized || !shader_object) {
        opengl_log_error("Invalid parameters");
        return;
    }
    
    opengl_log_verbose("Destroying shader");
    opengl_log_warning("Shader destruction not implemented yet");
    
    // Not implemented
}

static void opengl_reserve_vertex_buffer(uint64_t bytes) {
    if (!opengl_data.initialized) {
        opengl_log_error("OpenGL renderer not initialized");
        return;
    }
    
    opengl_log_verbose("Reserving %llu bytes for vertex buffer", bytes);
    opengl_log_warning("Vertex buffer reservation not implemented yet");
    
    // Not implemented
}

// Create and return the OpenGL renderer implementation
GAL_Renderer opengl_create_renderer(void) {
    GAL_Renderer renderer = {0};
    
    renderer.backend = GAL_BACKEND_OPENGL;
    renderer.name = "OpenGL";
    renderer.api_version = 330; // OpenGL 3.3
    
    // Set up logging functions
    renderer.log_verbose = opengl_log_verbose;
    renderer.log_info = opengl_log_info;
    renderer.log_warning = opengl_log_warning;
    renderer.log_error = opengl_log_error;
    
    // Set up core functions
    renderer.initialize = opengl_initialize;
    renderer.shutdown = opengl_shutdown;
    renderer.create_window = opengl_create_window;
    renderer.destroy_window = opengl_destroy_window;
    renderer.update = opengl_update;
    renderer.present = opengl_present;
    
    // Set up resource management functions
    renderer.create_texture = opengl_create_texture;
    renderer.update_texture = opengl_update_texture;
    renderer.destroy_texture = opengl_destroy_texture;
    
    // Set up rendering functions
    renderer.begin_frame = opengl_begin_frame;
    renderer.end_frame = opengl_end_frame;
    renderer.render_draw_frame = opengl_render_draw_frame;
    renderer.render_draw_frame_to_window = opengl_render_draw_frame_to_window;
    renderer.clear_render_target = opengl_clear_render_target;
    
    // Set up advanced functions
    renderer.compile_shader = opengl_compile_shader;
    renderer.destroy_shader = opengl_destroy_shader;
    renderer.reserve_vertex_buffer = opengl_reserve_vertex_buffer;
    
    // Set up implementation data
    renderer.implementation_data = &opengl_data;
    
    return renderer;
}

#endif // GAL_OPENGL_IMPLEMENTATION_INCLUDED
