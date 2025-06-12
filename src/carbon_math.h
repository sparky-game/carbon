/*
**  $$==================$$
**  ||       Math       ||
**  $$==================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_E 2.71828182845904523536
#define CARBON_E_0_5 1.64872127070012814684
#define CARBON_E_0_25 1.28402541668774148407
#define CARBON_E_0_125 1.13314845306682631682
#define CARBON_PI 3.14159265358979323846
#define CARBON_PI_2 (0.5 * CARBON_PI)
#define CARBON_2PI (2 * CARBON_PI)
#define CARBON_PI_4 (0.25 * CARBON_PI)
#define CARBON_4PI (4 * CARBON_PI)
#define CARBON_1_PI (1 / CARBON_PI)
#define CARBON_1_2PI (1 / CARBON_2PI)
#define CARBON_SQRT2 1.41421356237309504880
#define CARBON_1_SQRT2 (1 / CARBON_SQRT2)
#define CARBON_SQRT3 1.73205080756887729352
#define CARBON_1_SQRT3 (1 / CARBON_SQRT3)
#define CARBON_LOG2_E (1.4426950408889634074)
#define CARBON_LOG2_10 (3.32192809488736234789)
#define CARBON_I32_MAX 0x7fffffff
#define CARBON_RAND_MAX CARBON_I32_MAX
#define CARBON_F32_EPS 1.1920928955078125e-07
#define CARBON_MIN(x, y) (x < y ? x : y)
#define CARBON_MIN3(x, y, z) CARBON_MIN(x, CARBON_MIN(y, z))
#define CARBON_MAX(x, y) (x > y ? x : y)
#define CARBON_MAX3(x, y, z) CARBON_MAX(x, CARBON_MAX(y, z))
#define CARBON_CLAMP(x, min, max) ((x <= min) ? min : (x >= max) ? max : x)
#define CARBON_SIGN(x) (!x ? 0 : x < 0 ? -1 : 1)
#define CARBON_STEP(edge, x) (x < edge ? 0 : 1)
#define CARBON_SWAP(T, x, y) do { T z = x; x = y; y = z; } while (0)
#define CARBON_LERP(a, b, t) ((a) + ((b) - (a)) * (t))
// TODO: somehow perform bounds checking `CARBON_ASSERT(0 <= (i) && (i) < (m).rows && 0 <= (j) && (j) < (m).cols && "Matrix index out of bounds")`
#define CARBON_MAT_AT(m, i, j) (m).items[(i) * (m).cols + (j)]
// TODO: somehow perform bounds checking `CARBON_ASSERT(0 <= (i) && (i) < (r).cols && "Row index out of bounds")`
#define CARBON_ROW_AT(r, i) (r).items[(i)]
#define CARBON_MAT_PRINT(m) carbon_math_mat_print(m, #m)
#define CARBON_ROW_PRINT(r) carbon_math_row_print(r, #r)

/**
 * @brief Defines an inline 2D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 */
#define CARBON_VEC2(x, y) (CBN_Vec2){{(f32)(x), (f32)(y)}}

/**
 * @brief Defines an inline 2D vector.
 * @param x The value to assign to both the X and Y fields.
 */
#define CARBON_VEC2_1(x) CARBON_VEC2(x, x)

/**
 * @brief Defines an inline 2D vector (from polar coordinate system).
 * @param r The radial coordinate.
 * @param phi The angular coordinate.
 */
#define CARBON_VEC2_P(r, phi) CARBON_VEC2(r * cos(phi), r * sin(phi))

/**
 * @brief Defines an inline 2D vector whose 2 elements are equal to zero.
 */
#define CARBON_VEC2_ZERO CARBON_VEC2(0, 0)

/**
 * @brief Defines an inline 2D vector which represents -X direction.
 */
#define CARBON_VEC2_LEFT CARBON_VEC2(-1, 0)

/**
 * @brief Defines an inline 2D vector which represents +X direction.
 */
#define CARBON_VEC2_RIGHT CARBON_VEC2(1, 0)

/**
 * @brief Defines an inline 2D vector which represents -Y direction.
 */
#define CARBON_VEC2_DOWN CARBON_VEC2(0, -1)

/**
 * @brief Defines an inline 2D vector which represents +Y direction.
 */
#define CARBON_VEC2_UP CARBON_VEC2(0, 1)

/**
 * @brief Defines an inline 2D vector whose 2 elements are equal to one.
 */
