/*
**  $$=========================$$
**  ||       Math (Vec4)       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Defines an inline 4D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @param z The value to assign to the Z field.
 * @param w The value to assign to the W field.
 */
#define CARBON_VEC4(x, y, z, w) (CBN_Vec4){{.c = {(f32)(x), (f32)(y), (f32)(z), (f32)(w)}}}

/**
 * @brief Defines an inline 4D vector.
 * @param x The value to assign to the X, Y, Z and W fields.
 */
#define CARBON_VEC4_1(x) CARBON_VEC4((x), (x), (x), (x))

/**
 * @brief Defines an inline 4D vector.
 * @param v The ≥2D vector to assign to the X, Y and Z fields.
 * @param w The value to assign to the W field.
 */
#define CARBON_VEC4_2(v, z, w) CARBON_VEC4((v).x, (v).y, (z), (w))

/**
 * @brief Defines an inline 4D vector.
 * @param v The ≥3D vector to assign to the X, Y and Z fields.
 * @param w The value to assign to the W field.
 */
#define CARBON_VEC4_3(v, w) CARBON_VEC4((v).x, (v).y, (v).z, (w))

/**
 * @brief Defines an inline 4D vector whose 4 elements are equal to zero.
 */
#define CARBON_VEC4_ZERO CARBON_VEC4_1(0)

/**
 * @brief Defines an inline 4D vector whose 4 elements are equal to one.
 */
#define CARBON_VEC4_ONE CARBON_VEC4_1(1)

/**
 * @brief Represents a 4D vector with four 32-bit floating-point (f32) values.
 */
struct CBN_Vec4_t {
  union {
    struct { f32 x, y, z, w; };
    struct { CBN_Vec2 xy; CBN_Vec2 zw; };
    struct { f32 __pad1; CBN_Vec2 yz; f32 __pad2; };
    struct { CBN_Vec3 xyz; f32 __pad3; };
    struct { f32 __pad4; CBN_Vec3 yzw; };
    f32 c[4];
  };
};

#ifdef __cplusplus
struct CBN_Vec4 : CBN_Vec4_t {
  /**
   * @see carbon_math_vec4_add
   */
  CBN_Vec4 operator+(const CBN_Vec4 &v) const;
  CBN_Vec4 operator+(f32 s) const;
  friend CBN_Vec4 operator+(f32 s, const CBN_Vec4 &v);
  CBN_Vec4 &operator+=(const CBN_Vec4 &v);
  CBN_Vec4 &operator+=(f32 s);
  /**
   * @see carbon_math_vec4_sub
   */
  CBN_Vec4 operator-(f32 s) const;
  friend CBN_Vec4 operator-(f32 s, const CBN_Vec4 &v);
  /**
   * @see carbon_math_vec4_mult
   */
  CBN_Vec4 operator*(const CBN_Vec4 &v) const;
  CBN_Vec4 operator/(const CBN_Vec4 &v) const;
  /**
   * @see carbon_math_vec4_scale
   */
  CBN_Vec4 operator-(void) const;
  CBN_Vec4 operator*(f32 s) const;
  friend CBN_Vec4 operator*(f32 s, const CBN_Vec4 &v);
  friend CBN_Vec4 operator/(f32 s, const CBN_Vec4 &v);
  /**
   * @see carbon_math_vec4_clamp
   */
  void Clamp(const CBN_Vec4 &min, const CBN_Vec4 &max);
  /**
   * @see carbon_math_vec4_abs
   */
  CBN_Vec4 Abs(void) const;
  /**
   * @see carbon_math_vec4_exp
   */
  CBN_Vec4 Exp(void) const;
  /**
   * @see carbon_math_vec4_sin
   */
  CBN_Vec4 Sin(void) const;
  /**
   * @see carbon_math_vec4_cos
   */
  CBN_Vec4 Cos(void) const;
  /**
   * @see carbon_math_vec4_tan
   */
  CBN_Vec4 Tan(void) const;
  /**
   * @see carbon_math_vec4_tanh
   */
  CBN_Vec4 Tanh(void) const;
  /**
   * @see carbon_math_vec4_lerp
   */
  CBN_Vec4 Lerp(const CBN_Vec4 &v, f32 t) const;
  /**
   * @see carbon_math_vec4_to_cstr
   */
  const char *ToString(void) const;
};
#else
typedef struct CBN_Vec4_t CBN_Vec4;
#endif
CBNDEF_T(cbn::math, Vec4, CBN_Vec4);

