#ifndef OOGABOOGA_LINMATH_H
#define OOGABOOGA_LINMATH_H

#include <stdint.h>
#include <stdbool.h>

// Basic type definitions
typedef float f32;
typedef double f64;
typedef int32_t s32;
typedef int64_t s64;
typedef uint32_t u32;
typedef uint64_t u64;

// Mathematical constants
#define PI32 3.14159265358979323846f
#define TAU32 (2.0f * PI32)
#define DEG_TO_RAD (PI32 / 180.0f)
#define RAD_TO_DEG (180.0f / PI32)

// Float vector types
typedef union Vector2f32 {
    float data[2];
    struct { float x, y; };
    struct { float width, height; };
    float elements[2];
} Vector2f32;

typedef union Vector3f32 {
    float data[3];
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float width, height, depth; };
    struct { Vector2f32 xy; float __z; };
    float elements[3];
} Vector3f32;

typedef union Vector4f32 {
    float data[4];
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { float left, right, top, bottom; };
    struct { Vector2f32 xy, zw; };
    struct { Vector3f32 xyz; float __w; };
    struct { Vector3f32 rgb; float __a; };
    float elements[4];
} Vector4f32;

typedef union Vector2f64 {
    double data[2];
    struct { double x, y; };
    double elements[2];
} Vector2f64;

typedef union Vector3f64 {
    double data[3];
    struct { double x, y, z; };
    struct { Vector2f64 xy; double __z; };
    double elements[3];
} Vector3f64;

typedef union Vector4f64 {
    double data[4];
    struct { double x, y, z, w; };
    struct { Vector2f64 xy, zw; };
    struct { Vector3f64 xyz; double __w; };
    double elements[4];
} Vector4f64;

// Integer vector types
typedef union Vector2s32 {
    s32 data[2];
    struct { s32 x, y; };
    s32 elements[2];
} Vector2s32;

typedef union Vector3s32 {
    s32 data[3];
    struct { s32 x, y, z; };
    struct { Vector2s32 xy; s32 __z; };
    s32 elements[3];
} Vector3s32;

typedef union Vector4s32 {
    s32 data[4];
    struct { s32 x, y, z, w; };
    struct { Vector2s32 xy, zw; };
    struct { Vector3s32 xyz; s32 __w; };
    s32 elements[4];
} Vector4s32;

typedef union Vector2s64 {
    s64 data[2];
    struct { s64 x, y; };
    s64 elements[2];
} Vector2s64;

typedef union Vector3s64 {
    s64 data[3];
    struct { s64 x, y, z; };
    struct { Vector2s64 xy; s64 __z; };
    s64 elements[3];
} Vector3s64;

typedef union Vector4s64 {
    s64 data[4];
    struct { s64 x, y, z, w; };
    struct { Vector2s64 xy, zw; };
    struct { Vector3s64 xyz; s64 __w; };
    s64 elements[4];
} Vector4s64;

// Matrix and quaternion types
typedef struct Matrix4 {
    union { float m[4][4]; float data[16]; };
} Matrix4;

typedef union Quaternion {
    float data[4];
    struct { float x, y, z, w; };
    struct { Vector3f32 xyz; float w_; };
    Vector4f32 v;
    float elements[4];
} Quaternion;

// Type aliases for compatibility
typedef Vector2f32 Vector2;
typedef Vector3f32 Vector3;
typedef Vector4f32 Vector4;
typedef Vector2s32 Vector2i;
typedef Vector3s32 Vector3i;
typedef Vector4s32 Vector4i;

// Quaternion functions
Quaternion QuatIdentity(void);
Quaternion QuatFromAxisAngle(Vector3f32 axis, float radians);
Quaternion QuatMul(Quaternion a, Quaternion b);
Matrix4    M4FromQuatPos(Quaternion q, Vector3f32 pos);

// Vector constructor functions
Vector2f32 V2(float x, float y);
Vector3f32 V3(float x, float y, float z);
Vector4f32 V4(float x, float y, float z, float w);

Vector2f64 v2f64(double x, double y);
Vector3f64 v3f64(double x, double y, double z);
Vector4f64 v4f64(double x, double y, double z, double w);

Vector2s32 v2s32(s32 x, s32 y);
Vector3s32 v3s32(s32 x, s32 y, s32 z);
Vector4s32 v4s32(s32 x, s32 y, s32 z, s32 w);