#define CARBON_VEC2_ONE CARBON_VEC2(1, 1)

typedef union CBN_Vec3 CBN_Vec3;  // Forward declaration

/**
 * @brief Represents a 2D vector with two 32-bit floating-point (f32) values.
 */
typedef union CBN_Vec2 {
  f32 items[2];
  struct {
    union { f32 x, r, s, u; };
    union { f32 y, g, t, v; };
  };
#ifdef __cplusplus
  /**
   * @brief carbon_math_vec2_clamp
   * @param min The minimum value (as 2D vector).
   * @param max The maximum value (as 2D vector).
   */
  void Clamp(const CBN_Vec2 &min, const CBN_Vec2 &max);
  /**
   * @brief carbon_math_vec2_floor
   * @return The rounded down 2D vector.
   */
  CBN_Vec2 Floor(void) const;
  /**
   * @brief carbon_math_vec2_len
   * @return The 2D vector's length.
   */
  f32 Length(void) const;
  /**
   * @brief carbon_math_vec2_len_squared
   * @return The 2D vector's length squared.
   */
  f32 LengthSquared(void) const;
  /**
   * @brief carbon_math_vec2_norm
   * @return The normalized 2D vector.
   */
  CBN_Vec2 Normalize(void) const;
  /**
   * @brief carbon_math_vec2_lerp
   * @param v The other 2D vector.
   * @param t A value [0..1] indicating the weight of `v`.
   * @return The interpolated 2D vector.
   */
  CBN_Vec2 Lerp(const CBN_Vec2 &v, f32 t) const;
  /**
   * @brief carbon_math_vec2_rotate
   * @param angle The rotation to apply (in degrees).
   * @return The rotated 2D vector.
   */
  CBN_Vec2 Rotate(f32 angle) const;
  /**
   * @brief carbon_math_vec2_to_cstr
   * @return The serialized 2D vector as `(X, Y)`.
   */
  const char *ToString(void) const;
  // Overloaded Operators
  CBN_Vec2 operator+(const CBN_Vec2 &v) const;
  CBN_Vec2 operator+(const f32 s) const;
  friend CBN_Vec2 operator+(const f32 s, const CBN_Vec2 &v);
  void operator+=(const CBN_Vec2 &v);
  CBN_Vec2 operator-(void) const;
  CBN_Vec2 operator-(const CBN_Vec2 &v) const;
  CBN_Vec2 operator-(const f32 s) const;
  friend CBN_Vec2 operator-(const f32 s, const CBN_Vec2 &v);
  void operator-=(const CBN_Vec2 &v);
  f32 operator*(const CBN_Vec2 &v) const;
  CBN_Vec2 operator*(const f32 s) const;
  friend CBN_Vec2 operator*(const f32 s, const CBN_Vec2 &v);
  void operator*=(const f32 s);
  CBN_Vec2 operator/(const f32 s) const;
  void operator/=(const f32 s);
  // Swizzling Operations
  constexpr auto xx(void)  const;
  constexpr auto xy(void)  const;
  constexpr auto xz(void)  const;
  constexpr auto yx(void)  const;
  constexpr auto yy(void)  const;
  constexpr auto yz(void)  const;
  constexpr auto zx(void)  const;
  constexpr auto zy(void)  const;
  constexpr auto zz(void)  const;
  constexpr auto xxx(void) const;
  constexpr auto xxy(void) const;
  constexpr auto xxz(void) const;
  constexpr auto xyx(void) const;
  constexpr auto xyy(void) const;
  constexpr auto xyz(void) const;
  constexpr auto xzx(void) const;
  constexpr auto xzy(void) const;
  constexpr auto xzz(void) const;
  constexpr auto yxx(void) const;
  constexpr auto yxy(void) const;
  constexpr auto yxz(void) const;
  constexpr auto yyx(void) const;
  constexpr auto yyy(void) const;
  constexpr auto yyz(void) const;
  constexpr auto yzx(void) const;
  constexpr auto yzy(void) const;
  constexpr auto yzz(void) const;
  constexpr auto zxx(void) const;
  constexpr auto zxy(void) const;
  constexpr auto zxz(void) const;
  constexpr auto zyx(void) const;
  constexpr auto zyy(void) const;
  constexpr auto zyz(void) const;
  constexpr auto zzx(void) const;
  constexpr auto zzy(void) const;
  constexpr auto zzz(void) const;
#endif
} CBN_Vec2;

