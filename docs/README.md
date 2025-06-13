
# Oogabooga Engine

## TOC
- [What is Oogabooga?](#what-is-oogabooga)
	- [Professional C Development](#professional-c-development)
	- [Cross-Platform Support](#cross-platform-support)
	- [Unity Build System](#unity-build-system)
	- [Header Files for IDE Support](#header-files-for-ide-support)
- [Course: From Scratch to Steam](#course-from-scratch-to-steam)
- [Quickstart](#quickstart)
- [Platform Support](#platform-support)
- [Examples & Documentation](#examples--documentation)
- [Known bugs](#known-bugs)
- [Licensing](#licensing)
- [Contributions](#contributions)

## What is Oogabooga?

Oogabooga is a professional C game engine designed for real-world game development. It provides a comprehensive game development framework that follows proper C practices while maintaining high performance and developer productivity.

### Professional C Development

Oogabooga follows industry-standard C practices:

1. **Proper Header/Source Structure**  
   Every `.c` file has a corresponding `.h` file with complete function declarations and type definitions. This enables IDE features like symbol navigation, autocomplete, and error checking.

2. **Platform-Aware Architecture**  
   The engine is designed with clear platform abstractions, allowing the same game code to run on multiple platforms without modification.

3. **Modern Build System**  
   While using Unity builds for performance, the engine supports both traditional build systems and modern IDEs through proper header organization.

4. **Type Safety and Documentation**  
   All public APIs are properly declared in headers with comprehensive documentation for professional development workflows.

### Cross-Platform Support

Oogabooga supports the following platforms:

- **Windows**: MSVC (default) or MinGW/GCC with D3D11 renderer
- **Linux**: GCC with Vulkan renderer (no SDL2 dependency)
- **macOS**: Planned (Metal renderer)

The engine automatically detects the platform and selects the appropriate:
- Compiler toolchain (MSVC/GCC/Clang)
- Graphics API (D3D11/Vulkan/Metal)
- OS abstractions (Windows API/POSIX)

### Unity Build System

For maximum compile-time performance, Oogabooga uses a Unity build approach:
- All engine code is compiled together via `oogabooga.c`
- No separate object files or complex linking
- Extremely fast incremental builds
- Single-step compilation and execution

### Header Files for IDE Support

Unlike traditional "header-phobic" approaches, Oogabooga provides:
- Complete `.h` files for every `.c` file
- Umbrella header `oogabooga.h` for application inclusion
- Full IDE support (IntelliJ, Visual Studio, VS Code, etc.)
- Proper symbol navigation and error checking
- Professional development workflow compatibility

## Course: From Scratch to Steam

This project was started to be used in a course detailing the full ride from starting out making a game to publishing it to Steam. If you're keen on going all-in on getting a small game published to steam within 2-3 months, then check it out for free in our [Skool Community](https://www.skool.com/game-dev).

## Quickstart

### Windows
1. Install Visual Studio 2019+ with C++ build tools, or install MinGW
2. Clone repo to `<project_dir>`
3. Create your game file `my_game.c`:

```c
#include "oogabooga/oogabooga.h"

int entry(int argc, char **argv) {
    window.title = STR("My Game");
    window.scaled_width = 1280;
    window.scaled_height = 720;
    window.clear_color = hex_to_rgba(0x6495EDff);

    while (!window.should_close) {
        reset_temporary_storage();
        
        os_update(); 
        gfx_update();
    }

    return 0;
}
```

4. Add to `build.c`:
```c
#include "my_game.c"
```

5. Build:
   - **MSVC (default)**: `build_tool.exe`
   - **MinGW**: `build_tool.exe --useMinGWOnWindows`

6. Run: `build/game.exe`

### Linux
1. Install required packages by running `./install_deps.sh`

2. Clone the repository and create your game file (same as Windows)

3. Build with `./build_tool --verbose`

4. Run the game using `./build/game`

## Platform Support

### Windows
- **Compiler**: MSVC (default), MinGW/GCC (optional)
- **Graphics**: Direct3D 11
- **Audio**: Windows Audio Session API (WASAPI)
- **Windowing**: Win32 API

### Linux  
- **Compiler**: GCC
- **Graphics**: Vulkan (no SDL2 dependency)
- **Audio**: ALSA
- **Windowing**: X11

### Build Options
```bash
./build_tool [options]

Options:
  --useMinGWOnWindows   Use MinGW/GCC instead of MSVC on Windows
  --release             Build in release mode (default: debug)
  --verbose             Verbose output
  --help                Show help
```

## Architecture

The engine is organized with clear separation of concerns:

- **Platform Layer**: OS-specific implementations (Windows/Linux)
- **Graphics Layer**: Renderer-specific implementations (D3D11/Vulkan)
- **Engine Core**: Cross-platform game systems
- **Application Layer**: Your game code

Platform-specific code is completely hidden from application code. Your game includes only `oogabooga.h` and uses the same API regardless of platform.

## Known bugs & issues
- If DPI changes in runtime, updating window position or size will be a bit weird
- Converting 24-bit audio files doesn't really work
- Compiling with msys, cygwin, mingw etc fails

## Licensing
By default, the repository has an educational license that makes the engine free to use for personal projects.

[Educational license terms](https://github.com/alpinestudios/oogabooga/blob/master/LICENSE.md)

You can obtain the full commercial license by being an active member of the community and making your first game.

[Learn more here](https://www.skool.com/game-dev)

## Contributions
- Open PR's with `dev` as the base branch
- Keep it simple, no multi-layer abstractions
- Keep the implementation code readable, comment confusing code
- If you're introducing a new file/module, document the API and how to use it at the top of the file
- Add tests in tests.c if it makes sense to test
- Run tests (#define RUN_TESTS 1) before submitting PR
- Don't submit PR's for:
	- the sake of submitting PR's
	- Small polishing/tweaks that doesn't really affect the people making games
- When you submit a PR, please answer these prompts (if you're submitting a bugfix then you can skip this):
	- What feature/bugfix does this PR implement?
	- Why do we need this?
	- Describe at least one specific and practical problem this solves for people developing a game
	- Does this add complexity/friction for people making games? If so, how do you justify that?
