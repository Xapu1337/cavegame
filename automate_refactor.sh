#!/bin/bash

# Oogabooga Engine Naming Convention Automation Script
# Converts snake_case to camelCase for variables, PascalCase for functions

set -e

echo "Oogabooga Engine Refactoring Automation"
echo "========================================"

# Backup the original files first
echo "Creating backup..."
cp -r oogabooga oogabooga_backup
cp entry_cube_flop.c entry_cube_flop.c.backup
cp build.c build.c.backup

echo "Starting automated refactoring..."

# Function to convert snake_case to PascalCase
convert_to_pascal_case() {
    echo "$1" | sed 's/_\([a-z]\)/\U\1/g' | sed 's/^[a-z]/\U&/'
}

# Function to convert snake_case to camelCase  
convert_to_camel_case() {
    echo "$1" | sed 's/_\([a-z]\)/\U\1/g'
}

# List of common function patterns to convert (starting with most commonly used)
FUNCTIONS_TO_CONVERT=(
    # Math functions (heavily used)
    "v2f32" "v3f32" "v4f32"
    "v2_add" "v2_sub" "v2_mul" "v2_div" 
    "v3_add" "v3_sub" "v3_mul" "v3_div"
    "v4_add" "v4_sub" "v4_mul" "v4_div"
    "v2_mulf" "v3_mulf" "v4_mulf"
    "v2_divf" "v3_divf" "v4_divf"
    "v2_normalize" "v3_normalize" "v4_normalize"
    "v2_length" "v3_length" "v4_length"
    "v2_dot" "v3_dot" "v4_dot"
    "v3_cross"
    "m4_identity" "m4_scalar" "m4_translate" "m4_rotate" "m4_scale"
    "m4_make_perspective_projection" "m4_make_look_at"
    "m4_rotate_x" "m4_rotate_y" "m4_rotate_z"
    "m4_from_quat_pos"
    "quat_identity" "quat_from_axis_angle" "quat_mul"
    
    # Memory functions
    "get_heap_allocator" "get_temporary_allocator"
    "alloc" "dealloc" "alloc_string" "dealloc_string"
    "reset_temporary_storage"
    
    # String functions
    "alloc_string" "string_concat" "strings_match"
    "convert_to_null_terminated_string"
    "temp_convert_to_null_terminated_string"
    "string_find_from_left" "string_copy"
    "length_of_null_terminated_string"
    
    # Drawing functions
    "draw_rect_xform" "draw_quad_in_frame" "draw_frame_reset"
    "draw_rect_in_frame" "draw_circle_in_frame"
    
    # OS functions
    "os_get_elapsed_seconds" "os_update" "os_make_mutex"
    "os_lock_mutex" "os_unlock_mutex"
    
    # Input functions
    "is_key_down" "is_key_just_pressed"
    
    # Graphics functions
    "gfx_update"
)

# Phase 1: Convert function names to PascalCase
echo "Phase 1: Converting function names to PascalCase..."
for func in "${FUNCTIONS_TO_CONVERT[@]}"; do
    pascal_name=$(convert_to_pascal_case "$func")
    echo "Converting $func -> $pascal_name"
    
    # Convert in all source files
    find . -name "*.c" -o -name "*.h" | grep -E "\.(c|h)$" | while read file; do
        if [[ -f "$file" ]]; then
            # Replace function definitions and declarations
            sed -i "s/\b${func}(/${pascal_name}(/g" "$file"
            # Replace function pointers and typedefs
            sed -i "s/\b${func}\b/${pascal_name}/g" "$file"
        fi
    done
done

# Phase 2: Fix common variable patterns
echo "Phase 2: Converting common variable patterns to camelCase..."

# Common variable patterns to convert
VARIABLE_PATTERNS=(
    "draw_frame" "drawFrame"
    "input_frame" "inputFrame" 
    "cube_transform" "cubeTransform"
    "camera_xform" "cameraXform"
    "clear_color" "clearColor"
    "mouse_pos" "mousePos"
    "window_handle" "windowHandle"
    "buffer_size" "bufferSize"
    "file_path" "filePath"
    "start_index" "startIndex"
    "end_index" "endIndex"
    "block_size" "blockSize"
    "array_size" "arraySize"
)

for ((i=0; i<${#VARIABLE_PATTERNS[@]}; i+=2)); do
    snake_var="${VARIABLE_PATTERNS[i]}"
    camel_var="${VARIABLE_PATTERNS[i+1]}"
    
    echo "Converting variable pattern: $snake_var -> $camel_var"
    
    find . -name "*.c" -o -name "*.h" | grep -E "\.(c|h)$" | while read file; do
        if [[ -f "$file" ]]; then
            # Convert struct member access
            sed -i "s/\\.${snake_var}/.${camel_var}/g" "$file"
            # Convert variable declarations and usage (be careful not to match function names)
            sed -i "s/\b${snake_var}\b/${camel_var}/g" "$file"
        fi
    done
done

# Phase 3: Fix type compatibility issues
echo "Phase 3: Fixing type compatibility issues..."

# Standardize on specific types
find . -name "*.h" | while read file; do
    if [[ -f "$file" ]]; then
        # Standardize integer types
        sed -i 's/\bu64\b/uint64_t/g' "$file"
        sed -i 's/\bu32\b/uint32_t/g' "$file" 
        sed -i 's/\bu16\b/uint16_t/g' "$file"
        sed -i 's/\bu8\b/uint8_t/g' "$file"
        sed -i 's/\bs64\b/int64_t/g' "$file"
        sed -i 's/\bs32\b/int32_t/g' "$file"
        sed -i 's/\bs16\b/int16_t/g' "$file"
        sed -i 's/\bs8\b/int8_t/g' "$file"
        sed -i 's/\bfloat32\b/float/g' "$file"
        sed -i 's/\bfloat64\b/double/g' "$file"
    fi
done

echo "Automated refactoring complete!"
echo "Backup saved to oogabooga_backup/"
echo "Testing compilation..."

# Try to compile
if ./build_tool --verbose; then
    echo "SUCCESS: Compilation successful after automated refactoring!"
else
    echo "Compilation failed. Manual fixes needed."
    echo "You can restore from backup if needed: cp -r oogabooga_backup oogabooga"
fi
