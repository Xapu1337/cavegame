# Graphics Abstraction Layer (GAL) for Oogabooga Engine

## Overview

The Graphics Abstraction Layer (GAL) provides a unified interface for rendering graphics across multiple backends:

- **OpenGL**: Cross-platform 3D graphics API
- **Vulkan**: Modern, high-performance graphics API
- **Direct3D 11**: Windows-specific graphics API
- **Software Rendering**: CPU-based rendering as lowest-level fallback
- (Future potential for Metal or other APIs)

The GAL architecture ensures that the engine can run on any platform with optimal performance by automatically selecting and initializing the best available graphics backend.

## Architecture

### Core Components

1. **Core Interface (`gal.h`, `gal.c`)**
   - Defines the common API and structures used by all rendering backends
   - Handles backend selection and initialization with fallback chain
   - Provides utility functions for logging and error handling

2. **Backend Implementations**
   - `gal_opengl.c`: OpenGL implementation using SDL2
   - `gal_vulkan.c`: Vulkan implementation for Linux/Windows
   - (Future: Direct3D 11 in `gal_d3d11.c`)
   - `gal_software.c`: Software rendering fallback using CPU

3. **Adapter Layer (`gal_adapter.c`)**
   - Bridges the GAL with the existing engine graphics API
   - Maps the old function calls to the new GAL interface

4. **Utilities (`gal_utils.h`)**
   - Helper functions for error handling, backend detection, etc.

### Initialization Flow

1. The engine requests a specific backend (e.g., OpenGL)
2. GAL attempts to initialize that backend
3. If initialization fails, it tries to detect the best available backend for the platform
4. If that fails too, it tries other backends in order of preference
5. As a last resort, it falls back to software rendering
6. If all backends fail, it uses a minimal fallback renderer that logs errors

## Usage

### Basic Usage

```c
// Initialize the GAL with automatic backend selection
gal_initialize(GAL_BACKEND_AUTO);

// Create a window
GAL_Window_Desc window_desc = {
    .title = "My Game",
    .width = 1280,
    .height = 720,
    .vsync = true
};
gal_get_renderer()->create_window(&window_desc);

// Main loop
while (running) {
    gal_get_renderer()->begin_frame();
    // Render your scene
    gal_get_renderer()->end_frame();
    gal_get_renderer()->present();
}

// Clean up
gal_shutdown();
```

### Resource Management

The GAL handles various graphics resources including:

- **Textures**: 2D images for rendering
- **Render Targets**: Surfaces that can be rendered to
- **Shaders**: Programs that control the rendering pipeline

Each resource is created, updated, and destroyed through the GAL interface.

### Error Handling

The GAL uses result codes to indicate success or failure:

```c
GAL_Result result = gal_initialize(GAL_BACKEND_OPENGL);
if (result != GAL_RESULT_SUCCESS) {
    // Handle error
    const char* error_msg = gal_result_to_string(result);
    printf("Failed to initialize GAL: %s\n", error_msg);
}
```

## Building with GAL

### Using the Build Tool

The build tool supports various options for configuring the build:

```bash
# Basic build with automatic backend selection
./build_tool

# Build with specific backend
./build_tool --opengl
./build_tool --vulkan
./build_tool --d3d11
./build_tool --software

# Interactive build configuration
./build_tool --interactive

# Platform-specific builds
./build_tool --linux --vulkan
./build_tool --windows --d3d11
```

### Command-Line Options

- `--gal`: Use the Graphics Abstraction Layer
- `--opengl`, `--vulkan`, `--d3d11`, `--software`: Select a specific graphics backend
- `--auto`: Auto-detect the best available backend (default)
- `--release`: Build in release mode
- `--verbose`: Enable verbose output
- `--interactive`: Use interactive mode to configure the build

## Adding New Backends

To add a new graphics backend:

1. Create a new implementation file (e.g., `gal_new_backend.c`)
2. Implement all functions required by the GAL_Renderer structure
3. Create a `new_backend_create_renderer()` function
4. Add the new backend to the initialization logic in `gal.c`
5. Update the build system to support the new backend

## Software Renderer

The software renderer provides a CPU-based fallback when hardware acceleration is unavailable. It:

- Uses SDL2 for window creation and pixel operations
- Renders directly to memory buffers
- Provides basic 2D drawing operations
- Serves as the lowest-level fallback to ensure the engine always works

## Edge Cases and Error Handling

The GAL is designed to handle various edge cases:

- **Initialization Failures**: Attempts multiple backends with an intelligent fallback chain
- **Invalid Parameters**: Validates all input parameters before use
- **Resource Management**: Tracks and properly cleans up all allocated resources
- **Platform Compatibility**: Adapts to the capabilities of the target platform
- **Error Reporting**: Provides detailed error information for debugging

## Benefits

- **Portability**: Run on any platform with minimal code changes
- **Performance**: Use the most efficient API available on each platform
- **Maintainability**: Changes to rendering code only need to be made in one place
- **Extensibility**: Easy to add support for new graphics APIs
- **Debugging**: Common logging interface across all backends
- **Robustness**: Graceful degradation to software rendering when needed
