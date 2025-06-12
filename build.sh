#!/bin/sh
set -e
mkdir -p build
if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    COMPILER=x86_64-w64-mingw32-gcc
    OUT=build/cgame.exe
    LIBS="-lgdi32 -luser32 -lkernel32 -lwinmm -ld3d11 -ldxguid -ld3dcompiler"
else
    COMPILER=gcc
    OUT=build/game
    LIBS="-ldl -lpthread -lm"
fi
$COMPILER -std=c11 build.c $LIBS -o "$OUT" "$@"