Vector2s64 v2s64(s64 x, s64 y);
Vector3s64 v3s64(s64 x, s64 y, s64 z);
Vector4s64 v4s64(s64 x, s64 y, s64 z, s64 w);

// Integer vector aliases for convenience
#define v2i v2s32
#define v3i v3s32
#define v4i v4s32

// Integer vector operations
Vector2s32 v2s32_add(Vector2s32 a, Vector2s32 b);
Vector3s32 v3s32_add(Vector3s32 a, Vector3s32 b);
Vector4s32 v4s32_add(Vector4s32 a, Vector4s32 b);

Vector2s32 v2s32_sub(Vector2s32 a, Vector2s32 b);
Vector3s32 v3s32_sub(Vector3s32 a, Vector3s32 b);
Vector4s32 v4s32_sub(Vector4s32 a, Vector4s32 b);

Vector2s32 v2s32_mul(Vector2s32 a, Vector2s32 b);
Vector3s32 v3s32_mul(Vector3s32 a, Vector3s32 b);
Vector4s32 v4s32_mul(Vector4s32 a, Vector4s32 b);

Vector2s32 v2s32_muls32(Vector2s32 a, s32 s);
Vector3s32 v3s32_muls32(Vector3s32 a, s32 s);
Vector4s32 v4s32_muls32(Vector4s32 a, s32 s);

Vector2s32 v2s32_div(Vector2s32 a, Vector2s32 b);
Vector3s32 v3s32_div(Vector3s32 a, Vector3s32 b);
Vector4s32 v4s32_div(Vector4s32 a, Vector4s32 b);

Vector2s32 v2s32_divs32(Vector2s32 a, s32 s);
Vector3s32 v3s32_divs32(Vector3s32 a, s32 s);
Vector4s32 v4s32_divs32(Vector4s32 a, s32 s);

// Integer vector operation aliases
#define v2i_add v2s32_add
#define v3i_add v3s32_add
#define v4i_add v4s32_add
#define v2i_sub v2s32_sub
#define v3i_sub v3s32_sub
#define v4i_sub v4s32_sub
#define v2i_mul v2s32_mul
#define v3i_mul v3s32_mul
#define v4i_mul v4s32_mul
#define v2i_muli v2s32_muls32
#define v3i_muli v3s32_muls32
#define v4i_muli v4s32_muls32
#define v2i_div v2s32_div
#define v3i_div v3s32_div
#define v4i_div v4s32_div
#define v2i_divi v2s32_divs32
#define v3i_divi v3s32_divs32
#define v4i_divi v4s32_divs32

// Vector scalar functions
Vector2f32 v2f32_scalar(float a);
Vector3f32 v3f32_scalar(float a);
Vector4f32 v4f32_scalar(float a);

// Vector arithmetic
Vector2f32 V2Add(Vector2f32 a, Vector2f32 b);
Vector2f32 V2Sub(Vector2f32 a, Vector2f32 b);
Vector2f32 V2Mul(Vector2f32 a, Vector2f32 b);
Vector2f32 V2Mulf(Vector2f32 a, float s);
Vector2f32 V2Div(Vector2f32 a, Vector2f32 b);
Vector2f32 V2Divf(Vector2f32 a, float s);

Vector3f32 V3Add(Vector3f32 a, Vector3f32 b);
Vector3f32 V3Sub(Vector3f32 a, Vector3f32 b);
Vector3f32 V3Mul(Vector3f32 a, Vector3f32 b);
Vector3f32 V3Mulf(Vector3f32 a, float s);
Vector3f32 V3Div(Vector3f32 a, Vector3f32 b);
Vector3f32 V3Divf(Vector3f32 a, float s);

Vector4f32 V4Add(Vector4f32 a, Vector4f32 b);
Vector4f32 V4Sub(Vector4f32 a, Vector4f32 b);
Vector4f32 V4Mul(Vector4f32 a, Vector4f32 b);
Vector4f32 V4Mulf(Vector4f32 a, float s);
Vector4f32 V4Div(Vector4f32 a, Vector4f32 b);
Vector4f32 V4Divf(Vector4f32 a, float s);

// Vector length and normalization
float v2f32_length(Vector2f32 a);
float v3f32_length(Vector3f32 a);
float v4f32_length(Vector4f32 a);

