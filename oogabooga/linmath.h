#ifndef OOGABOOGA_LINMATH_H
#define OOGABOOGA_LINMATH_H

#include <stdint.h>

typedef union Vector2f32 Vector2f32;
typedef union Vector3f32 Vector3f32;
typedef union Vector4f32 Vector4f32;
typedef struct Matrix4 Matrix4;

// Basic vector and matrix type definitions used by the headers
typedef union Vector2f32 {
    float data[2];
    struct { float x, y; };
} Vector2f32;

typedef union Vector3f32 {
    float data[3];
    struct { float x, y, z; };
    struct { Vector2f32 xy; };
    struct { float _x; Vector2f32 yz; };
} Vector3f32;

typedef union Vector4f32 {
    float data[4];
    struct { float x, y, z, w; };
    struct { Vector2f32 xy; Vector2f32 zw; };
    struct { float x1, y1, x2, y2; };
    struct { float r, g, b, a; };
    struct { float left, bottom, right, top; };
    struct { Vector3f32 xyz; };
    struct { float _x; Vector3f32 yzw; };
} Vector4f32;

typedef union Quaternion {
    float data[4];
    struct {float x,y,z,w;};
    struct {Vector3f32 xyz; float w_;};
} Quaternion;

Quaternion quat_identity(void);
Quaternion quat_from_axis_angle(Vector3f32 axis, float radians);
Quaternion quat_mul(Quaternion a, Quaternion b);
Matrix4    m4_from_quat_pos(Quaternion q, Vector3f32 pos);

typedef struct Matrix4 {
    union { float m[4][4]; float data[16]; };
} Matrix4;

#endif
