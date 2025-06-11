#!/bin/sh
# Install required packages to build and run the example on Linux
set -e
sudo apt-get update
sudo apt-get install -y gcc-mingw-w64-x86-64 wine
