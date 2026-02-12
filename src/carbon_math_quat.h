/*
  $$=========================$$
  ||       Math (Quat)       ||
  $$=========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a 4D vector that is used to encode 3D physical rotations.
 *
 * Is used to efficiently rotate an object about the (x, y, z) 3D vector
 * by the angle θ, where `w = cos(θ/2)`.
 */
typedef CBN_Vec4 CBN_Quat;
CBNDEF_AKA(cbn::math, Quat, CBN_Quat);

/**
 * @brief Creates a quaternion.
 */
#define carbon_math_quat carbon_math_vec4

/**
 * @brief Creates a quaternion which represents identity.
 * @return The newly created quaternion.
 */
CBNDEF CBN_Quat carbon_math_quat_id(void);

/**
 * @brief Returns a quaternion being the conjugate of the provided one.
 * @param q The quaternion.
 * @return The newly created quaternion.
 */
CBNDEF CBN_Quat carbon_math_quat_conj(CBN_Quat q);

/**
 * @brief Computes the Hamilton product between two quaternions.
 * @param p The first quaternion.
 * @param q The second quaternion.
 * @return The resultant quaternion.
 */
CBNDEF CBN_Quat carbon_math_quat_mult(CBN_Quat p, CBN_Quat q);

/**
 * @brief Computes the dot product between two quaternions.
 */
#define carbon_math_quat_dot carbon_math_vec4_dot

/**
 * @brief Converts an Euler angle rotation specified as a 3D vector to a quaternion.
 * @param v The 3D vector.
 * @return The resultant quaternion.
 */
CBNDEF CBN_Quat carbon_math_quat_from_euler(CBN_Vec3 v);

/**
 * @brief Creates a quaternion from the given 3D axis and angle.
 * @param axis The 3D axis.
 * @param angle The angle of rotation, in degrees.
 * @return The resultant quaternion.
 */
CBNDEF CBN_Quat carbon_math_quat_from_axis_angle(CBN_Vec3 axis, f32 angle);

/**
 * @brief Returns the string representation of the quaternion using default formatting.
 */
#define carbon_math_quat_to_cstr carbon_math_vec4_to_cstr

/**
 * @brief Scales the quaternion by the specified scalar value.
 */
#define carbon_math_quat_scale carbon_math_vec4_scale

/**
 * @brief Returns the length of the quaternion squared.
 * @param q The quaternion.
 * @return The quaternion's length squared.
 */
CBNDEF f32 carbon_math_quat_len_squared(CBN_Quat q);

/**
 * @brief Returns the length of the quaternion.
 * @param q The quaternion.
 * @return The quaternion's length.
 */
CBNDEF f32 carbon_math_quat_len(CBN_Quat q);

/**
 * @brief Returns a quaternion with the same direction as the specified one, but with a length of `1`.
 * @param q The quaternion.
 * @return The normalized quaternion.
 */
CBNDEF CBN_Quat carbon_math_quat_norm(CBN_Quat q);

/**
 * @brief Returns the inverse of the provided quaternion.
 * @param q The quaternion.
 * @return The inverted quaternion.
 */
CBNDEF CBN_Quat carbon_math_quat_inv(CBN_Quat q);
