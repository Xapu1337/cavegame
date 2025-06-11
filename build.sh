#!/bin/sh

CROSS_CC=x86_64-w64-mingw32-gcc
CC=${CC:-$CROSS_CC}
CFLAGS="-g -O0 -std=c11 --static -D_CRT_SECURE_NO_WARNINGS \
        -Wextra -Wno-sign-compare -Wno-unused-parameter \
        -lkernel32 -lgdi32 -luser32 -lruntimeobject \
        -lwinmm -ld3d11 -ldxguid -ld3dcompiler \
        -lshlwapi -lole32 -lavrt -lksuser -ldbghelp \
        -lshcore"
SRC=../build.c
EXENAME=game.exe

if ! command -v "$CC" >/dev/null 2>&1; then
    echo "$CC not found. Install the mingw-w64 cross compiler (try ./install_deps.sh)." >&2
    exit 1
fi

mkdir -p build
cd build
$CC $SRC -o $EXENAME $CFLAGS "$@"
cd ..
