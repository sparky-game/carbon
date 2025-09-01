/*
**  $$=========================$$
**  ||       Math (Quat)       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Defines an inline quaternion.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @param z The value to assign to the Z field.
 * @param w The value to assign to the W field.
 */
#define CARBON_QUAT(x, y, z, w) (CBN_Quat){{(f32)(x), (f32)(y), (f32)(z), (f32)(w)}}

/**
 * @brief Represents a 4D vector that is used to encode 3D physical rotations.
 *
 * Is used to efficiently rotate an object about the (x, y, z) 3D vector
 * by the angle θ, where `w = cos(θ/2)`.
 */
typedef union CBN_Quat {
  f32 items[4];
  struct {
    union { f32 x, r, s; };
    union { f32 y, g, t; };
    union { f32 z, b, p, width; };
    union { f32 w, a, q, height; };
  };
} CBN_Quat;

/**
 * @brief Computes the Hamilton product between two quaternions.
 * @param p The first quaternion.
 * @param q The second quaternion.
 * @return The resultant quaternion.
 */
CARBON_API CBN_Quat carbon_math_quat_mult(CBN_Quat p, CBN_Quat q);

/**
 * @brief Computes the dot product between two quaternions.
 * @param p The first quaternion.
 * @param q The second quaternion.
 * @return The resultant scalar value.
 */
CARBON_API f32 carbon_math_quat_dot(CBN_Quat p, CBN_Quat q);

/**
 * @brief Converts an Euler angle rotation specified as a 3D vector to a quaternion.
 * @param v The 3D vector.
 * @return The resultant quaternion.
 */
CARBON_API CBN_Quat carbon_math_quat_from_euler(CBN_Vec3 v);
