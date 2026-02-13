/*
  $$=========================$$
  ||       Math (Vec2)       ||
  $$=========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a 2D vector with two 32-bit floating-point (f32) values.
 */
CBNDEF_T(CBN_Vec2) {
  union {
    struct { f32 x, y; };
    f32 c[2];
  };
};
CBNDEF_AKA(cbn::math, Vec2, CBN_Vec2);

/**
 * @brief Creates a 2D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @return The newly created 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2(f32 x, f32 y);

/**
 * @brief Creates a 2D vector.
 * @param x The value to assign to both the X and Y fields.
 * @return The newly created 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_1(f32 x);

/**
 * @brief Creates a 2D vector (from polar coordinate system).
 * @param r The radial distance.
 * @param phi The polar angle in radians.
 * @return The newly created 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_p(f32 r, f32 phi);

/**
 * @brief Adds two 2D vectors together (element-wise).
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Subtracts one 2D vector from another one (element-wise).
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Computes the Hadamard product (element-wise multiplication) between two 2D vectors.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_mult(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Computes the dot product between two 2D vectors.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant scalar value.
 */
CBNDEF f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Scales the 2D vector by the specified scalar value.
 * @param v The 2D vector.
 * @param s The scalar value.
 * @return The scaled 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_scale(CBN_Vec2 v, f32 s);

/**
 * @brief Returns the length of the 2D vector squared.
 * @param v The 2D vector.
 * @return The 2D vector's length squared.
 */
CBNDEF f32 carbon_math_vec2_len_squared(CBN_Vec2 v);

/**
 * @brief Returns the length of the 2D vector.
 * @param v The 2D vector.
 * @return The 2D vector's length.
 */
CBNDEF f32 carbon_math_vec2_len(CBN_Vec2 v);

/**
 * @brief Returns a 2D vector with the same direction as the specified one, but with a length of `1`.
 * @param v The 2D vector.
 * @return The normalized 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_norm(CBN_Vec2 v);

/**
 * @brief Restricts a 2D vector between a minimum and a maximum value.
 * @param v The 2D vector.
 * @param min The minimum value (as 2D vector).
 * @param max The maximum value (as 2D vector).
 * @return The restricted 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_clamp(CBN_Vec2 v, CBN_Vec2 min, CBN_Vec2 max);

/**
 * @brief Applies floor(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_floor(CBN_Vec2 v);

/**
 * @brief Applies abs(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_abs(CBN_Vec2 v);

/**
 * @brief Applies e^x function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_exp(CBN_Vec2 v);

/**
 * @brief Applies sin(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_sin(CBN_Vec2 v);

/**
 * @brief Applies cos(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_cos(CBN_Vec2 v);

/**
 * @brief Applies tan(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_tan(CBN_Vec2 v);

/**
 * @brief Applies tanh(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_tanh(CBN_Vec2 v);

/**
 * @brief Performs a linear interpolation between two 2D vectors based on the given weighting.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @param t A value [0..1] indicating the weight of `v`.
 * @return The interpolated 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_lerp(CBN_Vec2 u, CBN_Vec2 v, f32 t);

/**
 * @brief Rotates the 2D vector by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @return The rotated 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle);

/**
 * @brief Rotates the 2D vector around a given pivot point by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @param pivot The pivot point around which to rotate the 2D vector.
 * @return The rotated 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_rotate_around_pivot(CBN_Vec2 v, f32 angle, CBN_Vec2 pivot);

/**
 * @brief Returns the string representation of the 2D vector using default formatting.
 * @param v The 2D vector.
 * @return The serialized 2D vector as `(X, Y)`.
 */
CBNDEF char *carbon_math_vec2_to_cstr(CBN_Vec2 v);
