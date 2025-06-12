
// linmath.c implementation

#define v2_one  v2f32_one
#define v3_one  v3f32_one
#define v4_one  v4f32_one
#define v2i_one v2s32_one
#define v3i_one v3s32_one
#define v4i_one v4s32_one

#define v2_zero  v2f32_zero
#define v3_zero  v3f32_zero
#define v4_zero  v4f32_zero
#define v2i_zero v2s32_zero
#define v3i_zero v3s32_zero
#define v4i_zero v4s32_zero

#define V2Add  V2Add
#define V3Add  V3Add
#define V4Add  V4Add
#define v2i_add v2s32_add
#define v3i_add v3s32_add
#define v4i_add v4s32_add

#define V2Sub  V2Sub
#define V3Sub  V3Sub
#define V4Sub  V4Sub
#define v2i_sub v2s32_sub
#define v3i_sub v3s32_sub
#define v4i_sub v4s32_sub

#define V2Mul  V2Mul
#define V3Mul  V3Mul
#define V4Mul  V4Mul
#define v2i_mul v2s32_mul
#define v3i_mul v3s32_mul
#define v4i_mul v4s32_mul

#define V2Mulf  V2Mulf
#define V3Mulf  V3Mulf
#define V4Mulf  V4Mulf
#define v2i_muli v2s32_muls32
#define v3i_muli v3s32_muls32
#define v4i_muli v4s32_muls32

#define V2Div  V2Div
#define V3Div  V3Div
#define V4Div  V4Div
#define v2i_div v2s32_div
#define v3i_div v3s32_div
#define v4i_div v4s32_div

#define V2Divf  V2Divf
#define V3Divf  V3Divf
#define V4Divf  V4Divf
#define v2i_divi v2s32_divs32
#define v3i_divi v3s32_divs32
#define v4i_divi v4s32_divs32

#define V2Length  v2f32_length
#define V3Length  v3f32_length
#define V4Length  v4f32_length
#define v2i_length v2s32_length
#define v3i_length v3s32_length
#define v4i_length v4s32_length

#define v2_average  v2f32_average
#define v3_average  v3f32_average
#define v4_average  v4f32_average
#define v2i_average v2s32_average
#define v3i_average v3s32_average
#define v4i_average v4s32_average

#define v2_abs  v2f32_abs
#define v3_abs  v3f32_abs
#define v4_abs  v4f32_abs
#define v2i_abs v2s32_abs
#define v3i_abs v3s32_abs
#define v4i_abs v4s32_abs

#define v2_lerp  v2f32_lerp
#define v3_lerp  v3f32_lerp
#define v4_lerp  v4f32_lerp
#define v2i_lerp v2s32_lerp
#define v3i_lerp v3s32_lerp
#define v4i_lerp v4s32_lerp

#define v2_smerp  v2f32_smerp
#define v3_smerp  v3f32_smerp
#define v4_smerp  v4f32_smerp
#define v2i_smerp v2s32_smerp
#define v3i_smerp v3s32_smerp
#define v4i_smerp v4s32_smerp

#define V2Normalize  V2Normalize
#define V3Normalize  v3f32_normalize
#define V4Normalize  v4f32_normalize
#define v2i_normalize s2f32_normalize
#define v3i_normalize s3f32_normalize
#define v4i_normalize s4f32_normalize

#define V2Dot  v2f32_dot
#define V3Dot  v3f32_dot
#define V4Dot  v4f32_dot
#define v2i_dot v2s32_dot
#define v3i_dot v3s32_dot
#define v4i_dot v4s32_dot

#define v2_cross  v2f32_cross
#define V3Cross  v3f32_cross
#define v2i_cross v2s32_cross
#define v3i_cross v3s32_cross

#define v2_to_v2i v2f32_to_v2s32
#define v3_to_v3i v3f32_to_v3s32
#define v4_to_v4i v4f32_to_v4s32

#define v2i_to_v2 v2s32_to_v2f32
#define v3i_to_v3 v3s32_to_v3f32
#define v4i_to_v4 v4s32_to_v4f32

///
///
// Basic vector constructors
///
Vector2f32 V2(float32 x, float32 y) { Vector2f32 v = {x, y}; return v; }
Vector3f32 V3(float32 x, float32 y, float32 z) { Vector3f32 v = {x, y, z}; return v; }
Vector4f32 V4(float32 x, float32 y, float32 z, float32 w) { Vector4f32 v = {x, y, z, w}; return v; }
Vector2f64 v2f64(double x, double y) { Vector2f64 v = {x, y}; return v; }
Vector3f64 v3f64(double x, double y, double z) { Vector3f64 v = {x, y, z}; return v; }
Vector4f64 v4f64(double x, double y, double z, double w) { Vector4f64 v = {x, y, z, w}; return v; }
Vector2s32 v2s32(s32 x, s32 y) { Vector2s32 v = {x, y}; return v; }
Vector3s32 v3s32(s32 x, s32 y, s32 z) { Vector3s32 v = {x, y, z}; return v; }
Vector4s32 v4s32(s32 x, s32 y, s32 z, s32 w) { Vector4s32 v = {x, y, z, w}; return v; }
Vector2s64 v2s64(s64 x, s64 y) { Vector2s64 v = {x, y}; return v; }
Vector3s64 v3s64(s64 x, s64 y, s64 z) { Vector3s64 v = {x, y, z}; return v; }
Vector4s64 v4s64(s64 x, s64 y, s64 z, s64 w) { Vector4s64 v = {x, y, z, w}; return v; }

///
///
// Vectors - Function implementations
///


inline Vector2f32 v2f32_scalar(float32 a) { return (Vector2f32){a, a}; }
inline Vector3f32 v3f32_scalar(float32 a) { return (Vector3f32){a, a, a}; }
inline Vector4f32 v4f32_scalar(float32 a) { return (Vector4f32){a, a, a, a}; }
inline Vector2f64 v2f64_scalar(float32 a) { return (Vector2f64){a, a}; }
inline Vector3f64 v3f64_scalar(float32 a) { return (Vector3f64){a, a, a}; }
inline Vector4f64 v4f64_scalar(float32 a) { return (Vector4f64){a, a, a, a}; }
inline Vector2s32 v2s32_scalar(s32     a) { return (Vector2s32){a, a}; }
inline Vector3s32 v3s32_scalar(s32     a) { return (Vector3s32){a, a, a}; }
inline Vector4s32 v4s32_scalar(s32     a) { return (Vector4s32){a, a, a, a}; }
inline Vector2s64 v2s64_scalar(s64     a) { return (Vector2s64){a, a}; }
inline Vector3s64 v3s64_scalar(s64     a) { return (Vector3s64){a, a, a}; }
inline Vector4s64 v4s64_scalar(s64     a) { return (Vector4s64){a, a, a, a}; }

const Vector2f32 v2f32_one = {1, 1};
const Vector3f32 v3f32_one = {1, 1, 1};
const Vector4f32 v4f32_one = {1, 1, 1, 1};
const Vector2f64 v2f64_one = {1, 1};
const Vector3f64 v3f64_one = {1, 1, 1};
const Vector4f64 v4f64_one = {1, 1, 1, 1};
const Vector2s32 v2s32_one = {1, 1};
const Vector3s32 v3s32_one = {1, 1, 1};
const Vector4s32 v4s32_one = {1, 1, 1, 1};
const Vector2s64 v2s64_one = {1, 1};
const Vector3s64 v3s64_one = {1, 1, 1};
const Vector4s64 v4s64_one = {1, 1, 1, 1};

