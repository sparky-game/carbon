// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec4 carbon_math_vec4(f32 x, f32 y, f32 z, f32 w) {
  return (CBN_Vec4){.c = {x, y, z, w}};
}

CBN_Vec4 carbon_math_vec4_1(f32 x) {
  return carbon_math_vec4(x, x, x, x);
}

CBN_Vec4 carbon_math_vec4_2(CBN_Vec2 v, f32 z, f32 w) {
  return carbon_math_vec4(v.x, v.y, z, w);
}

CBN_Vec4 carbon_math_vec4_3(CBN_Vec3 v, f32 w) {
  return carbon_math_vec4(v.x, v.y, v.z, w);
}

CBN_Vec4 carbon_math_vec4_add(CBN_Vec4 u, CBN_Vec4 v) {
  return carbon_math_vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

CBN_Vec4 carbon_math_vec4_sub(CBN_Vec4 u, CBN_Vec4 v) {
  return carbon_math_vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

CBN_Vec4 carbon_math_vec4_mult(CBN_Vec4 u, CBN_Vec4 v) {
  return carbon_math_vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
}

f32 carbon_math_vec4_dot(CBN_Vec4 u, CBN_Vec4 v) {
  return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

CBN_Vec4 carbon_math_vec4_scale(CBN_Vec4 v, f32 s) {
  return carbon_math_vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

CBNINL CBN_Vec4 carbon_math_vec4__apply_func(CBN_Vec4 v, f32 (*f)(f32)) {
  return carbon_math_vec4(f(v.x), f(v.y), f(v.z), f(v.w));
}

CBN_Vec4 carbon_math_vec4_clamp(CBN_Vec4 v, CBN_Vec4 min, CBN_Vec4 max) {
  return carbon_math_vec4(carbon_math_clamp(v.x, min.x, max.x),
                          carbon_math_clamp(v.y, min.y, max.y),
                          carbon_math_clamp(v.z, min.z, max.z),
                          carbon_math_clamp(v.w, min.w, max.w));
}

CBN_Vec4 carbon_math_vec4_abs(CBN_Vec4 v) {
  return carbon_math_vec4__apply_func(v, carbon_math_abs);
}

CBN_Vec4 carbon_math_vec4_exp(CBN_Vec4 v) {
  return carbon_math_vec4__apply_func(v, carbon_math_exp);
}

CBN_Vec4 carbon_math_vec4_sin(CBN_Vec4 v) {
  return carbon_math_vec4__apply_func(v, carbon_math_sin);
}

CBN_Vec4 carbon_math_vec4_cos(CBN_Vec4 v) {
  return carbon_math_vec4__apply_func(v, carbon_math_cos);
}

CBN_Vec4 carbon_math_vec4_tan(CBN_Vec4 v) {
  return carbon_math_vec4__apply_func(v, carbon_math_tan);
}

CBN_Vec4 carbon_math_vec4_tanh(CBN_Vec4 v) {
  return carbon_math_vec4__apply_func(v, carbon_math_tanh);
}

char *carbon_math_vec4_to_cstr(CBN_Vec4 v) {
  return carbon_string_fmt("(%.3f, %.3f, %.3f, %.3f)", v.x, v.y, v.z, v.w);
}

CBN_Vec4 carbon_math_vec4_lerp(CBN_Vec4 u, CBN_Vec4 v, f32 t) {
  return carbon_math_vec4(carbon_math_lerp(u.x, v.x, t),
                          carbon_math_lerp(u.y, v.y, t),
                          carbon_math_lerp(u.z, v.z, t),
                          carbon_math_lerp(u.w, v.w, t));
}

bool carbon_math_vec4_project_3d(CBN_Vec4 v, CBN_Vec3 *out_v) {
  if (!out_v || !v.w) return false;
  *out_v = carbon_math_vec3(v.x/v.w, v.y/v.w, v.z/v.w);
  return true;
}