/**
 * @brief Defines an inline 3D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @param z The value to assign to the Z field.
 */
#define CARBON_VEC3(x, y, z) (CBN_Vec3){{(f32)(x), (f32)(y), (f32)(z)}}

/**
 * @brief Defines an inline 3D vector.
 * @param x The value to assign to the X, Y and Z fields.
 */
#define CARBON_VEC3_1(x) CARBON_VEC3(x, x, x)

/**
 * @brief Defines an inline 3D vector.
 * @param v The 2D vector to assign to the X and Y fields.
 * @param z The value to assign to the Z field.
 */
#define CARBON_VEC3_V(v, z) CARBON_VEC3((v).x, (v).y, z)

/**
 * @brief Defines an inline 3D vector whose 3 elements are equal to zero.
 */
#define CARBON_VEC3_ZERO CARBON_VEC3(0, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents -X direction.
 */
#define CARBON_VEC3_LEFT CARBON_VEC3(-1, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents +X direction.
 */
#define CARBON_VEC3_RIGHT CARBON_VEC3(1, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents -Y direction.
 */
#define CARBON_VEC3_DOWN CARBON_VEC3(0, -1, 0)

/**
 * @brief Defines an inline 3D vector which represents +Y direction.
 */
#define CARBON_VEC3_UP CARBON_VEC3(0, 1, 0)

/**
 * @brief Defines an inline 3D vector which represents -Z direction.
 */
#define CARBON_VEC3_BACK CARBON_VEC3(0, 0, -1)

/**
 * @brief Defines an inline 3D vector which represents +Z direction.
 */
#define CARBON_VEC3_FORWARD CARBON_VEC3(0, 0, 1)

/**
 * @brief Defines an inline 3D vector whose 3 elements are equal to one.
 */
#define CARBON_VEC3_ONE CARBON_VEC3(1, 1, 1)

/**
 * @brief Represents a 3D vector with three 32-bit floating-point (f32) values.
 */
typedef union CBN_Vec3 {
  f32 items[3];
  struct {
    union { f32 x, r, s, u; };
    union { f32 y, g, t, v; };
    union { f32 z, b, p, w; };
  };
#ifdef __cplusplus
  /**
   * @brief carbon_math_vec3_to_cstr
   * @return The serialized 3D vector as `(X, Y, Z)`.
   */
  const char *ToString(void) const;
  // Overloaded Operators
  CBN_Vec3 operator+(const CBN_Vec3 &v) const;
  CBN_Vec3 operator-(const CBN_Vec3 &v) const;
  f32 operator*(const CBN_Vec3 &v) const;
  // Swizzling Operations
  constexpr auto xx(void)  const;
  constexpr auto xy(void)  const;
  constexpr auto xz(void)  const;
  constexpr auto yx(void)  const;
  constexpr auto yy(void)  const;
  constexpr auto yz(void)  const;
  constexpr auto zx(void)  const;
  constexpr auto zy(void)  const;
  constexpr auto zz(void)  const;
  constexpr auto xxx(void) const;
  constexpr auto xxy(void) const;
  constexpr auto xxz(void) const;
  constexpr auto xyx(void) const;
  constexpr auto xyy(void) const;
  constexpr auto xyz(void) const;
  constexpr auto xzx(void) const;
  constexpr auto xzy(void) const;
  constexpr auto xzz(void) const;
  constexpr auto yxx(void) const;
  constexpr auto yxy(void) const;
  constexpr auto yxz(void) const;
  constexpr auto yyx(void) const;
  constexpr auto yyy(void) const;
  constexpr auto yyz(void) const;
  constexpr auto yzx(void) const;
  constexpr auto yzy(void) const;
  constexpr auto yzz(void) const;
  constexpr auto zxx(void) const;
  constexpr auto zxy(void) const;
  constexpr auto zxz(void) const;
  constexpr auto zyx(void) const;
  constexpr auto zyy(void) const;
  constexpr auto zyz(void) const;
  constexpr auto zzx(void) const;
  constexpr auto zzy(void) const;
  constexpr auto zzz(void) const;
#endif
} CBN_Vec3;

