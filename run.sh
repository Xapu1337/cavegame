#!/bin/sh
# Build the game and run it with Wine
set -e
sh build.sh
wine build/game.exe