/**
 * @brief Adds two 4D vectors together (element-wise).
 * @param u The first 4D vector.
 * @param v The second 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_add(CBN_Vec4 u, CBN_Vec4 v);

/**
 * @brief Subtracts one 4D vector from another one (element-wise).
 * @param u The first 4D vector.
 * @param v The second 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_sub(CBN_Vec4 u, CBN_Vec4 v);

/**
 * @brief Computes the Hadamard product (element-wise multiplication) between two 4D vectors.
 * @param u The first 4D vector.
 * @param v The second 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_mult(CBN_Vec4 u, CBN_Vec4 v);

/**
 * @brief Computes the dot product between two 4D vectors.
 * @param u The first 4D vector.
 * @param v The second 4D vector.
 * @return The resultant scalar value.
 */
CBNDEF f32 carbon_math_vec4_dot(CBN_Vec4 u, CBN_Vec4 v);

/**
 * @brief Scales the 4D vector by the specified scalar value.
 * @param v The 4D vector.
 * @param s The scalar value.
 * @return The scaled 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_scale(CBN_Vec4 v, f32 s);

/**
 * @brief Restricts a 4D vector between a minimum and a maximum value.
 * @param v The 4D vector.
 * @param min The minimum value (as 4D vector).
 * @param max The maximum value (as 4D vector).
 * @return The restricted 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_clamp(CBN_Vec4 v, CBN_Vec4 min, CBN_Vec4 max);

/**
 * @brief Applies abs(x) function to the 4D vector (element-wise).
 * @param v The 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_abs(CBN_Vec4 v);

/**
 * @brief Applies e^x function to the 4D vector (element-wise).
 * @param v The 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_exp(CBN_Vec4 v);

/**
 * @brief Applies sin(x) function to the 4D vector (element-wise).
 * @param v The 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_sin(CBN_Vec4 v);

/**
 * @brief Applies cos(x) function to the 4D vector (element-wise).
 * @param v The 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_cos(CBN_Vec4 v);

/**
 * @brief Applies tan(x) function to the 4D vector (element-wise).
 * @param v The 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_tan(CBN_Vec4 v);

/**
 * @brief Applies tanh(x) function to the 4D vector (element-wise).
 * @param v The 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_tanh(CBN_Vec4 v);

/**
 * @brief Returns the string representation of the 4D vector using default formatting.
 * @param v The 4D vector.
 * @return The serialized 4D vector as `(X, Y, Z, W)`.
 */
CBNDEF char *carbon_math_vec4_to_cstr(CBN_Vec4 v);

/**
 * @brief Performs a linear interpolation between two 4D vectors based on the given weighting.
 * @param u The first 4D vector.
 * @param v The second 4D vector.
 * @param t A value [0..1] indicating the weight of `v`.
 * @return The interpolated 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_vec4_lerp(CBN_Vec4 u, CBN_Vec4 v, f32 t);

/**
 * @brief Projects a 4D point/vector into 3D Normalized Device Coordinates (NDC).
 *
 * It performs the perspective divide algorithm, mapping (x, y, z, w) in clip space
 * to (x/w, y/w, z/w) in NDC space. The resulting coordinates are in the [-1..1] range
 * for objects inside the canonical view frustum. Z is retained and transformed.
 * The result is valid only for points in front of the camera.
 *
 * @param v The 4D vector.
 * @param out_v The projected vector in 3D space as NDC coordinates (output argument pointer).
 * @return A boolean value indicating whether it projected the 4D vector successfully or not.
 */
CBNDEF bool carbon_math_vec4_project_3d(CBN_Vec4 v, CBN_Vec3 *out_v);

// Local Variables:
// mode: c++
// End:
