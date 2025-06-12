#!/bin/sh
set -e
mkdir -p build
gcc -std=c11 app/sdl_cube_flop.c -lm $(sdl2-config --cflags --libs) -lGL -lGLU -o build/game "$@"
