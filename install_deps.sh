#!/bin/sh
# Install gcc on common Linux distributions
set -e
if command -v apt-get >/dev/null 2>&1; then
    sudo apt-get update
    sudo apt-get install -y build-essential
elif command -v yay >/dev/null 2>&1; then
    yay -S --needed --noconfirm gcc
elif command -v pacman >/dev/null 2>&1; then
    sudo pacman -Sy --needed --noconfirm gcc
else
    echo "Unsupported package manager. Install gcc manually." >&2
    exit 1
fi
