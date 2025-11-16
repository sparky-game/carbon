// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec4 carbon_math_vec4_add(CBN_Vec4 u, CBN_Vec4 v) {
  return CARBON_VEC4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

CBN_Vec4 carbon_math_vec4_sub(CBN_Vec4 u, CBN_Vec4 v) {
  return CARBON_VEC4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

CBN_Vec4 carbon_math_vec4_mult(CBN_Vec4 u, CBN_Vec4 v) {
  return CARBON_VEC4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
}

f32 carbon_math_vec4_dot(CBN_Vec4 u, CBN_Vec4 v) {
  return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

CBN_Vec4 carbon_math_vec4_scale(CBN_Vec4 v, f32 s) {
  return CARBON_VEC4(v.x * s, v.y * s, v.z * s, v.w * s);
}

CBN_Vec4 carbon_math_vec4_clamp(CBN_Vec4 v, CBN_Vec4 min, CBN_Vec4 max) {
  return CARBON_VEC4(CARBON_CLAMP(v.x, min.x, max.x),
                     CARBON_CLAMP(v.y, min.y, max.y),
                     CARBON_CLAMP(v.z, min.z, max.z),
                     CARBON_CLAMP(v.w, min.w, max.w));
}

CBN_Vec4 carbon_math_vec4_abs(CBN_Vec4 v) {
  return CARBON_VEC4(carbon_math_abs(v.x),
                     carbon_math_abs(v.y),
                     carbon_math_abs(v.z),
                     carbon_math_abs(v.w));
}

CBN_Vec4 carbon_math_vec4_exp(CBN_Vec4 v) {
  return CARBON_VEC4(carbon_math_exp(v.x),
                     carbon_math_exp(v.y),
                     carbon_math_exp(v.z),
                     carbon_math_exp(v.w));
}

CBN_Vec4 carbon_math_vec4_sin(CBN_Vec4 v) {
  return CARBON_VEC4(carbon_math_sin(v.x),
                     carbon_math_sin(v.y),
                     carbon_math_sin(v.z),
                     carbon_math_sin(v.w));
}

CBN_Vec4 carbon_math_vec4_cos(CBN_Vec4 v) {
  return CARBON_VEC4(carbon_math_cos(v.x),
                     carbon_math_cos(v.y),
                     carbon_math_cos(v.z),
                     carbon_math_cos(v.w));
}

CBN_Vec4 carbon_math_vec4_tan(CBN_Vec4 v) {
  return CARBON_VEC4(carbon_math_tan(v.x),
                     carbon_math_tan(v.y),
                     carbon_math_tan(v.z),
                     carbon_math_tan(v.w));
}

CBN_Vec4 carbon_math_vec4_tanh(CBN_Vec4 v) {
  return CARBON_VEC4(carbon_math_tanh(v.x),
                     carbon_math_tanh(v.y),
                     carbon_math_tanh(v.z),
                     carbon_math_tanh(v.w));
}

char *carbon_math_vec4_to_cstr(CBN_Vec4 v) {
  return carbon_string_fmt("(%.3f, %.3f, %.3f, %.3f)", v.x, v.y, v.z, v.w);
}

CBN_Vec4 carbon_math_vec4_lerp(CBN_Vec4 u, CBN_Vec4 v, f32 t) {
  return CARBON_VEC4(CARBON_LERP(u.x, v.x, t),
                     CARBON_LERP(u.y, v.y, t),
                     CARBON_LERP(u.z, v.z, t),
                     CARBON_LERP(u.w, v.w, t));
}

bool carbon_math_vec4_project_3d(CBN_Vec4 v, CBN_Vec3 *out_v) {
  if (!out_v || !v.w) return false;
  *out_v = CARBON_VEC3(v.x/v.w, v.y/v.w, v.z/v.w);
  return true;
}
