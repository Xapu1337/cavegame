#!/bin/bash
set -e

updateSystem() {
    sudo apt-get update
    sudo apt-get upgrade -y
}

installDevPackages() {
    sudo apt-get install -y build-essential pkg-config git \
        libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev \
        libvulkan-dev vulkan-tools libsdl2-dev libgl1-mesa-dev \
        libglu1-mesa-dev xdg-utils
}

main() {
    updateSystem
    installDevPackages
    echo "Development environment ready"
}

main "$@"
