#!/bin/sh
mkdir -p build
cd build
if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    CC=x86_64-w64-mingw32-gcc
    SRC=../build.c
    CFLAGS="-g -O0 -std=c11 --static -D_CRT_SECURE_NO_WARNINGS \
        -Wextra -Wno-sign-compare -Wno-unused-parameter \
        -lkernel32 -lgdi32 -luser32 -lruntimeobject \
        -lwinmm -ld3d11 -ldxguid -ld3dcompiler \
        -lshlwapi -lole32 -lavrt -lksuser -ldbghelp \
        -lshcore"
    EXE=game.exe
    $CC $SRC -o $EXE $CFLAGS "$@"
else
    CC=gcc
    SRC=../entry_cube_sdl.c
    CFLAGS="-g -O0 `sdl2-config --cflags` -lGL -lGLU `sdl2-config --libs` -lm"
    EXE=game
    $CC $SRC -o $EXE $CFLAGS "$@"
fi
cd ..
