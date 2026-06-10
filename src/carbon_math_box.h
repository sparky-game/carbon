/*
  $$========================$$
  ||       Math (Box)       ||
  $$========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents ...
 */
CBNDEF_T(CBN_Box) {
  union {
    struct { f32 x, y, z; };
    CBN_Vec3 xyz;
  };
  union {
    struct { f32 w, h, d; };
    CBN_Vec3 whd;
  };
};
CBNDEF_TAKA(cbn::math, Box, CBN_Box);

/**
 */
CBNDEF bool carbon_math_box_ray_intersects(CBN_Box b, CBN_Ray r, f32 *t);
