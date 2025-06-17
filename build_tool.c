#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef _WIN32
    #define IS_WINDOWS 1
    #define IS_LINUX 0
#elif defined(__linux__)
    #define IS_WINDOWS 0 
    #define IS_LINUX 1
#else
    #define IS_WINDOWS 0
    #define IS_LINUX 0
    #error "Unsupported platform"
#endif

// Available graphics backends
typedef enum {
    BACKEND_NONE = 0,
    BACKEND_OPENGL,
    BACKEND_VULKAN,
    BACKEND_D3D11,
    BACKEND_SOFTWARE,
    BACKEND_AUTO // Auto-detect based on platform
} Graphics_Backend;

// Command line options
typedef struct {
    int use_mingw_on_windows;
    int release_build;
    int verbose;
    Graphics_Backend graphics_backend;
    int interactive; // Interactive mode for build options
} Build_Options;

void print_usage(const char* program_name) {
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  --useMinGWOnWindows   Use MinGW/GCC instead of MSVC on Windows\n");
    printf("  --release             Build in release mode (default: debug)\n");
    printf("  --verbose             Verbose output\n");
    printf("  --opengl              Use OpenGL backend\n");
    printf("  --vulkan              Use Vulkan backend\n");
    printf("  --d3d11               Use Direct3D 11 backend (Windows only)\n");
    printf("  --software            Use Software rendering backend\n");
    printf("  --auto                Auto-detect best graphics backend (default)\n");
    printf("  --interactive         Interactive mode for build options\n");
    printf("  --help                Show this help\n");
    printf("\nExamples:\n");
    printf("  %s --vulkan           Build with Vulkan backend\n", program_name);
    printf("  %s --opengl           Build with OpenGL backend\n", program_name);
    printf("\nNote: GAL (Graphics Abstraction Layer) is now the standard rendering system.\n");
}

// Get input from user with a prompt
char get_user_choice(const char* prompt, const char* options) {
    printf("%s [%s]: ", prompt, options);
    fflush(stdout);
    
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin)) {
        if (buffer[0] != '\n') {
            return buffer[0];
        }
        // Return the first option if user just pressed enter
        return options[0];
    }
    return options[0];
}

// Prompt user for build options interactively
void get_interactive_options(Build_Options* opts) {
    printf("\nInteractive Build Configuration\n");
    printf("==============================\n");
    
    // Platform selection
    printf("Current platform: %s\n", IS_WINDOWS ? "Windows" : (IS_LINUX ? "Linux" : "Unknown"));
    
    // Compiler selection on Windows
    if (IS_WINDOWS) {
        char choice = get_user_choice("Select compiler", "M/m=MSVC, G/g=MinGW/GCC");
        opts->use_mingw_on_windows = (choice == 'G' || choice == 'g');
        printf("Using %s compiler\n", opts->use_mingw_on_windows ? "MinGW/GCC" : "MSVC");
    }
    
    // Build type
    char choice = get_user_choice("Build type", "D/d=Debug, R/r=Release");
    opts->release_build = (choice == 'R' || choice == 'r');
    printf("Building in %s mode\n", opts->release_build ? "release" : "debug");
    
    // Graphics API selection
    printf("\nGraphics API selection:\n");
    printf("1. Auto-detect best available\n");
    printf("2. OpenGL\n");
    printf("3. Vulkan");
    if (IS_WINDOWS) {
        printf("\n4. Direct3D 11");
    }
    printf("\n5. Software (CPU) renderer\n");
    
    choice = get_user_choice("Select graphics backend", "1-5");
    switch (choice) {
        case '2': opts->graphics_backend = BACKEND_OPENGL; break;
        case '3': opts->graphics_backend = BACKEND_VULKAN; break;
        case '4': opts->graphics_backend = IS_WINDOWS ? BACKEND_D3D11 : BACKEND_AUTO; break;
        case '5': opts->graphics_backend = BACKEND_SOFTWARE; break;
        case '1':
        default:  opts->graphics_backend = BACKEND_AUTO; break;
    }
    
    // Verbose output
    choice = get_user_choice("Enable verbose output", "Y/y=Yes, N/n=No");
    opts->verbose = (choice == 'Y' || choice == 'y');
    
    printf("\nConfiguration complete!\n\n");
}

