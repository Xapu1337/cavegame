#!/bin/sh
# Install required packages to build and run the example on Linux
set -e
if command -v apt-get >/dev/null 2>&1; then
    sudo apt-get update
    sudo apt-get install -y gcc-mingw-w64-x86-64 wine libsdl2-dev libgl1-mesa-dev
elif command -v yay >/dev/null 2>&1; then
    yay -S --needed --noconfirm mingw-w64-gcc wine sdl2 mesa
elif command -v pacman >/dev/null 2>&1; then
    sudo pacman -Sy --needed --noconfirm mingw-w64-gcc wine sdl2 mesa
else
    echo "Unsupported package manager. Install mingw-w64, SDL2 and wine manually." >&2
    exit 1
fi
