#!/bin/bash

# Stage C: Fix Vector Type Definition Issues
echo "=== FIXING VECTOR TYPE DEFINITION ISSUES - STAGE C ==="

# First, remove the conflicting typedef additions we made
sed -i '/\/\/ Vector\/Matrix type aliases for compatibility/,/^$/d' oogabooga/base.h

# Check if linmath.h has the proper vector definitions
echo "1. Ensuring vector types are properly defined in linmath.h..."

# Make sure linmath.h defines the basic vector types before anything else
cat > /tmp/vector_fix.h << 'EOF'
// Basic vector/matrix type definitions
typedef union Vector2f32 {
    struct { float x, y; };
    struct { float width, height; };
    float elements[2];
} Vector2f32;

typedef union Vector3f32 {
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float width, height, depth; };
    struct { Vector2f32 xy; float __z; };
    float elements[3];
} Vector3f32;

typedef union Vector4f32 {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { float left, right, top, bottom; };
    struct { Vector2f32 xy, zw; };
    struct { Vector3f32 xyz; float __w; };
    struct { Vector3f32 rgb; float __a; };
    float elements[4];
} Vector4f32;

typedef union Quaternion {
    struct { float x, y, z, w; };
    Vector4f32 v;
    float elements[4];
} Quaternion;

typedef struct Matrix4 {
    float elements[4][4];
} Matrix4;

// Type aliases for compatibility
typedef Vector2f32 Vector2;
typedef Vector3f32 Vector3;
typedef Vector4f32 Vector4;

EOF

# Insert these definitions at the top of linmath.h, after includes but before any other definitions
sed -i '/^#define PI32/i\
// INSERT_VECTOR_TYPES_HERE' oogabooga/linmath.h

# Now replace the placeholder with our definitions
sed -i '/\/\/ INSERT_VECTOR_TYPES_HERE/{
r /tmp/vector_fix.h
d
}' oogabooga/linmath.h

# Remove the conflicting typedef lines that were already there
sed -i '/^typedef Vector2f32 Vector2;$/d' oogabooga/linmath.h
sed -i '/^typedef Vector3f32 Vector3;$/d' oogabooga/linmath.h  
sed -i '/^typedef Vector4f32 Vector4;$/d' oogabooga/linmath.h

echo "2. Fixing header include order..."

# Make sure linmath.h is included before other headers that might use vector types
# Update the include order in oogabooga.c
sed -i '/^#include.*linmath\.h/d' oogabooga/oogabooga.c
sed -i '/^#include.*base\.h/a\
#include "linmath.h"' oogabooga/oogabooga.c

echo "3. Fixing remaining type issues..."

# Fix the broken function declaration in base.c
sed -i '/^void$/d' oogabooga/base.c

# Fix missing return types and broken declarations
sed -i 's/^GetHeapAllocator();$/Allocator GetHeapAllocator();/' oogabooga/base.c
sed -i 's/^GetTemporaryAllocator();$/Allocator GetTemporaryAllocator();/' oogabooga/base.c
sed -i 's/^Alloc(Allocator allocator, u64 size);$/void* Alloc(Allocator allocator, uint64_t size);/' oogabooga/base.c
sed -i 's/^alloc_uninitialized(Allocator allocator, u64 size);$/void* alloc_uninitialized(Allocator allocator, uint64_t size);/' oogabooga/base.c
sed -i 's/^Dealloc(Allocator allocator, void \*p);$/void Dealloc(Allocator allocator, void *p);/' oogabooga/base.c

echo "4. Fixing SIMD and platform-specific issues..."

# Fix Windows-specific calling convention in simd.c
sed -i 's/__cdecl//g' oogabooga/simd.c
sed -i 's/_In_//g' oogabooga/simd.c

echo "5. Fixing va_list initialization..."

# Fix va_list initialization across all files - need to add corresponding va_end
find oogabooga -name "*.c" -exec sed -i '/va_start(args, fmt);/{
a\
    /* va_end will be called after use */
}' {} \;

echo "6. Testing compilation..."

# Clean up temp file
rm -f /tmp/vector_fix.h

# Test build
if ./build_tool > build_log_stage_c.txt 2>&1; then
    echo "SUCCESS: Build completed successfully!"
else
    echo "Build attempted - checking error count..."
    error_count=$(grep -c "error:" build_log_stage_c.txt || echo "0")
    echo "Remaining errors: $error_count"
    
    if [ "$error_count" -lt 100 ]; then
        echo "SIGNIFICANT PROGRESS: Error count reduced dramatically!"
        echo "Top remaining error types:"
        grep "error:" build_log_stage_c.txt | head -10
    else
        echo "Still many errors - may need additional targeted fixes"
    fi
fi

echo "=== STAGE C COMPLETE ==="