Vector2f32 V2Normalize(Vector2f32 a);
Vector3f32 v3f32_normalize(Vector3f32 a);
Vector4f32 v4f32_normalize(Vector4f32 a);

// Vector utility functions
Vector2f32 v2f32_abs(Vector2f32 a);
Vector3f32 v3f32_abs(Vector3f32 a);
Vector4f32 v4f32_abs(Vector4f32 a);

Vector2f32 v2f32_lerp(Vector2f32 a, Vector2f32 b, float t);
Vector3f32 v3f32_lerp(Vector3f32 a, Vector3f32 b, float t);
Vector4f32 v4f32_lerp(Vector4f32 a, Vector4f32 b, float t);

Vector2f32 v2f32_smerp(Vector2f32 a, Vector2f32 b, float t);
Vector3f32 v3f32_smerp(Vector3f32 a, Vector3f32 b, float t);
Vector4f32 v4f32_smerp(Vector4f32 a, Vector4f32 b, float t);

// Vector dot and cross products
float v2f32_dot(Vector2f32 a, Vector2f32 b);
float v3f32_dot(Vector3f32 a, Vector3f32 b);
float v4f32_dot(Vector4f32 a, Vector4f32 b);

Vector3f32 v3f32_cross(Vector3f32 a, Vector3f32 b);

// Constants
extern const Vector2f32 v2f32_zero;
extern const Vector2f32 v2f32_one;
extern const Vector3f32 v3f32_zero;
extern const Vector3f32 v3f32_one;
extern const Vector4f32 v4f32_zero;
extern const Vector4f32 v4f32_one;

extern const Vector2f64 v2f64_zero;
extern const Vector2f64 v2f64_one;
extern const Vector3f64 v3f64_zero;
extern const Vector3f64 v3f64_one;
extern const Vector4f64 v4f64_zero;
extern const Vector4f64 v4f64_one;

extern const Vector2s32 v2s32_zero;
extern const Vector2s32 v2s32_one;
extern const Vector3s32 v3s32_zero;
extern const Vector3s32 v3s32_one;
extern const Vector4s32 v4s32_zero;
extern const Vector4s32 v4s32_one;

extern const Vector2s64 v2s64_zero;
extern const Vector2s64 v2s64_one;
extern const Vector3s64 v3s64_zero;
extern const Vector3s64 v3s64_one;
extern const Vector4s64 v4s64_zero;
extern const Vector4s64 v4s64_one;

/*
	Abbreviated type aliases for convenience:
*/

/*
	Function macros for abbreviated types:
*/
#define v2_expand(v) (v).x, (v).y
#define v3_expand(v) (v).x, (v).y, (v).z
#define v4_expand(v) (v).x, (v).y, (v).z, (v).w

#define v2 V2
#define v3 V3
#define v4 V4

#define v2_scalar v2f32_scalar
#define v3_scalar v3f32_scalar
#define v4_scalar v4f32_scalar

// Vector operation aliases
#define V3Mulf  V3Mulf

// Utility macros
#define clamp(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

//
// Functions
//

// Vector functions
Vector3f32 V3Mulf(Vector3f32 a, float s);
Vector3f32 V3Mul(Vector3f32 a, Vector3f32 b);

// Matrix4 functions
Matrix4 M4Identity(void);
Matrix4 M4Scalar(float s);
Matrix4 m4_make_rotation_z(float radians);
Matrix4 m4_make_rotation(Vector3f32 axis, float radians);
Matrix4 M4MakePerspectiveProjection(float fov_y, float aspect, float _near, float _far);
Matrix4 M4MakeLookAt(Vector3f32 eye, Vector3f32 target, Vector3f32 up);
Matrix4 M4Translate(Matrix4 m, Vector3f32 translation);
Matrix4 M4Rotate(Matrix4 m, Vector3f32 axis, float radians);
Matrix4 M4RotateX(Matrix4 m, float radians);
Matrix4 M4RotateY(Matrix4 m, float radians);
Matrix4 M4RotateZ(Matrix4 m, float radians);
Matrix4 M4Scale(Matrix4 m, Vector3f32 scale);

// Drawing functions  
typedef struct Draw_Quad Draw_Quad;
Draw_Quad* DrawRectXform(Matrix4 xform, Vector2f32 size, Vector4f32 color);

#endif