// Vector Swizzling Operations
#define CARBON_VEC_xx(v)  CARBON_VEC2((v).x, (v).x)
#define CARBON_VEC_xy(v)  CARBON_VEC2((v).x, (v).y)
#define CARBON_VEC_xz(v)  CARBON_VEC2((v).x, (v).z)
#define CARBON_VEC_yx(v)  CARBON_VEC2((v).y, (v).x)
#define CARBON_VEC_yy(v)  CARBON_VEC2((v).y, (v).y)
#define CARBON_VEC_yz(v)  CARBON_VEC2((v).y, (v).z)
#define CARBON_VEC_zx(v)  CARBON_VEC2((v).z, (v).x)
#define CARBON_VEC_zy(v)  CARBON_VEC2((v).z, (v).y)
#define CARBON_VEC_zz(v)  CARBON_VEC2((v).z, (v).z)
#define CARBON_VEC_xxx(v) CARBON_VEC3((v).x, (v).x, (v).x)
#define CARBON_VEC_xxy(v) CARBON_VEC3((v).x, (v).x, (v).y)
#define CARBON_VEC_xxz(v) CARBON_VEC3((v).x, (v).x, (v).z)
#define CARBON_VEC_xyx(v) CARBON_VEC3((v).x, (v).y, (v).x)
#define CARBON_VEC_xyy(v) CARBON_VEC3((v).x, (v).y, (v).y)
#define CARBON_VEC_xyz(v) CARBON_VEC3((v).x, (v).y, (v).z)
#define CARBON_VEC_xzx(v) CARBON_VEC3((v).x, (v).z, (v).x)
#define CARBON_VEC_xzy(v) CARBON_VEC3((v).x, (v).z, (v).y)
#define CARBON_VEC_xzz(v) CARBON_VEC3((v).x, (v).z, (v).z)
#define CARBON_VEC_yxx(v) CARBON_VEC3((v).y, (v).x, (v).x)
#define CARBON_VEC_yxy(v) CARBON_VEC3((v).y, (v).x, (v).y)
#define CARBON_VEC_yxz(v) CARBON_VEC3((v).y, (v).x, (v).z)
#define CARBON_VEC_yyx(v) CARBON_VEC3((v).y, (v).y, (v).x)
#define CARBON_VEC_yyy(v) CARBON_VEC3((v).y, (v).y, (v).y)
#define CARBON_VEC_yyz(v) CARBON_VEC3((v).y, (v).y, (v).z)
#define CARBON_VEC_yzx(v) CARBON_VEC3((v).y, (v).z, (v).x)
#define CARBON_VEC_yzy(v) CARBON_VEC3((v).y, (v).z, (v).y)
#define CARBON_VEC_yzz(v) CARBON_VEC3((v).y, (v).z, (v).z)
#define CARBON_VEC_zxx(v) CARBON_VEC3((v).z, (v).x, (v).x)
#define CARBON_VEC_zxy(v) CARBON_VEC3((v).z, (v).x, (v).y)
#define CARBON_VEC_zxz(v) CARBON_VEC3((v).z, (v).x, (v).z)
#define CARBON_VEC_zyx(v) CARBON_VEC3((v).z, (v).y, (v).x)
#define CARBON_VEC_zyy(v) CARBON_VEC3((v).z, (v).y, (v).y)
#define CARBON_VEC_zyz(v) CARBON_VEC3((v).z, (v).y, (v).z)
#define CARBON_VEC_zzx(v) CARBON_VEC3((v).z, (v).z, (v).x)
#define CARBON_VEC_zzy(v) CARBON_VEC3((v).z, (v).z, (v).y)
#define CARBON_VEC_zzz(v) CARBON_VEC3((v).z, (v).z, (v).z)

/**
 * @brief Defines an inline rectangle.
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 */
#define CARBON_RECT(x, y, w, h) (CBN_Rect){(f32)(x), (f32)(y), (f32)(w), (f32)(h)}

/**
 * @brief Defines an inline rectangle.
 * @param v The 2D vector to assign to the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 */
#define CARBON_RECT_V(v, w, h) CARBON_RECT(v.x, v.y, w, h)

/**
 * @brief Defines an inline rectangle whose width and height are equal (square).
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param sz The value to assign to both the width and the height of the size.
 */
#define CARBON_RECT_SQUARE(x, y, sz) CARBON_RECT(x, y, sz, sz)

/**
 * @brief Defines an inline rectangle whose width and height are equal (square).
 * @param v The 2D vector to assign to the position.
 * @param sz The value to assign to both the width and the height of the size.
 */
#define CARBON_RECT_SQUARE_V(v, sz) CARBON_RECT_SQUARE(v.x, v.y, sz)

/**
 * @brief Represents the position and size of a rectangle with four 32-bit floating-point (f32) values.
 */
