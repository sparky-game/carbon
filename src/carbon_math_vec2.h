/*
**  $$=========================$$
**  ||       Math (Vec2)       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

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
#define CARBON_VEC2_P(r, phi) CARBON_VEC2(r * carbon_math_cos(phi), r * carbon_math_sin(phi))

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
  constexpr auto yx(void)  const;
  constexpr auto yy(void)  const;
  constexpr auto xxx(void) const;
  constexpr auto xxy(void) const;
  constexpr auto xyx(void) const;
  constexpr auto xyy(void) const;
  constexpr auto yxx(void) const;
  constexpr auto yxy(void) const;
  constexpr auto yyx(void) const;
  constexpr auto yyy(void) const;
#endif
} CBN_Vec2;

// Swizzling Operations
#define CARBON_VEC_xx(v)  CARBON_VEC2((v).x, (v).x)
#define CARBON_VEC_xy(v)  CARBON_VEC2((v).x, (v).y)
#define CARBON_VEC_yx(v)  CARBON_VEC2((v).y, (v).x)
#define CARBON_VEC_yy(v)  CARBON_VEC2((v).y, (v).y)
#define CARBON_VEC_xxx(v) CARBON_VEC3((v).x, (v).x, (v).x)
#define CARBON_VEC_xxy(v) CARBON_VEC3((v).x, (v).x, (v).y)
#define CARBON_VEC_xyx(v) CARBON_VEC3((v).x, (v).y, (v).x)
#define CARBON_VEC_xyy(v) CARBON_VEC3((v).x, (v).y, (v).y)
#define CARBON_VEC_yxx(v) CARBON_VEC3((v).y, (v).x, (v).x)
#define CARBON_VEC_yxy(v) CARBON_VEC3((v).y, (v).x, (v).y)
#define CARBON_VEC_yyx(v) CARBON_VEC3((v).y, (v).y, (v).x)
#define CARBON_VEC_yyy(v) CARBON_VEC3((v).y, (v).y, (v).y)

CARBON_API CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v);

CARBON_API CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v);

CARBON_API f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v);

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

// Local Variables:
// mode: c++
// End:
