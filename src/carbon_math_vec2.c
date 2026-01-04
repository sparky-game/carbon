// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec2 carbon_math_vec2(f32 x, f32 y) {
  return (CBN_Vec2){{{x, y}}};
}

CBN_Vec2 carbon_math_vec2_1(f32 x) {
  return carbon_math_vec2(x, x);
}

CBN_Vec2 carbon_math_vec2_p(f32 r, f32 phi) {
  return carbon_math_vec2(r * carbon_math_cos(phi), r * carbon_math_sin(phi));
}

CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v) {
  return carbon_math_vec2(u.x + v.x, u.y + v.y);
}

CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v) {
  return carbon_math_vec2(u.x - v.x, u.y - v.y);
}

CBN_Vec2 carbon_math_vec2_mult(CBN_Vec2 u, CBN_Vec2 v) {
  return carbon_math_vec2(u.x * v.x, u.y * v.y);
}

f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v) {
  return u.x * v.x + u.y * v.y;
}

CBN_Vec2 carbon_math_vec2_scale(CBN_Vec2 v, f32 s) {
  return carbon_math_vec2(v.x * s, v.y * s);
}

CBN_Vec2 carbon_math_vec2_clamp(CBN_Vec2 v, CBN_Vec2 min, CBN_Vec2 max) {
  return carbon_math_vec2(CARBON_CLAMP(v.x, min.x, max.x), CARBON_CLAMP(v.y, min.y, max.y));
}

CBN_Vec2 carbon_math_vec2_floor(CBN_Vec2 v) {
  return carbon_math_vec2(carbon_math_floor(v.x), carbon_math_floor(v.y));
}

f32 carbon_math_vec2_len_squared(CBN_Vec2 v) {
  return carbon_math_vec2_dot(v, v);
}

f32 carbon_math_vec2_len(CBN_Vec2 v) {
  return carbon_math_sqrt(carbon_math_vec2_len_squared(v));
}

CBN_Vec2 carbon_math_vec2_norm(CBN_Vec2 v) {
  f32 sqlen = carbon_math_vec2_len_squared(v);
  if (sqlen <= CARBON_EPS_SQ) return v;
  return carbon_math_vec2_scale(v, carbon_math_rsqrt(sqlen));
}

CBN_Vec2 carbon_math_vec2_abs(CBN_Vec2 v) {
  return carbon_math_vec2(carbon_math_abs(v.x), carbon_math_abs(v.y));
}

CBN_Vec2 carbon_math_vec2_exp(CBN_Vec2 v) {
  return carbon_math_vec2(carbon_math_exp(v.x), carbon_math_exp(v.y));
}

CBN_Vec2 carbon_math_vec2_sin(CBN_Vec2 v) {
  return carbon_math_vec2(carbon_math_sin(v.x), carbon_math_sin(v.y));
}

CBN_Vec2 carbon_math_vec2_cos(CBN_Vec2 v) {
  return carbon_math_vec2(carbon_math_cos(v.x), carbon_math_cos(v.y));
}

CBN_Vec2 carbon_math_vec2_tan(CBN_Vec2 v) {
  return carbon_math_vec2(carbon_math_tan(v.x), carbon_math_tan(v.y));
}

CBN_Vec2 carbon_math_vec2_tanh(CBN_Vec2 v) {
  return carbon_math_vec2(carbon_math_tanh(v.x), carbon_math_tanh(v.y));
}

CBN_Vec2 carbon_math_vec2_lerp(CBN_Vec2 u, CBN_Vec2 v, f32 t) {
  return carbon_math_vec2(CARBON_LERP(u.x, v.x, t), CARBON_LERP(u.y, v.y, t));
}

CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle) {
  f32 phi = CARBON_TO_RADIANS(angle);
  f32 c = carbon_math_cos(phi), s = carbon_math_sin(phi);
  return carbon_math_vec2(c*v.x - s*v.y, s*v.x + c*v.y);
}

CBN_Vec2 carbon_math_vec2_rotate_around_pivot(CBN_Vec2 v, f32 angle, CBN_Vec2 pivot) {
  CBN_Vec2 r = carbon_math_vec2_rotate(carbon_math_vec2_sub(v, pivot), angle);
  return carbon_math_vec2_add(r, pivot);
}

char *carbon_math_vec2_to_cstr(CBN_Vec2 v) {
  return carbon_string_fmt("(%.3f, %.3f)", v.x, v.y);
}
