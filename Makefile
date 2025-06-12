# Simple Makefile to build the cube flop example
.PHONY: all linux windows clean

BUILD_DIR := build

# Windows build flags
WIN_CC := x86_64-w64-mingw32-gcc
WIN_SRC := build.c
WIN_FLAGS := -g -O0 -std=c11 --static -D_CRT_SECURE_NO_WARNINGS \
        -Wextra -Wno-sign-compare -Wno-unused-parameter \
        -lkernel32 -lgdi32 -luser32 -lruntimeobject \
        -lwinmm -ld3d11 -ldxguid -ld3dcompiler \
        -lshlwapi -lole32 -lavrt -lksuser -ldbghelp \
        -lshcore

# Linux build flags
LINUX_CC := gcc
LINUX_SRC := entry_cube_sdl.c
LINUX_FLAGS := -g -O0 `sdl2-config --cflags` -lGL -lGLU `sdl2-config --libs` -lm

all:
	@if [ "$(OS)" = "Windows_NT" ]; then \
		$(MAKE) windows; \
	else \
		$(MAKE) linux; \
	fi

linux: $(BUILD_DIR)/game

$(BUILD_DIR)/game: $(LINUX_SRC) | $(BUILD_DIR)
	$(LINUX_CC) $< -o $@ $(LINUX_FLAGS) $(EXTRA)

windows: $(BUILD_DIR)/game.exe

$(BUILD_DIR)/game.exe: $(WIN_SRC) | $(BUILD_DIR)
	$(WIN_CC) $< -o $@ $(WIN_FLAGS) $(EXTRA)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
