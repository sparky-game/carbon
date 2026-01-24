// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec3 carbon_math_vec3(f32 x, f32 y, f32 z) {
  return (CBN_Vec3){.c = {x, y, z}};
}

CBN_Vec3 carbon_math_vec3_1(f32 x) {
  return carbon_math_vec3(x, x, x);
}

CBN_Vec3 carbon_math_vec3_2(CBN_Vec2 v, f32 z) {
  return carbon_math_vec3(v.x, v.y, z);
}

CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v) {
  return carbon_math_vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v) {
  return carbon_math_vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

CBN_Vec3 carbon_math_vec3_mult(CBN_Vec3 u, CBN_Vec3 v) {
  return carbon_math_vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v) {
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v) {
  return carbon_math_vec3(u.y * v.z - u.z * v.y,
                          u.z * v.x - u.x * v.z,
                          u.x * v.y - u.y * v.x);
}

CBN_Vec3 carbon_math_vec3_scale(CBN_Vec3 v, f32 s) {
  return carbon_math_vec3(v.x * s, v.y * s, v.z * s);
}

CBN_Vec3 carbon_math_vec3_neg(CBN_Vec3 v) {
  return carbon_math_vec3_scale(v, -1);
}

f32 carbon_math_vec3_len_squared(CBN_Vec3 v) {
  return carbon_math_vec3_dot(v, v);
}

f32 carbon_math_vec3_len(CBN_Vec3 v) {
  return carbon_math_sqrt(carbon_math_vec3_len_squared(v));
}

CBN_Vec3 carbon_math_vec3_norm(CBN_Vec3 v) {
  f32 sqlen = carbon_math_vec3_len_squared(v);
  if (sqlen <= CARBON_EPS_SQ) return v;
  return carbon_math_vec3_scale(v, carbon_math_rsqrt(sqlen));
}

char *carbon_math_vec3_to_cstr(CBN_Vec3 v) {
  return carbon_string_fmt("(%.3f, %.3f, %.3f)", v.x, v.y, v.z);
}

CBN_Vec3 carbon_math_vec3_lerp(CBN_Vec3 u, CBN_Vec3 v, f32 t) {
  return carbon_math_vec3(carbon_math_lerp(u.x, v.x, t),
                          carbon_math_lerp(u.y, v.y, t),
                          carbon_math_lerp(u.z, v.z, t));
}

CBN_Vec3 carbon_math_vec3_rotate_x(CBN_Vec3 v, f32 angle) {
  CBN_Vec2 r = carbon_math_vec2_rotate(v.yz, angle);
  return carbon_math_vec3(v.x, r.x, r.y);
}

CBN_Vec3 carbon_math_vec3_rotate_y(CBN_Vec3 v, f32 angle) {
  CBN_Vec2 r = carbon_math_vec2_rotate(CARBON_VEC_xz(v), angle);
  return carbon_math_vec3(r.x, v.y, r.y);
}

CBN_Vec3 carbon_math_vec3_rotate_z(CBN_Vec3 v, f32 angle) {
  CBN_Vec2 r = carbon_math_vec2_rotate(v.xy, angle);
  return carbon_math_vec3(r.x, r.y, v.z);
}

CBN_Vec3 carbon_math_vec3_rotate(CBN_Vec3 v, CBN_Quat q) {
  CBN_Vec3 u = q.xyz;
  CBN_Vec3 t = carbon_math_vec3_scale(carbon_math_vec3_cross(u, v), 2);
  return carbon_math_vec3_add(v, carbon_math_vec3_add(carbon_math_vec3_scale(t, q.w), carbon_math_vec3_cross(u, t)));
}

bool carbon_math_vec3_project_2d(CBN_Vec3 v, f32 near_z, CBN_Vec2 *out_v) {
  if (!out_v || v.z <= near_z) return false;
  *out_v = carbon_math_vec2(v.x/v.z, v.y/v.z);
  return true;
}

CBN_Vec3 carbon_math_vec3_reflect(CBN_Vec3 i, CBN_Vec3 n) {
  n = carbon_math_vec3_norm(n);
  return carbon_math_vec3_sub(i, carbon_math_vec3_scale(n, 2 * carbon_math_vec3_dot(n, i)));
}

CBN_Vec3 carbon_math_vec3_refract(CBN_Vec3 i, CBN_Vec3 n, f32 idx) {
  f32 d = carbon_math_vec3_dot(n, i);
  f32 k = 1 - idx*idx * (1 - d*d);
  if (k < 0) return carbon_math_vec3_1(0);
  return carbon_math_vec3_sub(carbon_math_vec3_scale(i, idx), carbon_math_vec3_scale(n, idx * d + carbon_math_sqrt(k)));
}