const Vector2f32 v2f32_zero = {0, 0};
const Vector3f32 v3f32_zero = {0, 0, 0};
const Vector4f32 v4f32_zero = {0, 0, 0, 0};
const Vector2f64 v2f64_zero = {0, 0};
const Vector3f64 v3f64_zero = {0, 0, 0};
const Vector4f64 v4f64_zero = {0, 0, 0, 0};
const Vector2s32 v2s32_zero = {0, 0};
const Vector3s32 v3s32_zero = {0, 0, 0};
const Vector4s32 v4s32_zero = {0, 0, 0, 0};
const Vector2s64 v2s64_zero = {0, 0};
const Vector3s64 v3s64_zero = {0, 0, 0};
const Vector4s64 v4s64_zero = {0, 0, 0, 0};

inline Vector2f32 V2Add(Vector2f32 a, Vector2f32 b)  { return V2(a.x+b.x, a.y+b.y); }
inline Vector3f32 V3Add(Vector3f32 a, Vector3f32 b)  { return V3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline Vector4f32 V4Add(Vector4f32 a, Vector4f32 b)  { return V4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
inline Vector2f64 v2f64_add(Vector2f64 a, Vector2f64 b)  { return v2f64(a.x+b.x, a.y+b.y); }
inline Vector3f64 v3f64_add(Vector3f64 a, Vector3f64 b)  { return v3f64(a.x+b.x, a.y+b.y, a.z+b.z); }
inline Vector4f64 v4f64_add(Vector4f64 a, Vector4f64 b)  { return v4f64(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
inline Vector2s32 v2s32_add(Vector2s32 a, Vector2s32 b)  { return v2s32(a.x+b.x, a.y+b.y); }
inline Vector3s32 v3s32_add(Vector3s32 a, Vector3s32 b)  { return v3s32(a.x+b.x, a.y+b.y, a.z+b.z); }
inline Vector4s32 v4s32_add(Vector4s32 a, Vector4s32 b)  { return v4s32(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
inline Vector2s64 v2s64_add(Vector2s64 a, Vector2s64 b)  { return v2s64(a.x+b.x, a.y+b.y); }
inline Vector3s64 v3s64_add(Vector3s64 a, Vector3s64 b)  { return v3s64(a.x+b.x, a.y+b.y, a.z+b.z); }
inline Vector4s64 v4s64_add(Vector4s64 a, Vector4s64 b)  { return v4s64(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }

inline Vector2f32 V2Sub(Vector2f32 a, Vector2f32 b)  { return V2(a.x-b.x, a.y-b.y); }
inline Vector3f32 V3Sub(Vector3f32 a, Vector3f32 b)  { return V3(a.x-b.x, a.y-b.y, a.z-b.z); }
inline Vector4f32 V4Sub(Vector4f32 a, Vector4f32 b)  { return V4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }
inline Vector2f64 v2f64_sub(Vector2f64 a, Vector2f64 b)  { return v2f64(a.x-b.x, a.y-b.y); }
inline Vector3f64 v3f64_sub(Vector3f64 a, Vector3f64 b)  { return v3f64(a.x-b.x, a.y-b.y, a.z-b.z); }
inline Vector4f64 v4f64_sub(Vector4f64 a, Vector4f64 b)  { return v4f64(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }
inline Vector2s32 v2s32_sub(Vector2s32 a, Vector2s32 b)  { return v2s32(a.x-b.x, a.y-b.y); }
inline Vector3s32 v3s32_sub(Vector3s32 a, Vector3s32 b)  { return v3s32(a.x-b.x, a.y-b.y, a.z-b.z); }
inline Vector4s32 v4s32_sub(Vector4s32 a, Vector4s32 b)  { return v4s32(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }
inline Vector2s64 v2s64_sub(Vector2s64 a, Vector2s64 b)  { return v2s64(a.x-b.x, a.y-b.y); }
inline Vector3s64 v3s64_sub(Vector3s64 a, Vector3s64 b)  { return v3s64(a.x-b.x, a.y-b.y, a.z-b.z); }
inline Vector4s64 v4s64_sub(Vector4s64 a, Vector4s64 b)  { return v4s64(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }

inline Vector2f32 V2Mul(Vector2f32 a, Vector2f32 b)  { return V2(a.x*b.x, a.y*b.y); }
inline Vector3f32 V3Mul(Vector3f32 a, Vector3f32 b)  { return V3(a.x*b.x, a.y*b.y, a.z*b.z); }
inline Vector4f32 V4Mul(Vector4f32 a, Vector4f32 b)  { return V4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w); }
inline Vector2f64 v2f64_mul(Vector2f64 a, Vector2f64 b)  { return v2f64(a.x*b.x, a.y*b.y); }
inline Vector3f64 v3f64_mul(Vector3f64 a, Vector3f64 b)  { return v3f64(a.x*b.x, a.y*b.y, a.z*b.z); }
inline Vector4f64 v4f64_mul(Vector4f64 a, Vector4f64 b)  { return v4f64(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w); }
inline Vector2s32 v2s32_mul(Vector2s32 a, Vector2s32 b)  { return v2s32(a.x*b.x, a.y*b.y); }
inline Vector3s32 v3s32_mul(Vector3s32 a, Vector3s32 b)  { return v3s32(a.x*b.x, a.y*b.y, a.z*b.z); }
inline Vector4s32 v4s32_mul(Vector4s32 a, Vector4s32 b)  { return v4s32(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w); }
inline Vector2s64 v2s64_mul(Vector2s64 a, Vector2s64 b)  { return v2s64(a.x*b.x, a.y*b.y); }
inline Vector3s64 v3s64_mul(Vector3s64 a, Vector3s64 b)  { return v3s64(a.x*b.x, a.y*b.y, a.z*b.z); }
inline Vector4s64 v4s64_mul(Vector4s64 a, Vector4s64 b)  { return v4s64(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w); }

inline Vector2f32 V2Mulf(Vector2f32 a, float32 s) { return V2Mul(a, V2(s, s)); }
inline Vector3f32 V3Mulf(Vector3f32 a, float32 s) { return V3Mul(a, V3(s, s, s)); }
inline Vector4f32 V4Mulf(Vector4f32 a, float32 s) { return V4Mul(a, V4(s, s, s, s)); }
inline Vector2f64 v2f64_mulf64(Vector2f64 a, float64 s) { return v2f64_mul(a, v2f64(s, s)); }
inline Vector3f64 v3f64_mulf64(Vector3f64 a, float64 s) { return v3f64_mul(a, v3f64(s, s, s)); }
inline Vector4f64 v4f64_mulf64(Vector4f64 a, float64 s) { return v4f64_mul(a, v4f64(s, s, s, s)); }
inline Vector2s32 v2s32_muls32(Vector2s32 a, s32     s) { return v2s32_mul(a, v2s32(s, s)); }
inline Vector3s32 v3s32_muls32(Vector3s32 a, s32     s) { return v3s32_mul(a, v3s32(s, s, s)); }
inline Vector4s32 v4s32_muls32(Vector4s32 a, s32     s) { return v4s32_mul(a, v4s32(s, s, s, s)); }
inline Vector2s64 v2s64_muls64(Vector2s64 a, s64     s) { return v2s64_mul(a, v2s64(s, s)); }
inline Vector3s64 v3s64_muls64(Vector3s64 a, s64     s) { return v3s64_mul(a, v3s64(s, s, s)); }
inline Vector4s64 v4s64_muls64(Vector4s64 a, s64     s) { return v4s64_mul(a, v4s64(s, s, s, s)); }

inline Vector2f32 V2Div(Vector2f32 a, Vector2f32 b)  { return V2(a.x/b.x, a.y/b.y); }
inline Vector3f32 V3Div(Vector3f32 a, Vector3f32 b)  { return V3(a.x/b.x, a.y/b.y, a.z/b.z); }
inline Vector4f32 V4Div(Vector4f32 a, Vector4f32 b)  { return V4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w); }
inline Vector2f64 v2f64_div(Vector2f64 a, Vector2f64 b)  { return v2f64(a.x/b.x, a.y/b.y); }
inline Vector3f64 v3f64_div(Vector3f64 a, Vector3f64 b)  { return v3f64(a.x/b.x, a.y/b.y, a.z/b.z); }
inline Vector4f64 v4f64_div(Vector4f64 a, Vector4f64 b)  { return v4f64(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w); }
inline Vector2s32 v2s32_div(Vector2s32 a, Vector2s32 b)  { return v2s32(a.x/b.x, a.y/b.y); }
inline Vector3s32 v3s32_div(Vector3s32 a, Vector3s32 b)  { return v3s32(a.x/b.x, a.y/b.y, a.z/b.z); }
inline Vector4s32 v4s32_div(Vector4s32 a, Vector4s32 b)  { return v4s32(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w); }
inline Vector2s64 v2s64_div(Vector2s64 a, Vector2s64 b)  { return v2s64(a.x/b.x, a.y/b.y); }
inline Vector3s64 v3s64_div(Vector3s64 a, Vector3s64 b)  { return v3s64(a.x/b.x, a.y/b.y, a.z/b.z); }
inline Vector4s64 v4s64_div(Vector4s64 a, Vector4s64 b)  { return v4s64(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w); }

inline Vector2f32 V2Divf(Vector2f32 a, float32 s) { return V2Div(a, V2(s, s)); }
inline Vector3f32 V3Divf(Vector3f32 a, float32 s) { return V3Div(a, V3(s, s, s)); }
inline Vector4f32 V4Divf(Vector4f32 a, float32 s) { return V4Div(a, V4(s, s, s, s)); }
inline Vector2f64 v2f64_divf64(Vector2f64 a, float64 s) { return v2f64_div(a, v2f64(s, s)); }
inline Vector3f64 v3f64_divf64(Vector3f64 a, float64 s) { return v3f64_div(a, v3f64(s, s, s)); }
inline Vector4f64 v4f64_divf64(Vector4f64 a, float64 s) { return v4f64_div(a, v4f64(s, s, s, s)); }
inline Vector2s32 v2s32_divs32(Vector2s32 a, s32     s) { return v2s32_div(a, v2s32(s, s)); }
inline Vector3s32 v3s32_divs32(Vector3s32 a, s32     s) { return v3s32_div(a, v3s32(s, s, s)); }
inline Vector4s32 v4s32_divs32(Vector4s32 a, s32     s) { return v4s32_div(a, v4s32(s, s, s, s)); }
inline Vector2s64 v2s64_divs64(Vector2s64 a, s64     s) { return v2s64_div(a, v2s64(s, s)); }
inline Vector3s64 v3s64_divs64(Vector3s64 a, s64     s) { return v3s64_div(a, v3s64(s, s, s)); }
inline Vector4s64 v4s64_divs64(Vector4s64 a, s64     s) { return v4s64_div(a, v4s64(s, s, s, s)); }

inline float32 v2f32_length(Vector2f32 a) { return sqrt(a.x*a.x + a.y*a.y); }
inline float32 v3f32_length(Vector3f32 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
inline float32 v4f32_length(Vector4f32 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w); }
inline float64 v2f64_length(Vector2f64 a) { return sqrt(a.x*a.x + a.y*a.y); }
inline float64 v3f64_length(Vector3f64 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
inline float64 v4f64_length(Vector4f64 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w); }
inline float32 v2s32_length(Vector2s32 a) { return sqrt(a.x*a.x + a.y*a.y); }
inline float32 v3s32_length(Vector3s32 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
inline float32 v4s32_length(Vector4s32 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w); }
inline float64 v2s64_length(Vector2s64 a) { return sqrt(a.x*a.x + a.y*a.y); }
inline float64 v3s64_length(Vector3s64 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
inline float64 v4s64_length(Vector4s64 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w); }

inline float32 v2f32_average(Vector2f32 a) { return (a.x+a.y)/2.0; }
inline float32 v3f32_average(Vector3f32 a) { return (a.x + a.y + a.z) / 3.0; }
inline float32 v4f32_average(Vector4f32 a) { return (a.x + a.y + a.z + a.w) / 4.0; }
inline float64 v2f64_average(Vector2f64 a) { return (a.x+a.y)/2.0; }
inline float64 v3f64_average(Vector3f64 a) { return (a.x + a.y + a.z) / 3.0; }
inline float64 v4f64_average(Vector4f64 a) { return (a.x + a.y + a.z + a.w) / 4.0; }
inline float32 v2s32_average(Vector2s32 a) { return (a.x+a.y)/2.0; }
inline float32 v3s32_average(Vector3s32 a) { return (a.x + a.y + a.z) / 3.0; }
inline float32 v4s32_average(Vector4s32 a) { return (a.x + a.y + a.z + a.w) / 4.0; }
inline float64 v2s64_average(Vector2s64 a) { return (a.x+a.y)/2.0; }
inline float64 v3s64_average(Vector3s64 a) { return (a.x + a.y + a.z) / 3.0; }
inline float64 v4s64_average(Vector4s64 a) { return (a.x + a.y + a.z + a.w) / 4.0; }

inline Vector2f32 v2f32_abs(Vector2f32 a) { return V2(fabsf(a.x), fabsf(a.y)); }
inline Vector3f32 v3f32_abs(Vector3f32 a) { return V3(fabsf(a.x), fabsf(a.y), fabsf(a.z)); }
inline Vector4f32 v4f32_abs(Vector4f32 a) { return V4(fabsf(a.x), fabsf(a.y), fabsf(a.z), fabsf(a.w)); }
inline Vector2f64 v2f64_abs(Vector2f64 a) { return v2f64(fabs(a.x), fabs(a.y)); }
inline Vector3f64 v3f64_abs(Vector3f64 a) { return v3f64(fabs(a.x), fabs(a.y), fabs(a.z)); }
inline Vector4f64 v4f64_abs(Vector4f64 a) { return v4f64(fabs(a.x), fabs(a.y), fabs(a.z), fabs(a.w)); }
inline Vector2s32 v2s32_abs(Vector2s32 a) { return v2s32(abs(a.x), abs(a.y)); }
inline Vector3s32 v3s32_abs(Vector3s32 a) { return v3s32(abs(a.x), abs(a.y), abs(a.z)); }
inline Vector4s32 v4s32_abs(Vector4s32 a) { return v4s32(abs(a.x), abs(a.y), abs(a.z), abs(a.w)); }
inline Vector2s64 v2s64_abs(Vector2s64 a) { return v2s64(llabs(a.x), llabs(a.y)); }
inline Vector3s64 v3s64_abs(Vector3s64 a) { return v3s64(llabs(a.x), llabs(a.y), llabs(a.z)); }
inline Vector4s64 v4s64_abs(Vector4s64 a) { return v4s64(llabs(a.x), llabs(a.y), llabs(a.z), llabs(a.w)); }

inline Vector2f32 v2f32_lerp(Vector2f32 a, Vector2f32 b, float32 t)  { return V2(lerpf32(a.x, b.x, t), lerpf32(a.y, b.y, t)); }
inline Vector3f32 v3f32_lerp(Vector3f32 a, Vector3f32 b, float32 t)  { return V3(lerpf32(a.x, b.x, t), lerpf32(a.y, b.y, t), lerpf32(a.z, b.z, t)); }
inline Vector4f32 v4f32_lerp(Vector4f32 a, Vector4f32 b, float32 t)  { return V4(lerpf32(a.x, b.x, t), lerpf32(a.y, b.y, t), lerpf32(a.z, b.z, t), lerpf32(a.w, b.w, t)); }
inline Vector2f64 v2f64_lerp(Vector2f64 a, Vector2f64 b, float64 t)  { return v2f64(lerpf64(a.x, b.x, t), lerpf64(a.y, b.y, t)); }
inline Vector3f64 v3f64_lerp(Vector3f64 a, Vector3f64 b, float64 t)  { return v3f64(lerpf64(a.x, b.x, t), lerpf64(a.y, b.y, t), lerpf64(a.z, b.z, t)); }
inline Vector4f64 v4f64_lerp(Vector4f64 a, Vector4f64 b, float64 t)  { return v4f64(lerpf64(a.x, b.x, t), lerpf64(a.y, b.y, t), lerpf64(a.z, b.z, t), lerpf64(a.w, b.w, t)); }
inline Vector2s32 v2s32_lerp(Vector2s32 a, Vector2s32 b, float64 t)  { return v2s32(lerpi(a.x, b.x, t), lerpi(a.y, b.y, t)); }
inline Vector3s32 v3s32_lerp(Vector3s32 a, Vector3s32 b, float64 t)  { return v3s32(lerpi(a.x, b.x, t), lerpi(a.y, b.y, t), lerpi(a.z, b.z, t)); }
inline Vector4s32 v4s32_lerp(Vector4s32 a, Vector4s32 b, float64 t)  { return v4s32(lerpi(a.x, b.x, t), lerpi(a.y, b.y, t), lerpi(a.z, b.z, t), lerpi(a.w, b.w, t)); }
inline Vector2s64 v2s64_lerp(Vector2s64 a, Vector2s64 b, float64 t)  { return v2s64(lerpi(a.x, b.x, t), lerpi(a.y, b.y, t)); }
inline Vector3s64 v3s64_lerp(Vector3s64 a, Vector3s64 b, float64 t)  { return v3s64(lerpi(a.x, b.x, t), lerpi(a.y, b.y, t), lerpi(a.z, b.z, t)); }
inline Vector4s64 v4s64_lerp(Vector4s64 a, Vector4s64 b, float64 t)  { return v4s64(lerpi(a.x, b.x, t), lerpi(a.y, b.y, t), lerpi(a.z, b.z, t), lerpi(a.w, b.w, t)); }

inline Vector2f32 v2f32_smerp(Vector2f32 a, Vector2f32 b, float32 t)  { return V2(smerpf32(a.x, b.x, t), smerpf32(a.y, b.y, t)); }
inline Vector3f32 v3f32_smerp(Vector3f32 a, Vector3f32 b, float32 t)  { return V3(smerpf32(a.x, b.x, t), smerpf32(a.y, b.y, t), smerpf32(a.z, b.z, t)); }
inline Vector4f32 v4f32_smerp(Vector4f32 a, Vector4f32 b, float32 t)  { return V4(smerpf32(a.x, b.x, t), smerpf32(a.y, b.y, t), smerpf32(a.z, b.z, t), smerpf32(a.w, b.w, t)); }
inline Vector2f64 v2f64_smerp(Vector2f64 a, Vector2f64 b, float64 t)  { return v2f64(smerpf64(a.x, b.x, t), smerpf64(a.y, b.y, t)); }
inline Vector3f64 v3f64_smerp(Vector3f64 a, Vector3f64 b, float64 t)  { return v3f64(smerpf64(a.x, b.x, t), smerpf64(a.y, b.y, t), smerpf64(a.z, b.z, t)); }
inline Vector4f64 v4f64_smerp(Vector4f64 a, Vector4f64 b, float64 t)  { return v4f64(smerpf64(a.x, b.x, t), smerpf64(a.y, b.y, t), smerpf64(a.z, b.z, t), smerpf64(a.w, b.w, t)); }
inline Vector2s32 v2s32_smerp(Vector2s32 a, Vector2s32 b, float64 t)  { return v2s32(smerpi(a.x, b.x, t),   smerpi(a.y, b.y, t)); }
inline Vector3s32 v3s32_smerp(Vector3s32 a, Vector3s32 b, float64 t)  { return v3s32(smerpi(a.x, b.x, t),   smerpi(a.y, b.y, t), smerpi(a.z, b.z, t)); }
inline Vector4s32 v4s32_smerp(Vector4s32 a, Vector4s32 b, float64 t)  { return v4s32(smerpi(a.x, b.x, t),   smerpi(a.y, b.y, t), smerpi(a.z, b.z, t), smerpi(a.w, b.w, t)); }
inline Vector2s64 v2s64_smerp(Vector2s64 a, Vector2s64 b, float64 t)  { return v2s64(smerpi(a.x, b.x, t),   smerpi(a.y, b.y, t)); }
inline Vector3s64 v3s64_smerp(Vector3s64 a, Vector3s64 b, float64 t)  { return v3s64(smerpi(a.x, b.x, t),   smerpi(a.y, b.y, t), smerpi(a.z, b.z, t)); }
inline Vector4s64 v4s64_smerp(Vector4s64 a, Vector4s64 b, float64 t)  { return v4s64(smerpi(a.x, b.x, t),   smerpi(a.y, b.y, t), smerpi(a.z, b.z, t), smerpi(a.w, b.w, t)); }

inline Vector2f32 V2Normalize(Vector2f32 a) { return v2f32_length(a) == 0 ? v2f32_scalar(0) : V2Divf(a, v2f32_length(a)); }
inline Vector3f32 v3f32_normalize(Vector3f32 a) { return v3f32_length(a) == 0 ? v3f32_scalar(0) : V3Divf(a, v3f32_length(a)); }
inline Vector4f32 v4f32_normalize(Vector4f32 a) { return v4f32_length(a) == 0 ? v4f32_scalar(0) : V4Divf(a, v4f32_length(a)); }
inline Vector2f64 v2f64_normalize(Vector2f64 a) { return v2f64_length(a) == 0 ? v2f64_scalar(0) : v2f64_divf64(a, v2f64_length(a)); }
inline Vector3f64 v3f64_normalize(Vector3f64 a) { return v3f64_length(a) == 0 ? v3f64_scalar(0) : v3f64_divf64(a, v3f64_length(a)); }
inline Vector4f64 v4f64_normalize(Vector4f64 a) { return v4f64_length(a) == 0 ? v4f64_scalar(0) : v4f64_divf64(a, v4f64_length(a)); }
inline Vector2s32 v2s32_normalize(Vector2s32 a) { return v2s32_length(a) == 0 ? v2s32_scalar(0) : v2s32_divs32(a, v2s32_length(a)); }
inline Vector3s32 v3s32_normalize(Vector3s32 a) { return v3s32_length(a) == 0 ? v3s32_scalar(0) : v3s32_divs32(a, v3s32_length(a)); }
inline Vector4s32 v4s32_normalize(Vector4s32 a) { return v4s32_length(a) == 0 ? v4s32_scalar(0) : v4s32_divs32(a, v4s32_length(a)); }
inline Vector2s64 v2s64_normalize(Vector2s64 a) { return v2s64_length(a) == 0 ? v2s64_scalar(0) : v2s64_divs64(a, v2s64_length(a)); }
inline Vector3s64 v3s64_normalize(Vector3s64 a) { return v3s64_length(a) == 0 ? v3s64_scalar(0) : v3s64_divs64(a, v3s64_length(a)); }
inline Vector4s64 v4s64_normalize(Vector4s64 a) { return v4s64_length(a) == 0 ? v4s64_scalar(0) : v4s64_divs64(a, v4s64_length(a)); }

inline float32 v2f32_dot(Vector2f32 a, Vector2f32 b) { return simd_dot_product_float32_64((float32*)&a, (float32*)&b); }
inline float32 v3f32_dot(Vector3f32 a, Vector3f32 b) { return simd_dot_product_float32_96((float32*)&a, (float32*)&b); }
inline float32 v4f32_dot(Vector4f32 a, Vector4f32 b) { return simd_dot_product_float32_128((float32*)&a, (float32*)&b); }
inline float64 v2f64_dot(Vector2f64 a, Vector2f64 b) { return a.x * b.x + a.y * b.y; }
inline float64 v3f64_dot(Vector3f64 a, Vector3f64 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline float64 v4f64_dot(Vector4f64 a, Vector4f64 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
// v2s32_dot is not reasonable
// v3s32_dot is not reasonable
// v4s32_dot is not reasonable
// v2s64_dot is not reasonable
// v3s64_dot is not reasonable
// v4s64_dot is not reasonable

inline float32 v2f32_cross(Vector2f32 a, Vector2f32 b) { return (a.x * b.y) - (a.y * b.x); }
inline Vector3f32 v3f32_cross(Vector3f32 a, Vector3f32 b) { return V3((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x)); }
// v4f32_cross is not reasonable
inline float64 v2f64_cross(Vector2f64 a, Vector2f64 b) { return (a.x * b.y) - (a.y * b.x); }
inline Vector3f64 v3f64_cross(Vector3f64 a, Vector3f64 b) { return v3f64((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x)); }
// v4f64_cross is not reasonable
// v2s32_cross is not reasonable
// v3s32_cross is not reasonable
// v4s32_cross is not reasonable
// v2s64_cross is not reasonable
// v3s64_cross is not reasonable
// v4s64_cross is not reasonable


//
// Conversions
//

Vector2f32 v2f64_to_v2f32(Vector2f64 v) { return V2(v.x, v.y); }
Vector2f32 v2s64_to_v2f32(Vector2s64 v) { return V2(v.x, v.y); }
Vector2f32 v2s32_to_v2f32(Vector2s32 v) { return V2(v.x, v.y); }
Vector3f32 v3f64_to_v3f32(Vector3f64 v) { return V3(v.x, v.y, v.z); }
Vector3f32 v3s64_to_v3f32(Vector3s64 v) { return V3(v.x, v.y, v.z); }
Vector3f32 v3s32_to_v3f32(Vector3s32 v) { return V3(v.x, v.y, v.z); }
Vector4f32 v4f64_to_v4f32(Vector4f64 v) { return V4(v.x, v.y, v.z, v.w); }
Vector4f32 v4s64_to_v4f32(Vector4s64 v) { return V4(v.x, v.y, v.z, v.w); }
Vector4f32 v4s32_to_v4f32(Vector4s32 v) { return V4(v.x, v.y, v.z, v.w); }

Vector2f64 v2f32_to_v2f64(Vector2f32 v) { return v2f64(v.x, v.y); }
Vector2f64 v2s64_to_v2f64(Vector2s64 v) { return v2f64(v.x, v.y); }
Vector2f64 v2s32_to_v2f64(Vector2s32 v) { return v2f64(v.x, v.y); }
Vector3f64 v3f32_to_v3f64(Vector3f32 v) { return v3f64(v.x, v.y, v.z); }
Vector3f64 v3s64_to_v3f64(Vector3s64 v) { return v3f64(v.x, v.y, v.z); }
Vector3f64 v3s32_to_v3f64(Vector3s32 v) { return v3f64(v.x, v.y, v.z); }
Vector4f64 v4f32_to_v4f64(Vector4f32 v) { return v4f64(v.x, v.y, v.z, v.w); }
Vector4f64 v4s64_to_v4f64(Vector4s64 v) { return v4f64(v.x, v.y, v.z, v.w); }
Vector4f64 v4s32_to_v4f64(Vector4s32 v) { return v4f64(v.x, v.y, v.z, v.w); }

Vector2s32 v2f32_to_v2s32(Vector2f32 v) { return v2s32(v.x, v.y); }
Vector2s32 v2s64_to_v2s32(Vector2s64 v) { return v2s32(v.x, v.y); }
Vector2s32 v2f64_to_v2s32(Vector2f64 v) { return v2s32(v.x, v.y); }
Vector3s32 v3f32_to_v3s32(Vector3f32 v) { return v3s32(v.x, v.y, v.z); }
Vector3s32 v3s64_to_v3s32(Vector3s64 v) { return v3s32(v.x, v.y, v.z); }
Vector3s32 v3f64_to_v3s32(Vector3f64 v) { return v3s32(v.x, v.y, v.z); }
Vector4s32 v4f32_to_v4s32(Vector4f32 v) { return v4s32(v.x, v.y, v.z, v.w); }
Vector4s32 v4s64_to_v4s32(Vector4s64 v) { return v4s32(v.x, v.y, v.z, v.w); }
Vector4s32 v4f64_to_v4s32(Vector4f64 v) { return v4s32(v.x, v.y, v.z, v.w); }

Vector2s64 v2f32_to_v2s64(Vector2f32 v) { return v2s64(v.x, v.y); }
Vector2s64 v2s32_to_v2s64(Vector2s32 v) { return v2s64(v.x, v.y); }
Vector2s64 v2f64_to_v2s64(Vector2f64 v) { return v2s64(v.x, v.y); }
Vector3s64 v3f32_to_v3s64(Vector3f32 v) { return v3s64(v.x, v.y, v.z); }
Vector3s64 v3s32_to_v3s64(Vector3s32 v) { return v3s64(v.x, v.y, v.z); }
Vector3s64 v3f64_to_v3s64(Vector3f64 v) { return v3s64(v.x, v.y, v.z); }
Vector4s64 v4f32_to_v4s64(Vector4f32 v) { return v4s64(v.x, v.y, v.z, v.w); }
Vector4s64 v4s32_to_v4s64(Vector4s32 v) { return v4s64(v.x, v.y, v.z, v.w); }
Vector4s64 v4f64_to_v4s64(Vector4f64 v) { return v4s64(v.x, v.y, v.z, v.w); }


// #Cleanup deprecate?
Vector2f32 v2_rotate_point_around_pivot(Vector2f32 point, Vector2f32 pivot, float32 rotation_radians) {
    float32 s = sin(rotation_radians);
    float32 c = cos(rotation_radians);

    point = V2Sub(point, pivot);

    float32 x_new = point.x * c - point.y * s;
    float32 y_new = point.x * s + point.y * c;

    point = V2Add(V2(x_new, y_new), pivot);

    return point;
}

///
///
// Matrices
///


//
// Quaternion
//

Quaternion quat(float32 x, float32 y, float32 z, float32 w) {
    Quaternion q = { .x = x, .y = y, .z = z, .w = w };
    return q;
}

Quaternion QuatIdentity() { return quat(0,0,0,1); }

Quaternion QuatFromAxisAngle(Vector3f32 axis, float32 radians) {
    float32 h = radians * 0.5f;
    float32 s = sinf(h);
    return quat(axis.x*s, axis.y*s, axis.z*s, cosf(h));
}

Quaternion QuatMul(Quaternion a, Quaternion b) {
    Quaternion q;
    q.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    q.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    q.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    q.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
    return q;
}

Matrix4 M4FromQuatPos(Quaternion q, Vector3f32 pos) {
    Matrix4 m = M4Scalar(1.0f);
    float32 x = q.x, y = q.y, z = q.z, w = q.w;
    m.m[0][0] = 1 - 2*y*y - 2*z*z;
    m.m[0][1] = 2*x*y + 2*w*z;
    m.m[0][2] = 2*x*z - 2*w*y;
    m.m[1][0] = 2*x*y - 2*w*z;
    m.m[1][1] = 1 - 2*x*x - 2*z*z;
    m.m[1][2] = 2*y*z + 2*w*x;
    m.m[2][0] = 2*x*z + 2*w*y;
    m.m[2][1] = 2*y*z - 2*w*x;
    m.m[2][2] = 1 - 2*x*x - 2*y*y;
    m.m[0][3] = pos.x;
    m.m[1][3] = pos.y;
    m.m[2][3] = pos.z;
    return m;
}

//
// Matrix4
//


Matrix4 M4Scalar(float32 scalar) {
    Matrix4 m;
    for (int i = 0; i < 16; i++) {
        m.data[i] = 0.0f;
    }
    m.data[0] = scalar; 
    m.data[5] = scalar; 
    m.data[10] = scalar;
    m.data[15] = scalar;
    return m;
}

inline Matrix4 M4Identity() { return M4Scalar(1.0); }

Matrix4 m4_transpose(Matrix4 m) {
    Matrix4 transpose;
    transpose.m[0][0] = m.m[0][0];
    transpose.m[0][1] = m.m[1][0];
    transpose.m[0][2] = m.m[2][0];
    transpose.m[0][3] = m.m[3][0];

    transpose.m[1][0] = m.m[0][1];
    transpose.m[1][1] = m.m[1][1];
    transpose.m[1][2] = m.m[2][1];
    transpose.m[1][3] = m.m[3][1];

    transpose.m[2][0] = m.m[0][2];
    transpose.m[2][1] = m.m[1][2];
    transpose.m[2][2] = m.m[2][2];
    transpose.m[2][3] = m.m[3][2];

    transpose.m[3][0] = m.m[0][3];
    transpose.m[3][1] = m.m[1][3];
    transpose.m[3][2] = m.m[2][3];
    transpose.m[3][3] = m.m[3][3];

    return transpose;
}

Matrix4 m4_make_translation(Vector3f32 translation) {
    Matrix4 m = M4Scalar(1.0);
    m.m[0][0] = 1.0f; m.m[1][1] = 1.0f; m.m[2][2] = 1.0f; m.m[3][3] = 1.0f;
    m.m[0][3] = translation.x;
    m.m[1][3] = translation.y;
    m.m[2][3] = translation.z;
    return m;
}

Matrix4 m4_make_rotation(Vector3f32 axis, float32 radians) {
    Matrix4 m = M4Scalar(1.0);
    float32 c = cosf(radians);
    float32 s = sinf(radians);
    float32 t = 1.0f - c;

    m.m[0][0] = c + axis.x * axis.x * t;
    m.m[0][1] = axis.x * axis.y * t + axis.z * s;
    m.m[0][2] = axis.x * axis.z * t - axis.y * s;

    m.m[1][0] = axis.y * axis.x * t - axis.z * s;
    m.m[1][1] = c + axis.y * axis.y * t;
    m.m[1][2] = axis.y * axis.z * t + axis.x * s;

    m.m[2][0] = axis.z * axis.x * t + axis.y * s;
    m.m[2][1] = axis.z * axis.y * t - axis.x * s;
    m.m[2][2] = c + axis.z * axis.z * t;

    m.m[3][3] = 1.0f;
    return m;
}

inline Matrix4 m4_make_rotation_z(float32 radians) { return m4_make_rotation(V3(0, 0, 1), radians); }

Matrix4 m4_make_scale(Vector3f32 scale) {
    Matrix4 m = M4Scalar(1.0);
    m.m[0][0] = scale.x;
    m.m[1][1] = scale.y;
    m.m[2][2] = scale.z;
    m.m[3][3] = 1.0f;
    return m;
}

Matrix4 m4_mul(Matrix4 a, Matrix4 b) {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {

        	
            result.m[i][j] = a.m[i][0] * b.m[0][j] +
                             a.m[i][1] * b.m[1][j] +
                             a.m[i][2] * b.m[2][j] +
                             a.m[i][3] * b.m[3][j];
        }
    }
    return result;
}

inline Matrix4 M4Translate(Matrix4 m, Vector3f32 translation) {
    Matrix4 translation_matrix = m4_make_translation(translation);
    return m4_mul(m, translation_matrix);
}

inline Matrix4 M4Rotate(Matrix4 m, Vector3f32 axis, float32 radians) {
    Matrix4 rotation_matrix = m4_make_rotation(axis, radians);
    return m4_mul(m, rotation_matrix);
}
inline Matrix4 M4RotateZ(Matrix4 m, float32 radians) {
    Matrix4 rotation_matrix = m4_make_rotation(V3(0, 0, 1), radians);
    return m4_mul(m, rotation_matrix);
}
inline Matrix4 M4RotateX(Matrix4 m, float32 radians) {
    Matrix4 rotation_matrix = m4_make_rotation(V3(1, 0, 0), radians);
    return m4_mul(m, rotation_matrix);
}
inline Matrix4 M4RotateY(Matrix4 m, float32 radians) {
    Matrix4 rotation_matrix = m4_make_rotation(V3(0, 1, 0), radians);
    return m4_mul(m, rotation_matrix);
}

inline Matrix4 M4Scale(Matrix4 m, Vector3f32 scale) {
    Matrix4 scale_matrix = m4_make_scale(scale);
    return m4_mul(m, scale_matrix);
}


// _near & _far because microsoft...
Matrix4 m4_make_orthographic_projection(float32 left, float32 right, float32 bottom, float32 top, float32 _near, float32 _far) {
    Matrix4 m = M4Scalar(1.0f);
    m.m[0][0] = 2.0f / (right - left);
    m.m[1][1] = 2.0f / (top - bottom);
    m.m[2][2] = -2.0f / (_far - _near);
    m.m[0][3] = -(right + left) / (right - left);
    m.m[1][3] = -(top + bottom) / (top - bottom);
    m.m[2][3] = -(_far + _near) / (_far - _near);
    m.m[3][3] = 1.0f;
    return m;
}

Matrix4 M4MakePerspectiveProjection(float32 fov_y, float32 aspect, float32 _near, float32 _far) {
    Matrix4 m = ZERO(Matrix4);
    float32 f = 1.0f / tanf(fov_y * 0.5f);
    m.m[0][0] = f / aspect;
    m.m[1][1] = f;
    m.m[2][2] = (_far + _near) / (_near - _far);
    m.m[2][3] = (2.0f * _far * _near) / (_near - _far);
    m.m[3][2] = -1.0f;
    return m;
}

Matrix4 M4MakeLookAt(Vector3f32 eye, Vector3f32 target, Vector3f32 up) {
    Vector3f32 f = V3Normalize(V3Sub(target, eye));
    Vector3f32 s = V3Normalize(V3Cross(f, up));
    Vector3f32 u = V3Cross(s, f);

    Matrix4 m = M4Scalar(1.0f);
    m.m[0][0] = s.x; m.m[0][1] = s.y; m.m[0][2] = s.z; m.m[0][3] = -V3Dot(s, eye);
    m.m[1][0] = u.x; m.m[1][1] = u.y; m.m[1][2] = u.z; m.m[1][3] = -V3Dot(u, eye);
    m.m[2][0] = -f.x; m.m[2][1] = -f.y; m.m[2][2] = -f.z; m.m[2][3] = V3Dot(f, eye);
    m.m[3][3] = 1.0f;
    return m;
}

Vector4f32 m4_transform(Matrix4 m, Vector4f32 v) {
    Vector4f32 result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}
Matrix4 m4_inverse(Matrix4 m) {
    Matrix4 inv;
    float32 det;

    inv.m[0][0] = m.m[1][1] * m.m[2][2] * m.m[3][3] -
                  m.m[1][1] * m.m[2][3] * m.m[3][2] -
                  m.m[2][1] * m.m[1][2] * m.m[3][3] +
                  m.m[2][1] * m.m[1][3] * m.m[3][2] +
                  m.m[3][1] * m.m[1][2] * m.m[2][3] -
                  m.m[3][1] * m.m[1][3] * m.m[2][2];

    inv.m[1][0] = -m.m[1][0] * m.m[2][2] * m.m[3][3] +
                  m.m[1][0] * m.m[2][3] * m.m[3][2] +
                  m.m[2][0] * m.m[1][2] * m.m[3][3] -
                  m.m[2][0] * m.m[1][3] * m.m[3][2] -
                  m.m[3][0] * m.m[1][2] * m.m[2][3] +
                  m.m[3][0] * m.m[1][3] * m.m[2][2];

    inv.m[2][0] = m.m[1][0] * m.m[2][1] * m.m[3][3] -
                  m.m[1][0] * m.m[2][3] * m.m[3][1] -
                  m.m[2][0] * m.m[1][1] * m.m[3][3] +
                  m.m[2][0] * m.m[1][3] * m.m[3][1] +
                  m.m[3][0] * m.m[1][1] * m.m[2][3] -
                  m.m[3][0] * m.m[1][3] * m.m[2][1];

    inv.m[3][0] = -m.m[1][0] * m.m[2][1] * m.m[3][2] +
                   m.m[1][0] * m.m[2][2] * m.m[3][1] +
                   m.m[2][0] * m.m[1][1] * m.m[3][2] -
                   m.m[2][0] * m.m[1][2] * m.m[3][1] -
                   m.m[3][0] * m.m[1][1] * m.m[2][2] +
                   m.m[3][0] * m.m[1][2] * m.m[2][1];

    inv.m[0][1] = -m.m[0][1] * m.m[2][2] * m.m[3][3] +
                  m.m[0][1] * m.m[2][3] * m.m[3][2] +
                  m.m[2][1] * m.m[0][2] * m.m[3][3] -
                  m.m[2][1] * m.m[0][3] * m.m[3][2] -
                  m.m[3][1] * m.m[0][2] * m.m[2][3] +
                  m.m[3][1] * m.m[0][3] * m.m[2][2];

    inv.m[1][1] = m.m[0][0] * m.m[2][2] * m.m[3][3] -
                  m.m[0][0] * m.m[2][3] * m.m[3][2] -
                  m.m[2][0] * m.m[0][2] * m.m[3][3] +
                  m.m[2][0] * m.m[0][3] * m.m[3][2] +
                  m.m[3][0] * m.m[0][2] * m.m[2][3] -
                  m.m[3][0] * m.m[0][3] * m.m[2][2];

    inv.m[2][1] = -m.m[0][0] * m.m[2][1] * m.m[3][3] +
                   m.m[0][0] * m.m[2][3] * m.m[3][1] +
                   m.m[2][0] * m.m[0][1] * m.m[3][3] -
                   m.m[2][0] * m.m[0][3] * m.m[3][1] -
                   m.m[3][0] * m.m[0][1] * m.m[2][3] +
                   m.m[3][0] * m.m[0][3] * m.m[2][1];

    inv.m[3][1] = m.m[0][0] * m.m[2][1] * m.m[3][2] -
                  m.m[0][0] * m.m[2][2] * m.m[3][1] -
                  m.m[2][0] * m.m[0][1] * m.m[3][2] +
                  m.m[2][0] * m.m[0][2] * m.m[3][1] +
                  m.m[3][0] * m.m[0][1] * m.m[2][2] -
                  m.m[3][0] * m.m[0][2] * m.m[2][1];

    inv.m[0][2] = m.m[0][1] * m.m[1][2] * m.m[3][3] -
                  m.m[0][1] * m.m[1][3] * m.m[3][2] -
                  m.m[1][1] * m.m[0][2] * m.m[3][3] +
                  m.m[1][1] * m.m[0][3] * m.m[3][2] +
                  m.m[3][1] * m.m[0][2] * m.m[1][3] -
                  m.m[3][1] * m.m[0][3] * m.m[1][2];

    inv.m[1][2] = -m.m[0][0] * m.m[1][2] * m.m[3][3] +
                  m.m[0][0] * m.m[1][3] * m.m[3][2] +
                  m.m[1][0] * m.m[0][2] * m.m[3][3] -
                  m.m[1][0] * m.m[0][3] * m.m[3][2] -
                  m.m[3][0] * m.m[0][2] * m.m[1][3] +
                  m.m[3][0] * m.m[0][3] * m.m[1][2];

    inv.m[2][2] = m.m[0][0] * m.m[1][1] * m.m[3][3] -
                  m.m[0][0] * m.m[1][3] * m.m[3][1] -
                  m.m[1][0] * m.m[0][1] * m.m[3][3] +
                  m.m[1][0] * m.m[0][3] * m.m[3][1] +
                  m.m[3][0] * m.m[0][1] * m.m[1][3] -
                  m.m[3][0] * m.m[0][3] * m.m[1][1];

    inv.m[3][2] = -m.m[0][0] * m.m[1][1] * m.m[3][2] +
                   m.m[0][0] * m.m[1][2] * m.m[3][1] +
                   m.m[1][0] * m.m[0][1] * m.m[3][2] -
                   m.m[1][0] * m.m[0][2] * m.m[3][1] -
                   m.m[3][0] * m.m[0][1] * m.m[1][2] +
                   m.m[3][0] * m.m[0][2] * m.m[1][1];

    inv.m[0][3] = -m.m[0][1] * m.m[1][2] * m.m[2][3] +
                   m.m[0][1] * m.m[1][3] * m.m[2][2] +
                   m.m[1][1] * m.m[0][2] * m.m[2][3] -
                   m.m[1][1] * m.m[0][3] * m.m[2][2] -
                   m.m[2][1] * m.m[0][2] * m.m[1][3] +
                   m.m[2][1] * m.m[0][3] * m.m[1][2];

    inv.m[1][3] = m.m[0][0] * m.m[1][2] * m.m[2][3] -
                  m.m[0][0] * m.m[1][3] * m.m[2][2] -
                  m.m[1][0] * m.m[0][2] * m.m[2][3] +
                  m.m[1][0] * m.m[0][3] * m.m[2][2] +
                  m.m[2][0] * m.m[0][2] * m.m[1][3] -
                  m.m[2][0] * m.m[0][3] * m.m[1][2];

    inv.m[2][3] = -m.m[0][0] * m.m[1][1] * m.m[2][3] +
                   m.m[0][0] * m.m[1][3] * m.m[2][1] +
                   m.m[1][0] * m.m[0][1] * m.m[2][3] -
                   m.m[1][0] * m.m[0][3] * m.m[2][1] -
                   m.m[2][0] * m.m[0][1] * m.m[1][3] +
                   m.m[2][0] * m.m[0][3] * m.m[1][1];

    inv.m[3][3] = m.m[0][0] * m.m[1][1] * m.m[2][2] -
                  m.m[0][0] * m.m[1][2] * m.m[2][1] -
                  m.m[1][0] * m.m[0][1] * m.m[2][2] +
                  m.m[1][0] * m.m[0][2] * m.m[2][1] +
                  m.m[2][0] * m.m[0][1] * m.m[1][2] -
                  m.m[2][0] * m.m[0][2] * m.m[1][1];

    det = m.m[0][0] * inv.m[0][0] + m.m[0][1] * inv.m[1][0] + m.m[0][2] * inv.m[2][0] + m.m[0][3] * inv.m[3][0];

    if (det == 0)
        return M4Scalar(0); 

    det = 1.0f / det;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            inv.m[i][j] *= det;
        }
    }

    return inv;
}




//
// Matrix3
//

typedef struct Matrix3 {
    union {
        float32 m[3][3]; 
        float32 data[9]; 
    };
} Matrix3;

// Sets m3 part of an identity m4
Matrix4 m3_to_m4(Matrix3 mat3) {
    Matrix4 mat4 = M4Identity();

	// Row 0    
    mat4.m[0][0] = mat3.m[0][0];
    mat4.m[0][1] = mat3.m[0][1];
    mat4.m[0][3] = mat3.m[0][2];
    
    // Row 1
    mat4.m[1][0] = mat3.m[1][0];
    mat4.m[1][1] = mat3.m[1][1];
    mat4.m[1][3] = mat3.m[1][2];
    
    // Leave row 2 as identity
    
    // Row 3
    mat4.m[3][0] = mat3.m[2][0];
    mat4.m[3][1] = mat3.m[2][1];
    mat4.m[3][3] = mat3.m[2][2];

    return mat4;
}

Matrix3 m3_scalar(float32 scalar) {
    Matrix3 m;
    for (int i = 0; i < 9; i++) {
        m.data[i] = 0.0f;
    }
    m.data[0] = scalar; 
    m.data[4] = scalar; 
    m.data[8] = scalar;
    return m;
}

inline Matrix3 m3_identity() { return m3_scalar(1.0f); }

Matrix3 m3_transpose(Matrix3 m) {
    Matrix3 transpose;
    transpose.m[0][0] = m.m[0][0];
    transpose.m[0][1] = m.m[1][0];
    transpose.m[0][2] = m.m[2][0];

    transpose.m[1][0] = m.m[0][1];
    transpose.m[1][1] = m.m[1][1];
    transpose.m[1][2] = m.m[2][1];

    transpose.m[2][0] = m.m[0][2];
    transpose.m[2][1] = m.m[1][2];
    transpose.m[2][2] = m.m[2][2];

    return transpose;
}

Matrix3 m3_make_translation(Vector2f32 translation) {
    Matrix3 m = m3_scalar(1.0f);
    m.m[0][2] = translation.x;
    m.m[1][2] = translation.y;
    return m;
}

Matrix3 m3_make_rotation(float32 radians) {
    Matrix3 m = m3_scalar(1.0f);
    float32 c = cosf(radians);
    float32 s = sinf(radians);

    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

Matrix3 m3_make_scale(Vector2f32 scale) {
    Matrix3 m = m3_scalar(1.0f);
    m.m[0][0] = scale.x;
    m.m[1][1] = scale.y;
    return m;
}

Matrix3 m3_mul(Matrix3 a, Matrix3 b) {
    Matrix3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] +
                             a.m[i][1] * b.m[1][j] +
                             a.m[i][2] * b.m[2][j];
        }
    }
    return result;
}

inline Matrix3 m3_translate(Matrix3 m, Vector2f32 translation) {
    Matrix3 translation_matrix = m3_make_translation(translation);
    return m3_mul(m, translation_matrix);
}

inline Matrix3 m3_rotate(Matrix3 m, float32 radians) {
    Matrix3 rotation_matrix = m3_make_rotation(radians);
    return m3_mul(m, rotation_matrix);
}

inline Matrix3 m3_scale(Matrix3 m, Vector2f32 scale) {
    Matrix3 scale_matrix = m3_make_scale(scale);
    return m3_mul(m, scale_matrix);
}

Matrix3 m3_inverse(Matrix3 m) {
    Matrix3 inv;
    float32 det;

    inv.m[0][0] = m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1];
    inv.m[0][1] = m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2];
    inv.m[0][2] = m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1];

    inv.m[1][0] = m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2];
    inv.m[1][1] = m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0];
    inv.m[1][2] = m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2];

    inv.m[2][0] = m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0];
    inv.m[2][1] = m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1];
    inv.m[2][2] = m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];

    det = m.m[0][0] * inv.m[0][0] + m.m[0][1] * inv.m[1][0] + m.m[0][2] * inv.m[2][0];

    if (det == 0.0f) {
        return m3_scalar(0.0f);
    }

    det = 1.0f / det;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            inv.m[i][j] *= det;
        }
    }

    return inv;
}

Vector3f32 m3_transform(Matrix3 m, Vector3f32 v) {
    Vector3f32 result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;
    return result;
}
