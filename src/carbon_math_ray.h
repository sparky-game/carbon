/*
  $$========================$$
  ||       Math (Ray)       ||
  $$========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents an infinite 3D line starting at origin and going in some direction.
 */
CBNDEF_T(CBN_Ray) {
  CBN_Vec3 origin;
  CBN_Vec3 direction;
};
CBNDEF_TAKA(cbn::math, Ray, CBN_Ray);

/**
 * @brief Computes a point along the 3D ray at a given distance from its origin.
 * @param r The 3D ray.
 * @param distance The scalar distance along the ray's direction vector.
 * @return The computed 3D point in space.
 */
CBNDEF CBN_Vec3 carbon_math_ray_get_point(CBN_Ray r, f32 distance);

/**
 * @brief Returns the string representation of the 3D ray using default formatting.
 * @param r The 3D ray.
 * @return The serialized 3D ray as `(Ori, Dir)`.
 */
CBNDEF char *carbon_math_ray_to_cstr(CBN_Ray r);
