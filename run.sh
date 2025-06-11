#!/bin/sh
# Build the game and run it with Wine
set -e
sh build.sh "$@"
if command -v wine >/dev/null 2>&1; then
    wine build/game.exe
else
    echo "Wine is not installed. Run ./install_deps.sh first." >&2
    exit 1
fi