typedef struct CBN_Rect {
  f32 x, y;
  f32 w, h;
#ifdef __cplusplus
  /**
   * @brief carbon_math_rect_contains_point
   * @param p The point to check.
   * @return A boolean value representing whether the point is contained within this rectangle or not.
   */
  u8 Contains(const CBN_Vec2 &p) const;
  /**
   * @brief carbon_math_rect_detect_collision
   * @param r The other rectangle to check with.
   * @return A boolean value representing whether they overlap (also touching).
   */
  u8 Overlaps(const CBN_Rect &r) const;
  /**
   * @brief carbon_math_rect_scale
   * @param s The scale factor value.
   */
  void Scale(const f32 s);
#endif
} CBN_Rect;

typedef struct {
  f32 *items;
  usz rows;
  usz cols;
} CBN_Matrix;

typedef struct {
  f32 *items;
  usz cols;
} CBN_Row;

CARBON_API void carbon_math_srand(u64 seed);
CARBON_API i32 carbon_math_rand(void);
CARBON_API i32 carbon_math_rand_between(i32 min, i32 max);
CARBON_API f32 carbon_math_randf(void);
CARBON_API f32 carbon_math_randf_between(f32 min, f32 max);
CARBON_API void carbon_math_mt19937_64_srand(u64 seed);
CARBON_API u64 carbon_math_mt19937_64_rand(void);
CARBON_API f32 carbon_math_abs(f32 x);
CARBON_API f32 carbon_math_round(f32 x);
CARBON_API f32 carbon_math_floor(f32 x);
CARBON_API f32 carbon_math_ceil(f32 x);
CARBON_API f32 carbon_math_snap(f32 x, f32 dx);
CARBON_API f32 carbon_math_sqrt(f32 x);
CARBON_API i32 carbon_math_imod(i32 x, i32 y);
CARBON_API f32 carbon_math_fmod(f32 x, f32 y);
CARBON_API f32 carbon_math_pow(f32 x, f32 y);
CARBON_API f32 carbon_math_log2(f32 x);
CARBON_API f32 carbon_math_exp2(f32 x);
CARBON_API f32 carbon_math_ldexp2(f32 x, i32 exp);
CARBON_API f32 carbon_math_log(f32 x);
CARBON_API f32 carbon_math_exp(f32 x);
CARBON_API f32 carbon_math_ldexp(f32 x, i32 exp);
CARBON_API f32 carbon_math_log10(f32 x);
CARBON_API f32 carbon_math_exp10(f32 x);
CARBON_API f32 carbon_math_ldexp10(f32 x, i32 exp);
CARBON_API f32 carbon_math_frexp(f32 x, i32 *exp);
CARBON_API f32 carbon_math_sigmoid(f32 x);
CARBON_API f32 carbon_math_tanh(f32 x);
CARBON_API f32 carbon_math_smoothstep(f32 a, f32 b, f32 t);
CARBON_API i8 carbon_math_cmp(f32 x, f32 y);

/**
 * @brief Concatenates two unsigned integers together (e.g. `10` || `69` :: `1069`).
 * @param x The first unsigned integer.
 * @param y The second unsigned integer.
 * @return The unsigned integer representing their concatenation.
 */
CARBON_API u64 carbon_math_concat(u64 x, u64 y);

CARBON_API f32 carbon_math_sin(f32 x);
CARBON_API f32 carbon_math_cos(f32 x);
CARBON_API f32 carbon_math_asin(f32 x);
CARBON_API f32 carbon_math_atan(f32 x);
CARBON_API f32 carbon_math_atan2(f32 y, f32 x);

CARBON_API CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v);
CARBON_API CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v);
CARBON_API CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v);
CARBON_API CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v);
CARBON_API f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v);
CARBON_API f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v);
CARBON_API CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Restricts a 2D vector between a minimum and a maximum value.
 * @param v The 2D vector.
 * @param min The minimum value (as 2D vector).
 * @param max The maximum value (as 2D vector).
 * @return The restricted 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_clamp(CBN_Vec2 v, CBN_Vec2 min, CBN_Vec2 max);

/**
 * @brief Rounds down the components of the 2D vector.
 * @param v The 2D vector.
 * @return The rounded down 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_floor(CBN_Vec2 v);

/**
 * @brief Returns the length of the 2D vector squared.
 * @param v The 2D vector.
 * @return The 2D vector's length squared.
 */
CARBON_API f32 carbon_math_vec2_len_squared(CBN_Vec2 v);

