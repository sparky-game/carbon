// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

CBN_Vec3 carbon_math_ray_get_point(CBN_Ray r, f32 distance) {
  return carbon_math_vec3_add(r.origin, carbon_math_vec3_scale(r.direction, distance));
}

char *carbon_math_ray_to_cstr(CBN_Ray r) {
  return carbon_string_fmt("(%s, %s)",
                           carbon_math_vec3_to_cstr(r.origin),
                           carbon_math_vec3_to_cstr(r.direction));
}