Build_Options parse_args(int argc, char** argv) {
    Build_Options opts = {0};
    
    // Default to auto-detect backend
    opts.graphics_backend = BACKEND_AUTO;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--useMinGWOnWindows") == 0) {
            opts.use_mingw_on_windows = 1;
        } else if (strcmp(argv[i], "--release") == 0) {
            opts.release_build = 1;
        } else if (strcmp(argv[i], "--verbose") == 0) {
            opts.verbose = 1;
        } else if (strcmp(argv[i], "--opengl") == 0) {
            opts.graphics_backend = BACKEND_OPENGL;
        } else if (strcmp(argv[i], "--vulkan") == 0) {
            opts.graphics_backend = BACKEND_VULKAN;
        } else if (strcmp(argv[i], "--d3d11") == 0) {
            opts.graphics_backend = BACKEND_D3D11;
        } else if (strcmp(argv[i], "--software") == 0) {
            opts.graphics_backend = BACKEND_SOFTWARE;
        } else if (strcmp(argv[i], "--auto") == 0) {
            opts.graphics_backend = BACKEND_AUTO;
        } else if (strcmp(argv[i], "--interactive") == 0) {
            opts.interactive = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            exit(0);
        } else {
            printf("Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            exit(1);
        }
    }
    
    return opts;
}

