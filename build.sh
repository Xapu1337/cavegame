#!/bin/sh
set -e

# Simple Linux build script that compiles the SDL/OpenGL version
# of the cube flop example. This no longer attempts any Windows
# cross‑compilation.

mkdir -p build
gcc entry_cube_sdl.c -g -O0 $(sdl2-config --cflags --libs) -lGL -lGLU -lm -o build/game "$@"
