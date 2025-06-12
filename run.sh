#!/bin/sh
set -e

# Build and run the native version
sh ./build.sh "$@"
./build/game
