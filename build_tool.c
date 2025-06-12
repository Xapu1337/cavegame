#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Command line options
typedef struct {
    int use_mingw_on_windows;
    int release_build;
    int verbose;
} Build_Options;

void print_usage(const char* program_name) {
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  --useMinGWOnWindows   Use MinGW/GCC instead of MSVC on Windows\n");
    printf("  --release             Build in release mode (default: debug)\n");
    printf("  --verbose             Verbose output\n");
    printf("  --help                Show this help\n");
}

Build_Options parse_args(int argc, char** argv) {
    Build_Options opts = {0};
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--useMinGWOnWindows") == 0) {
            opts.use_mingw_on_windows = 1;
        } else if (strcmp(argv[i], "--release") == 0) {
            opts.release_build = 1;
        } else if (strcmp(argv[i], "--verbose") == 0) {
            opts.verbose = 1;
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
    
    const char *cmd = NULL;
    
    #if IS_WINDOWS
        printf("Building for Windows...\n");

        const char *files = "oogabooga/oogabooga.c app/cube_flip.c";

        if (opts.use_mingw_on_windows) {
            printf("Using MinGW/GCC...\n");

            // MinGW build command with GCC
            cmd = "gcc -g -I. -DENTRY_PROC=Entry "
                  "-DINITIAL_PROGRAM_MEMORY_SIZE=5*1024*1024 "
                  "-DTEMPORARY_STORAGE_SIZE=2*1024*1024 "
                  "-o build/game.exe "
                  "%s "
                  "-std=c11 -Wall -Wextra -Wno-unused-parameter "
                  "-lkernel32 -lgdi32 -luser32 -lruntimeobject -lwinmm "
                  "-ld3d11 -ldxguid -ld3dcompiler -lshlwapi -lole32 "
                  "-lshcore -lavrt -lksuser -ldbghelp";

            static char cmd_buffer[1024];
            snprintf(cmd_buffer, sizeof(cmd_buffer), cmd, files);
            cmd = cmd_buffer;

        } else {
            printf("Using MSVC (clang-cl)...\n");

            // MSVC build command using clang-cl (or cl.exe)
            cmd = "clang-cl /Fe:build/game.exe %s /std:c11 /Zi /Od "
                  "/DENTRY_PROC=Entry "
                  "/DINITIAL_PROGRAM_MEMORY_SIZE=5*1024*1024 "
                  "/DTEMPORARY_STORAGE_SIZE=2*1024*1024 "
                  "/D_CRT_SECURE_NO_WARNINGS "
                  "kernel32.lib gdi32.lib user32.lib runtimeobject.lib winmm.lib "
                  "d3d11.lib dxguid.lib d3dcompiler.lib shlwapi.lib ole32.lib "
                  "shcore.lib avrt.lib ksuser.lib dbghelp.lib";

            static char cmd_buffer[1024];
            snprintf(cmd_buffer, sizeof(cmd_buffer), cmd, files);
            cmd = cmd_buffer;
        }
        
        // Create build directory
        printf("Creating build directory...\n");
        system("if not exist build mkdir build");
        
    #elif IS_LINUX
        printf("Building for Linux...\n");
        printf("Using GCC with Vulkan...\n");
        
        // Linux build command using gcc with Vulkan (NO SDL2)
        const char* optimization = opts.release_build ? "-O3 -DNDEBUG" : "-g -O0";

        const char *files = "oogabooga/oogabooga.c app/cube_flip.c";

        // Note: This is a simplified command. In practice you'd want to check
        // for Vulkan SDK installation and proper linking
        cmd = "gcc -I. "
              "-DENTRY_PROC=Entry "
              "-D_POSIX_C_SOURCE=200809L "
              "-DINITIAL_PROGRAM_MEMORY_SIZE=5*1024*1024 "
              "-DTEMPORARY_STORAGE_SIZE=2*1024*1024 "
              "%s "
              "%s "
              "-std=c11 -Wall -Wextra -Wno-unused-parameter "
              "-lm -lpthread -ldl -lX11 -lvulkan "
              "-o build/game";

        static char cmd_buffer[1024];
        snprintf(cmd_buffer, sizeof(cmd_buffer), cmd, files, optimization);
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
    
    if (result == 0) {
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
                printf("  Renderer: D3D11\n");
            } else {
                printf("  Compiler: MSVC\n");
                printf("  Renderer: D3D11\n");
            }
        #elif IS_LINUX
            printf("  Compiler: GCC\n");
            printf("  Renderer: Vulkan\n");
        #endif
        
    } else {
        printf("Build failed with exit code: %d\n", result);
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
