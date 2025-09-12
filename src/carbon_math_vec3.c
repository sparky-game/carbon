// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v) {
  return CARBON_VEC3(u.x + v.x, u.y + v.y, u.z + v.z);
}

CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v) {
  return CARBON_VEC3(u.x - v.x, u.y - v.y, u.z - v.z);
}

CBN_Vec3 carbon_math_vec3_mult(CBN_Vec3 u, CBN_Vec3 v) {
  return CARBON_VEC3(u.x * v.x, u.y * v.y, u.z * v.z);
}

f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v) {
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v) {
  return (CBN_Vec3) {
    .x = u.y * v.z - u.z * v.y,
    .y = u.z * v.x - u.x * v.z,
    .z = u.x * v.y - u.y * v.x
  };
}

CBN_Vec3 carbon_math_vec3_scale(CBN_Vec3 v, f32 s) {
  return CARBON_VEC3(v.x * s, v.y * s, v.z * s);
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

CBN_Vec3 carbon_math_vec3_rotate(CBN_Vec3 v, CBN_Quat q) {
  CBN_Vec3 u = CARBON_VEC3_V(q);
  CBN_Vec3 t = carbon_math_vec3_scale(carbon_math_vec3_cross(u, v), 2);
  return carbon_math_vec3_add(v, carbon_math_vec3_add(carbon_math_vec3_scale(t, q.w), carbon_math_vec3_cross(u, t)));
}

u8 carbon_math_vec3_project_2d(CBN_Vec3 v, f32 near_z, CBN_Vec2 *out_v) {
  if (!out_v || v.z <= near_z) return false;
  *out_v = CARBON_VEC2(v.x/v.z, v.y/v.z);
  return true;
}
