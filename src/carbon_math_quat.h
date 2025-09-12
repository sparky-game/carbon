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
 * @brief Defines an inline quaternion.
 * @param v The 3D vector to assign to the X, Y and Z fields.
 * @param w The value to assign to the W field.
 */
#define CARBON_QUAT_3(v, w) CARBON_VEC4_3(v, w)

/**
 * @brief Defines an inline quaternion being the conjugate of the provided one.
 * @param q The quaternion.
 */
#define CARBON_QUAT_C(q) CARBON_QUAT(-(q).x, -(q).y, -(q).z, (q).w)

/**
 * @brief Defines an inline quaternion which represents identity.
 */
#define CARBON_QUAT_ID CARBON_QUAT(0, 0, 0, 1)

/**
 * @brief Represents a 4D vector that is used to encode 3D physical rotations.
 *
 * Is used to efficiently rotate an object about the (x, y, z) 3D vector
 * by the angle θ, where `w = cos(θ/2)`.
 */
typedef CBN_Vec4 CBN_Quat;

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

/**
 * @brief Creates a quaternion from the given 3D axis and angle.
 * @param axis The 3D axis.
 * @param angle The angle of rotation.
 * @return The resultant quaternion.
 */
CARBON_API CBN_Quat carbon_math_quat_from_axis_angle(CBN_Vec3 axis, f32 angle);

/**
 * @brief Returns the string representation of the quaternion using default formatting.
 * @param q The quaternion.
 * @return The serialized quaternion as `(X, Y, Z, W)`.
 */
CARBON_API char *carbon_math_quat_to_cstr(CBN_Quat q);

/**
 * @brief Scales the quaternion by the specified scalar value.
 * @param q The quaternion.
 * @param s The scalar value.
 * @return The scaled quaternion.
 */
CARBON_API CBN_Quat carbon_math_quat_scale(CBN_Quat q, f32 s);

/**
 * @brief Returns the length of the quaternion squared.
 * @param q The quaternion.
 * @return The quaternion's length squared.
 */
CARBON_API f32 carbon_math_quat_len_squared(CBN_Quat q);

/**
 * @brief Returns the length of the quaternion.
 * @param q The quaternion.
 * @return The quaternion's length.
 */
CARBON_API f32 carbon_math_quat_len(CBN_Quat q);

/**
 * @brief Returns a quaternion with the same direction as the specified one, but with a length of `1`.
 * @param q The quaternion.
 * @return The normalized quaternion.
 */
CARBON_API CBN_Quat carbon_math_quat_norm(CBN_Quat q);

/**
 * @brief Returns the inverse of the provided quaternion.
 * @param q The quaternion.
 * @return The inverted quaternion.
 */
CARBON_API CBN_Quat carbon_math_quat_inv(CBN_Quat q);
