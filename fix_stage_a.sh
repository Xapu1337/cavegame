#!/bin/bash

# Stage A: Fix Fundamental Type System Issues
# This script fixes the most critical problems that prevent compilation

set -e

echo "=== STAGE A: FUNDAMENTAL TYPE SYSTEM FIXES ==="
echo "This stage fixes:"
echo "1. Duplicate type definitions"
echo "2. Basic type standardization" 
echo "3. Critical naming convention fixes"
echo "================================================"

# Create backup if not exists
if [ ! -d "oogabooga_backup" ]; then
    echo "Creating backup..."
    cp -r oogabooga oogabooga_backup
    cp entry_cube_flop.c entry_cube_flop.c.backup 2>/dev/null || true
    cp build.c build.c.backup 2>/dev/null || true
fi

echo "Phase A1: Remove duplicate type definitions from os_interface.h"

# Remove duplicate struct string from os_interface.h 
sed -i '/^typedef struct string { uint64_t count; uint8_t \*data; } string;$/d' oogabooga/os_interface.h

# Remove duplicate Allocator_Message enum from os_interface.h
sed -i '/^typedef enum Allocator_Message {$/,/^} Allocator_Message;$/d' oogabooga/os_interface.h

# Remove duplicate Allocator struct from os_interface.h  
sed -i '/^typedef struct Allocator {$/,/^} Allocator;$/d' oogabooga/os_interface.h

echo "Phase A2: Fix broken typedefs in headers"

# Fix the typedef issues that were broken by the previous script
if grep -q "typedef f32 float" oogabooga/linmath.h; then
    sed -i 's/typedef f32 float;/typedef float f32;/' oogabooga/linmath.h
fi

if grep -q "typedef f64 double" oogabooga/linmath.h; then
    sed -i 's/typedef f64 double;/typedef double f64;/' oogabooga/linmath.h
fi

echo "Phase A3: Fix basic type standardization"

# Standardize primitive types in headers first 
find oogabooga -name "*.h" | while read file; do
    echo "Standardizing types in $file"
    # Use temporary file to avoid issues
    tmp_file=$(mktemp)
    
    # Convert primitive types to standard forms
    sed 's/\bu64\b/uint64_t/g' "$file" | \
    sed 's/\bu32\b/uint32_t/g' | \
    sed 's/\bu16\b/uint16_t/g' | \
    sed 's/\bu8\b/uint8_t/g' | \
    sed 's/\bs64\b/int64_t/g' | \
    sed 's/\bs32\b/int32_t/g' | \
    sed 's/\bs16\b/int16_t/g' | \
    sed 's/\bs8\b/int8_t/g' | \
    sed 's/\bfloat32\b/float/g' | \
    sed 's/\bfloat64\b/double/g' > "$tmp_file"
    
    mv "$tmp_file" "$file"
done

echo "Phase A4: Fix critical Vector type compatibility"

# The main issue is Vector2/Vector3/Vector4 vs Vector2f32/Vector3f32/Vector4f32 
# Let's standardize on Vector2f32 etc. as the base types and create aliases

echo "Fixing vector type aliases in linmath.h"
if ! grep -q "typedef Vector2f32 Vector2;" oogabooga/linmath.h; then
    # Add type aliases after the Vector definitions
    sed -i '/^} Vector4f32;$/a\\ntypedef Vector2f32 Vector2;\ntypedef Vector3f32 Vector3;\ntypedef Vector4f32 Vector4;' oogabooga/linmath.h
fi

echo "Phase A5: Fix the most critical function naming issues"

# Focus on the most commonly used functions that are breaking compilation
CRITICAL_FUNCTIONS=(
    "v2f32_add:V2Add"
    "v2f32_sub:V2Sub" 
    "v2f32_mul:V2Mul"
    "v2f32_div:V2Div"
    "v2f32_mulf32:V2Mulf"
    "v2f32_divf32:V2Divf"
    "v2f32_normalize:V2Normalize"
    "v3f32_add:V3Add"
    "v3f32_sub:V3Sub"
    "v3f32_mul:V3Mul"
    "v3f32_div:V3Div"
    "v3f32_mulf32:V3Mulf"
    "v3f32_divf32:V3Divf"
    "v4f32_add:V4Add"
    "v4f32_sub:V4Sub"
    "v4f32_mul:V4Mul"
    "v4f32_div:V4Div"
    "v4f32_mulf32:V4Mulf"
    "v4f32_divf32:V4Divf"
    "V2f32:V2"
    "V3f32:V3"
    "V4f32:V4"
)

for func_pair in "${CRITICAL_FUNCTIONS[@]}"; do
    old_name="${func_pair%:*}"
    new_name="${func_pair#*:}"
    
    echo "Converting $old_name -> $new_name"
    
    # Convert in all files, being careful about word boundaries
    find oogabooga -name "*.c" -o -name "*.h" | while read file; do
        sed -i "s/\b${old_name}\b/${new_name}/g" "$file"
    done
    
    # Also convert in entry files
    sed -i "s/\b${old_name}\b/${new_name}/g" entry_cube_flop.c 2>/dev/null || true
    sed -i "s/\b${old_name}\b/${new_name}/g" build.c 2>/dev/null || true
done

echo "Phase A6: Fix struct member naming for most common cases"

CRITICAL_STRUCT_MEMBERS=(
    "cameraXform"
    "clearColor"
    "projection"
)

for member in "${CRITICAL_STRUCT_MEMBERS[@]}"; do
    echo "Ensuring struct member .$member is properly named"
    # These should already be correct, but make sure
    find oogabooga -name "*.c" -o -name "*.h" | while read file; do
        # Convert camera_xform -> cameraXform
        sed -i "s/\.camera_xform/.cameraXform/g" "$file"
        # Convert clear_color -> clearColor  
        sed -i "s/\.clear_color/.clearColor/g" "$file"
    done
    
    # Also convert in entry files
    sed -i "s/\.camera_xform/.cameraXform/g" entry_cube_flop.c 2>/dev/null || true
    sed -i "s/\.clear_color/.clearColor/g" entry_cube_flop.c 2>/dev/null || true
done

echo "Phase A7: Test compilation"

echo "Testing compilation after Stage A fixes..."
if ./build_tool --verbose 2>&1 | tee build_log_stage_a.txt; then
    echo "SUCCESS: Stage A fixes resolved the compilation issues!"
    echo "You can proceed to Stage B for more comprehensive naming convention fixes."
else
    echo "Stage A fixes applied, but compilation still has issues."
    echo "Check build_log_stage_a.txt for details."
    echo "The most critical structural issues should now be resolved."
    echo "Remaining issues are likely more specific naming/compatibility problems."
fi

echo "Stage A complete!"
echo "Backup available at: oogabooga_backup/"
echo "Build log saved to: build_log_stage_a.txt"