int main(int argc, char **argv) {
    Build_Options opts = parse_args(argc, argv);
    
    printf("Oogabooga Build Tool\n");
    printf("====================\n");
    
    // If interactive mode is enabled, prompt for options
    if (opts.interactive) {
        get_interactive_options(&opts);
    }
    
    // Auto-detect backend if not specified
    if (opts.graphics_backend == BACKEND_AUTO) {
        #if IS_WINDOWS
            opts.graphics_backend = BACKEND_D3D11; // Prefer D3D11 on Windows
        #else
            // Check if Vulkan is available, otherwise use OpenGL, fall back to Software
            int have_vulkan_headers = access("/usr/include/vulkan/vulkan.h", F_OK) == 0;
            int have_gl_headers = access("/usr/include/GL/gl.h", F_OK) == 0;
            
            if (have_vulkan_headers) {
                opts.graphics_backend = BACKEND_VULKAN;
            } else if (have_gl_headers) {
                opts.graphics_backend = BACKEND_OPENGL;
            } else {
                opts.graphics_backend = BACKEND_SOFTWARE;
            }
        #endif
    }
    
    // Validate backend selection
    #if IS_LINUX
        if (opts.graphics_backend == BACKEND_D3D11) {
            printf("Warning: Direct3D 11 is not available on Linux. Falling back to OpenGL.\n");
            opts.graphics_backend = BACKEND_OPENGL;
        }
    #endif
    
    const char *cmd = NULL;
    const char *backend_name = NULL;
    const char *backend_flags = NULL;
    
    // Set backend name and flags
    switch (opts.graphics_backend) {
        case BACKEND_OPENGL:
            backend_name = "OpenGL";
            backend_flags = "-DRENDERER_OPENGL=1 -DRENDERER_VULKAN=0 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=0";
            #if IS_WINDOWS
                backend_flags = "-DRENDERER_OPENGL=1 -DRENDERER_VULKAN=0 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=0 -lopengl32 -lglu32";
            #else
                backend_flags = "-DRENDERER_OPENGL=1 -DRENDERER_VULKAN=0 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=0 -lGL -lGLU";
            #endif
            break;
            
        case BACKEND_VULKAN:
            backend_name = "Vulkan";
            #if IS_WINDOWS
                backend_flags = "-DRENDERER_OPENGL=0 -DRENDERER_VULKAN=1 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=0 -lvulkan";
            #else
                backend_flags = "-DRENDERER_OPENGL=0 -DRENDERER_VULKAN=1 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=0 -lvulkan";
            #endif
            break;
            
        case BACKEND_D3D11:
            backend_name = "Direct3D 11";
            #if IS_WINDOWS
                backend_flags = "-DRENDERER_OPENGL=0 -DRENDERER_VULKAN=0 -DRENDERER_D3D11=1 -DRENDERER_SOFTWARE=0 -ld3d11 -ld3dcompiler -ldxgi";
            #else
                printf("Warning: Direct3D 11 is not available on Linux. Falling back to OpenGL.\n");
                backend_name = "OpenGL";
                backend_flags = "-DRENDERER_OPENGL=1 -DRENDERER_VULKAN=0 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=0 -lGL -lGLU";
                opts.graphics_backend = BACKEND_OPENGL;
            #endif
            break;
            
        case BACKEND_SOFTWARE:
            backend_name = "Software";
            backend_flags = "-DRENDERER_OPENGL=0 -DRENDERER_VULKAN=0 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=1";
            break;
            
        default:
            backend_name = "Unknown";
            backend_flags = "-DRENDERER_OPENGL=0 -DRENDERER_VULKAN=0 -DRENDERER_D3D11=0 -DRENDERER_SOFTWARE=1";
            opts.graphics_backend = BACKEND_SOFTWARE;
            break;
    }
    
    // GAL files are already included in oogabooga.c via #include directives
    // No need to compile them separately - this avoids multiple definition errors
    const char *gal_source_files = "";
    
    // Add SDL2 dependency for GAL
    char* extended_backend_flags = malloc(strlen(backend_flags) + 100);
    sprintf(extended_backend_flags, "%s -lSDL2", backend_flags);
    backend_flags = extended_backend_flags;
    
    // Build command based on platform
    #if IS_WINDOWS
        printf("Building for Windows with %s backend...\n", backend_name);

        const char *files = "oogabooga/oogabooga.c app/cube_flip.c";
        char full_files[512];
        sprintf(full_files, "%s%s", files, gal_source_files);

        if (opts.use_mingw_on_windows) {
            printf("Using MinGW/GCC...\n");

            // MinGW build command with GCC
            cmd = "gcc %s -I. -DENTRY_PROC=Entry "
                 "-DINITIAL_PROGRAM_MEMORY_SIZE=512*1024*1024 "
                  "-DTEMPORARY_STORAGE_SIZE=2*1024*1024 "
                  "-o build/game.exe "
                  "%s "
                  "-std=c11 -Wall -Wextra -Wno-unused-parameter "
                  "%s "
                  "-lkernel32 -lgdi32 -luser32 -lruntimeobject -lwinmm "
                  "-lshlwapi -lole32 -lshcore -lavrt -lksuser -ldbghelp";

            const char* optimization = opts.release_build ? "-O3 -DNDEBUG" : "-g -O0";
            static char cmd_buffer[2048];
            snprintf(cmd_buffer, sizeof(cmd_buffer), cmd, optimization, full_files, backend_flags);
            cmd = cmd_buffer;

        } else {
            printf("Using MSVC (clang-cl)...\n");

            // MSVC build command using clang-cl (or cl.exe)
            cmd = "clang-cl /Fe:build/game.exe %s %s "
                  "/DENTRY_PROC=Entry "
                  "/DINITIAL_PROGRAM_MEMORY_SIZE=512*1024*1024 "
                  "/DTEMPORARY_STORAGE_SIZE=2*1024*1024 "
                  "/D_CRT_SECURE_NO_WARNINGS "
                  "%s "
                  "kernel32.lib gdi32.lib user32.lib runtimeobject.lib winmm.lib "
                  "d3d11.lib dxguid.lib d3dcompiler.lib shlwapi.lib ole32.lib "
                  "shcore.lib avrt.lib ksuser.lib dbghelp.lib";

            const char* optimization = opts.release_build ? "/O2 /DNDEBUG" : "/Zi /Od";
            static char cmd_buffer[2048];
            snprintf(cmd_buffer, sizeof(cmd_buffer), cmd, full_files, optimization, backend_flags);
            cmd = cmd_buffer;
        }
        
        // Create build directory
        printf("Creating build directory...\n");
        system("if not exist build mkdir build");
        
    #elif IS_LINUX
        printf("Building for Linux with %s backend...\n", backend_name);
        
        // Check for necessary dependencies - SDL2 is always needed for GAL
        int sdl2_available = system("pkg-config --exists sdl2") == 0;
        
        if (!sdl2_available) {
            printf("Warning: SDL2 development libraries not found, but required for GAL.\n");
            printf("Installing SDL2 development libraries...\n");
            system("sudo apt-get update && sudo apt-get install -y libsdl2-dev");
        }
        
        // Linux build command using gcc
        const char* optimization = opts.release_build ? "-O3 -DNDEBUG" : "-g -O0";
        const char *files = "oogabooga/oogabooga.c app/cube_flip.c";
        
        char full_files[512];
        sprintf(full_files, "%s%s", files, gal_source_files);

        // Build the command - SDL2 is always required for GAL
        const char* sdl_flags = "`pkg-config --cflags --libs sdl2`";

        cmd = "gcc -I. "
              "%s "  // Optimization flags
              "-DENTRY_PROC=Entry "
              "-D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE "
              "-DINITIAL_PROGRAM_MEMORY_SIZE=512*1024*1024 "
              "-DTEMPORARY_STORAGE_SIZE=2*1024*1024 "
              "%s "  // Source files
              "-std=c11 -Wall -Wextra -Wno-unused-parameter "
              "-lm -lpthread -ldl -lX11 "
              "%s "  // Backend flags
              "%s "  // SDL flags
              "-o build/game";

        static char cmd_buffer[2048];
        snprintf(cmd_buffer, sizeof(cmd_buffer), cmd, optimization, full_files, backend_flags, sdl_flags);
        cmd = cmd_buffer;
        
        // Create build directory
        printf("Creating build directory...\n");
        system("mkdir -p build");
        
    #endif
    
    if (!cmd) {
        printf("Failed to determine build command\n");
        return 1;
    }
    
    if (opts.verbose) {
        printf("Executing: %s\n", cmd);
    }
    
    printf("Compiling...\n");
    int result = system(cmd);
    int exit_code = -1;
    if (result != -1) {
        exit_code = WEXITSTATUS(result);
    }

    if (exit_code == 0) {
        printf("Build successful!\n");
        printf("Executable: ");
        #if IS_WINDOWS
            printf("build/game.exe\n");
        #elif IS_LINUX
            printf("build/game\n");
        #endif
        
        printf("\nPlatform configuration:\n");
        #if IS_WINDOWS
            if (opts.use_mingw_on_windows) {
                printf("  Compiler: GCC (MinGW)\n");
            } else {
                printf("  Compiler: MSVC\n");
            }
        #elif IS_LINUX
            printf("  Compiler: GCC\n");
        #endif
        
        printf("  Renderer: %s\n", backend_name);
        
    } else {
        printf("Build failed with exit code: %d\n", exit_code);
        printf("\nPossible issues:\n");
        #if IS_WINDOWS
            if (!opts.use_mingw_on_windows) {
                printf("  - MSVC/Visual Studio not installed or not in PATH\n");
                printf("  - Try using --useMinGWOnWindows flag\n");
            } else {
                printf("  - MinGW/GCC not installed or not in PATH\n");
            }
            printf("  - DirectX SDK/Windows SDK not found\n");
        #elif IS_LINUX
            printf("  - GCC not installed\n");
            printf("  - Vulkan SDK/drivers not installed\n");
            printf("  - Missing system libraries (libX11-dev, libvulkan-dev)\n");
            printf("  - Try: sudo apt install build-essential libx11-dev libvulkan-dev vulkan-tools\n");
        #endif
        return 1;
    }
    
    return 0;
}
