#!/bin/sh
set -e

# Build and run the SDL version on Linux
sh ./build.sh "$@"
if [ -z "$DISPLAY" ] && [ -z "$WAYLAND_DISPLAY" ]; then
    echo "Headless environment detected; build finished at build/game"
else
    ./build/game
fi
