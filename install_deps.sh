#!/bin/sh
# Install required packages to build and run the example on Linux
set -e
if command -v apt-get >/dev/null 2>&1; then
    sudo apt-get update
    sudo apt-get install -y build-essential \
        libx11-dev libvulkan-dev vulkan-tools \
        libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev
elif command -v yay >/dev/null 2>&1; then
    yay -S --needed --noconfirm gcc make libx11 vulkan-headers vulkan-validation-layers vulkan-tools sdl2 mesa glu
elif command -v pacman >/dev/null 2>&1; then
    sudo pacman -Sy --needed --noconfirm gcc make libx11 vulkan-headers vulkan-validation-layers vulkan-tools sdl2 mesa glu
else
    echo "Unsupported package manager. Install gcc, X11, Vulkan, SDL2, GL and GLU libraries manually." >&2
    exit 1
fi
