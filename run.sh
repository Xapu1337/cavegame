#!/bin/sh
set -e

# Build and run the SDL version on Linux
sh ./build.sh "$@"
./build/game
