#!/bin/sh
set -e
sh build.sh "$@"
if [ -f build/game.exe ]; then
    if command -v wine >/dev/null 2>&1; then
        wine build/game.exe
    else
        echo "Wine is not installed. Run ./install_deps.sh first." >&2
        exit 1
    fi
else
    ./build/game
fi
