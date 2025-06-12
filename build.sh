#!/bin/sh
set -e

# Simple build script that compiles build.c using gcc.
# If mingw is available, you can cross-compile on Linux for Windows
# by running build.bat under Wine or using x86_64-w64-mingw32-gcc.

mkdir -p build
gcc -std=c11 -g -O0 build.c -o build/game "$@"
