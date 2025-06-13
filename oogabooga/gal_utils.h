#ifndef OOGABOOGA_GAL_UTILS_H
#define OOGABOOGA_GAL_UTILS_H

/**
 * Graphics Abstraction Layer (GAL) Utilities
 * 
 * This header provides utility functions that are useful across different GAL implementations.
 */

#include "gal.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// Convert a GAL_Result code to a string
static const char* gal_result_to_string(GAL_Result result) {
    switch (result) {
        case GAL_RESULT_SUCCESS: return "Success";
        case GAL_RESULT_ERROR_INITIALIZATION_FAILED: return "Initialization failed";
        case GAL_RESULT_ERROR_INVALID_PARAMETER: return "Invalid parameter";
        case GAL_RESULT_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case GAL_RESULT_ERROR_NOT_IMPLEMENTED: return "Not implemented";
        case GAL_RESULT_ERROR_UNSUPPORTED_FEATURE: return "Unsupported feature";
        case GAL_RESULT_ERROR_DEVICE_LOST: return "Device lost";
        case GAL_RESULT_ERROR_FORMAT_NOT_SUPPORTED: return "Format not supported";
        case GAL_RESULT_ERROR_UNKNOWN: 
        default: return "Unknown error";
    }
}

// Convert a GAL_Backend enum to a string
static const char* gal_backend_to_string(GAL_Backend backend) {
    switch (backend) {
        case GAL_BACKEND_OPENGL: return "OpenGL";
        case GAL_BACKEND_VULKAN: return "Vulkan";
        case GAL_BACKEND_D3D11: return "Direct3D 11";
        case GAL_BACKEND_SOFTWARE: return "Software";
        case GAL_BACKEND_METAL: return "Metal";
        case GAL_BACKEND_UNKNOWN:
        default: return "Unknown";
    }
}

// Check if the system supports a specific graphics backend
static bool gal_is_backend_supported(GAL_Backend backend) {
    switch (backend) {
        case GAL_BACKEND_OPENGL:
            #if defined(RENDERER_OPENGL) && RENDERER_OPENGL
                return true;
            #else
                return false;
            #endif
            
        case GAL_BACKEND_VULKAN:
            #if defined(PLATFORM_LINUX) && defined(RENDERER_VULKAN) && PLATFORM_LINUX && RENDERER_VULKAN
                return true;
            #else
                return false;
            #endif
            
        case GAL_BACKEND_D3D11:
            #if defined(_WIN32) && defined(RENDERER_D3D11) && RENDERER_D3D11
                return true;
            #else
                return false;
            #endif
            
        case GAL_BACKEND_SOFTWARE:
            return true; // Software renderer should always be available
            
        case GAL_BACKEND_METAL:
            #if defined(__APPLE__) && defined(RENDERER_METAL) && RENDERER_METAL
                return true;
            #else
                return false;
            #endif
            
        default:
            return false;
    }
}

// Get the best available backend for the current platform
static GAL_Backend gal_get_best_backend(void) {
    #if defined(_WIN32)
        // On Windows, prefer D3D11, then OpenGL, then Software
        if (gal_is_backend_supported(GAL_BACKEND_D3D11)) {
            return GAL_BACKEND_D3D11;
        } else if (gal_is_backend_supported(GAL_BACKEND_OPENGL)) {
            return GAL_BACKEND_OPENGL;
        }
    #elif defined(__APPLE__)
        // On macOS, prefer Metal, then OpenGL, then Software
        if (gal_is_backend_supported(GAL_BACKEND_METAL)) {
            return GAL_BACKEND_METAL;
        } else if (gal_is_backend_supported(GAL_BACKEND_OPENGL)) {
            return GAL_BACKEND_OPENGL;
        }
    #else
        // On other platforms (Linux), prefer Vulkan, then OpenGL, then Software
        if (gal_is_backend_supported(GAL_BACKEND_VULKAN)) {
            return GAL_BACKEND_VULKAN;
        } else if (gal_is_backend_supported(GAL_BACKEND_OPENGL)) {
            return GAL_BACKEND_OPENGL;
        }
    #endif
    
    // Software is the final fallback
    return GAL_BACKEND_SOFTWARE;
}

// Memory safety check for texture operations
static bool gal_validate_texture(GAL_Texture_Handle texture) {
    if (!texture) {
        return false;
    }
    
    // Additional validation could be performed here if needed
    return true;
}

// Memory safety check for allocator
static bool gal_validate_allocator(Allocator allocator) {
    // Check if the allocator is valid (has proc function)
    if (!allocator.proc) {
        return false;
    }
    
    return true;
}

#endif // OOGABOOGA_GAL_UTILS_H
