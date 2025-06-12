#!/bin/sh
set -e
sh ./build.sh "$@"
if [ -f build/game ]; then
    ./build/game
elif [ -f build/cgame.exe ]; then
    if command -v wine >/dev/null 2>&1; then
        wine build/cgame.exe
    else
        echo "Built Windows binary but Wine is not installed" >&2
    fi
else
    echo "No executable built" >&2
fi