/**
 * @brief Returns the length of the 2D vector.
 * @param v The 2D vector.
 * @return The 2D vector's length.
 */
CARBON_API f32 carbon_math_vec2_len(CBN_Vec2 v);

/**
 * @brief Returns a 2D vector with the same direction as the specified one, but with a length of `1`.
 * @param v The 2D vector.
 * @return The normalized 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_norm(CBN_Vec2 v);

/**
 * @brief Performs a linear interpolation between two 2D vectors based on the given weighting.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @param t A value [0..1] indicating the weight of `v`.
 * @return The interpolated 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_lerp(CBN_Vec2 u, CBN_Vec2 v, f32 t);

/**
 * @brief Scales the 2D vector by the specified scalar value.
 * @param v The 2D vector.
 * @param s The scalar value.
 * @return The scaled 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_scale(CBN_Vec2 v, f32 s);

/**
 * @brief Rotates the 2D vector by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @return The rotated 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle);

/**
 * @brief Rotates the 2D vector around a given pivot point by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @param pivot The pivot point around which to rotate the 2D vector.
 * @return The rotated 2D vector.
 */
CARBON_API CBN_Vec2 carbon_math_vec2_rotate_around_pivot(CBN_Vec2 v, f32 angle, CBN_Vec2 pivot);

/**
 * @brief Returns the string representation of the 2D vector using default formatting.
 * @param v The 2D vector.
 * @return The serialized 2D vector as `(X, Y)`.
 */
CARBON_API char *carbon_math_vec2_to_cstr(CBN_Vec2 v);

/**
 * @brief Returns the string representation of the 3D vector using default formatting.
 * @param v The 3D vector.
 * @return The serialized 3D vector as `(X, Y, Z)`.
 */
CARBON_API char *carbon_math_vec3_to_cstr(CBN_Vec3 v);

/**
 * @brief Determines whether the specified point is contained within a rectangle.
 * @param r The rectangle.
 * @param p The point to check.
 * @return A boolean value representing whether the point is contained within this rectangle or not.
 */
CARBON_API u8 carbon_math_rect_contains_point(CBN_Rect r, CBN_Vec2 p);

/**
 * @brief Determines whether the rectangles collide/overlap/intersect with each other.
 * @param r1 The first rectangle.
 * @param r2 The second rectangle.
 * @return A boolean value representing whether they overlap (also touching).
 */
CARBON_API u8 carbon_math_rect_detect_collision(CBN_Rect r1, CBN_Rect r2);

/**
 * @brief Scales the rectangle by the specified value.
 * @param r The rectangle.
 * @param s The scale factor value.
 */
CARBON_API void carbon_math_rect_scale(CBN_Rect *r, const f32 s);

CARBON_API CBN_Matrix carbon_math_mat_create(usz rows, usz cols);
CARBON_API void carbon_math_mat_destroy(CBN_Matrix *m);
CARBON_API void carbon_math_mat_fill(CBN_Matrix m, f32 x);
CARBON_API void carbon_math_mat_rand(CBN_Matrix m, f32 min, f32 max);
CARBON_API CBN_Row carbon_math_mat_row(CBN_Matrix m, usz row);
CARBON_API void carbon_math_mat_copy(CBN_Matrix dst, CBN_Matrix src);
CARBON_API void carbon_math_mat_add(CBN_Matrix dst, CBN_Matrix m);
CARBON_API void carbon_math_mat_dot(CBN_Matrix dst, CBN_Matrix a, CBN_Matrix b);
CARBON_API void carbon_math_mat_map(CBN_Matrix m, f32 (*f)(f32));
CARBON_API void carbon_math_mat_print(CBN_Matrix m, const char *name);
CARBON_API CBN_Row carbon_math_row_create(usz cols);
CARBON_API void carbon_math_row_destroy(CBN_Row *r);
CARBON_API CBN_Matrix carbon_math_row_to_mat(CBN_Row r);
CARBON_API CBN_Row carbon_math_row_slice(CBN_Row r, usz i, usz cols);
CARBON_API void carbon_math_row_fill(CBN_Row r, f32 x);
CARBON_API void carbon_math_row_rand(CBN_Row r, f32 min, f32 max);
CARBON_API void carbon_math_row_copy(CBN_Row dst, CBN_Row src);
CARBON_API void carbon_math_row_print(CBN_Row r, const char *name);

// Local Variables:
// mode: c++
// End:
