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
#define CARBON_VEC4(x, y, z, w) (CBN_Vec4){{(f32)(x), (f32)(y), (f32)(z), (f32)(w)}}

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
 * @brief Represents a 4D vector with four 32-bit floating-point (f32) values.
 */
typedef union CBN_Vec4 {
  f32 items[4];
  struct {
    union { f32 x, r; };
    union { f32 y, g; };
    union { f32 z, b; };
    union { f32 w, a; };
  };
#ifdef __cplusplus
  /**
   * @see carbon_math_vec4_to_cstr
   */
  const char *ToString(void) const;
#endif
} CBN_Vec4;

/**
 * @brief Returns the string representation of the 4D vector using default formatting.
 * @param v The 4D vector.
 * @return The serialized 4D vector as `(X, Y, Z, W)`.
 */
CARBON_API char *carbon_math_vec4_to_cstr(CBN_Vec4 v);

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
CARBON_API bool carbon_math_vec4_project_3d(CBN_Vec4 v, CBN_Vec3 *out_v);
