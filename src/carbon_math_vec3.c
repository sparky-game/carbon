// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v) {
  return (CBN_Vec3) {
    .x = u.x + v.x,
    .y = u.y + v.y,
    .z = u.z + v.z
  };
}

CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v) {
  return (CBN_Vec3) {
    .x = u.x - v.x,
    .y = u.y - v.y,
    .z = u.z - v.z
  };
}

f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v) {
  return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v) {
  return (CBN_Vec3) {
    .x = (u.y * v.z) - (u.z * v.y),
    .y = (u.z * v.x) - (u.x * v.z),
    .z = (u.x * v.y) - (u.y * v.x)
  };
}

char *carbon_math_vec3_to_cstr(CBN_Vec3 v) {
  return carbon_string_fmt("(%.3f, %.3f, %.3f)", v.x, v.y, v.z);
}

CBN_Vec3 carbon_math_vec3_rotate_x(CBN_Vec3 v, f32 angle) {
  CBN_Vec2 r = carbon_math_vec2_rotate(CARBON_VEC_yz(v), angle);
  return CARBON_VEC3(v.x, r.x, r.y);
}

CBN_Vec3 carbon_math_vec3_rotate_y(CBN_Vec3 v, f32 angle) {
  CBN_Vec2 r = carbon_math_vec2_rotate(CARBON_VEC_xz(v), angle);
  return CARBON_VEC3(r.x, v.y, r.y);
}

CBN_Vec3 carbon_math_vec3_rotate_z(CBN_Vec3 v, f32 angle) {
  CBN_Vec2 r = carbon_math_vec2_rotate(CARBON_VEC_xy(v), angle);
  return CARBON_VEC3(r.x, r.y, v.z);
}

CBN_Vec2 carbon_math_vec3_project_2d(CBN_Vec3 v) {
  if (v.z < 0) v.z = -v.z;
  if (v.z < CARBON_EPS) v.z += CARBON_EPS;
  return CARBON_VEC2(v.x / v.z, v.y / v.z);
}
