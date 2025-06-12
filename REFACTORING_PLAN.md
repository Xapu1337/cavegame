# Oogabooga Engine Refactoring Plan

## Naming Convention Changes
- **Variables**: snake_case → camelCase
- **Functions**: snake_case → PascalCase  
- **Constants**: UPPER_SNAKE → SCREAMING_SNAKE (no change needed)

## Phase 1: Critical Fixes (Foundation)
- [ ] Remove duplicate type definitions between headers
- [ ] Fix conflicting function declarations
- [ ] Consolidate forward declarations
- [ ] Remove redefined macros and constants
- [ ] Fix type compatibility issues

## Phase 2: Core Types & Base System
- [ ] Refactor base.h/base.c (allocators, basic types)
- [ ] Refactor string system (string.h/string.c)
- [ ] Refactor memory system (memory.h/memory.c)

## Phase 3: Math & Linear Math
- [ ] Refactor linmath.h/linmath.c (all vector/matrix functions)
- [ ] Update vector constructors (v2f32 → V2f32, etc.)
- [ ] Update matrix functions (m4_identity → M4Identity, etc.)

## Phase 4: Core Engine Systems
- [ ] Refactor os_interface.h/os_interface.c
- [ ] Refactor drawing.h/drawing.c
- [ ] Refactor input.h/input.c
- [ ] Refactor gfx_interface.h

## Phase 5: Utility Systems  
- [ ] Refactor hash.h/hash.c
- [ ] Refactor growing_array.h/growing_array.c
- [ ] Refactor path_utils.h/path_utils.c
- [ ] Refactor string_format.h/string_format.c

## Phase 6: Platform & Implementation
- [ ] Refactor os implementation files
- [ ] Refactor graphics implementation files
- [ ] Update tests.c

## Phase 7: Examples & Build
- [ ] Update entry_cube_flop.c
- [ ] Update build.c configuration
- [ ] Test compilation with build_tool
- [ ] Fix any remaining issues

## Key Function Renames (Examples)
```
get_heap_allocator() → GetHeapAllocator()
alloc_string() → AllocString()  
v2f32() → V2f32()
m4_identity() → M4Identity()
draw_rect_xform() → DrawRectXform()
os_get_elapsed_seconds() → OsGetElapsedSeconds()
```

## Critical Files to Start With
1. base.h/base.c - Core allocator and type system
2. string.h/string.c - String handling
3. linmath.h/linmath.c - Math functions heavily used
4. os_interface.h - OS abstraction layer
5. drawing.h/drawing.c - Main drawing API

## Build Strategy
- Use build_tool.c instead of old build scripts
- Focus on Linux/Vulkan build path initially
- Fix each compilation error systematically
- Test with simple examples first

## Estimated Effort
- Phase 1: 2-3 hours (critical for any progress)
- Phases 2-6: 1-2 days each (substantial refactoring)
- Phase 7: 1 day (integration and testing)
- **Total: 1-2 weeks of focused work**

This refactoring touches virtually every line of code in the engine.
