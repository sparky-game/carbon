/*
**  $$=========================$$
**  ||       Math (Mat4)       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a 4x4 matrix that is used to encode 3D physical transformations.
 */
typedef struct CBN_Mat4 {
  f32 items[4][4];
} CBN_Mat4;

/**
 * @brief Creates a zeroed-out 4x4 matrix.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_zero(void);

/**
 * @brief Creates an identity 4x4 matrix.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_id(void);

/**
 * @brief Multiplies two 4x4 matrices together.
 * @param a The first 4x4 matrix.
 * @param b The second 4x4 matrix.
 * @return The resultant 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_mult(CBN_Mat4 a, CBN_Mat4 b);

/**
 * @brief Multiplies a 4x4 matrix with a 3D vector (x, y, z, 1).
 * @param m The 4x4 matrix.
 * @param v The 3D vector.
 * @return The resultant 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_mat4_mult_vec3(CBN_Mat4 m, CBN_Vec3 v);

/**
 * @brief Multiplies a 4x4 matrix with a 4D vector.
 * @param m The 4x4 matrix.
 * @param v The 4D vector.
 * @return The resultant 4D vector.
 */
CBNDEF CBN_Vec4 carbon_math_mat4_mult_vec4(CBN_Mat4 m, CBN_Vec4 v);

/**
 * @brief Creates a 4x4 rotation matrix from the given quaternion.
 * @param q The quaternion.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_from_quat(CBN_Quat q);

/**
 * @brief Creates a 4x4 translation matrix from the given 3D position vector.
 * @param position The 3D vector.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_translation(CBN_Vec3 position);

/**
 * @brief Creates a 4x4 scale matrix from the given 3D scale vector.
 * @param scale The 3D vector.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_scale(CBN_Vec3 scale);

/**
 * @brief Creates a 4x4 model matrix from the given translation, rotation and scale (TRS).
 * @param t The 3D translation/position vector.
 * @param r The rotation quaternion.
 * @param s The 3D scale vector.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_model(CBN_Vec3 t, CBN_Quat r, CBN_Vec3 s);

/**
 * @brief Creates a 4x4 view matrix.
 * @param position The 3D camera position vector.
 * @param rotation The camera rotation quaternion.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_view(CBN_Vec3 position, CBN_Quat rotation);

/**
 * @brief Creates a 4x4 perspective projection matrix.
 * @param fov The Field-of-View (FOV) in degrees.
 * @param aspect The aspect ratio.
 * @param near The near clipping plane distance.
 * @param far The far clipping plane distance.
 * @return The 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_math_mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far);
