// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v) {
  return (CBN_Vec2) {
    .x = u.x + v.x,
    .y = u.y + v.y
  };
}

CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v) {
  return (CBN_Vec2) {
    .x = u.x - v.x,
    .y = u.y - v.y
  };
}

f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v) {
  return (u.x * v.x) + (u.y * v.y);
}

CBN_Vec2 carbon_math_vec2_clamp(CBN_Vec2 v, CBN_Vec2 min, CBN_Vec2 max) {
  return CARBON_VEC2(CARBON_CLAMP(v.x, min.x, max.x), CARBON_CLAMP(v.y, min.y, max.y));
}

CBN_Vec2 carbon_math_vec2_floor(CBN_Vec2 v) {
  return CARBON_VEC2(carbon_math_floor(v.x), carbon_math_floor(v.y));
}

f32 carbon_math_vec2_len_squared(CBN_Vec2 v) {
  return carbon_math_vec2_dot(v, v);
}

f32 carbon_math_vec2_len(CBN_Vec2 v) {
  return carbon_math_sqrt(carbon_math_vec2_len_squared(v));
}

CBN_Vec2 carbon_math_vec2_norm(CBN_Vec2 v) {
  f32 len = carbon_math_vec2_len(v);
  if (!len) return v;
  return carbon_math_vec2_scale(v, 1/len);
}

CBN_Vec2 carbon_math_vec2_lerp(CBN_Vec2 u, CBN_Vec2 v, f32 t) {
  return CARBON_VEC2(CARBON_LERP(u.x, v.x, t), CARBON_LERP(u.y, v.y, t));
}

CBN_Vec2 carbon_math_vec2_scale(CBN_Vec2 v, f32 s) {
  return CARBON_VEC2(v.x * s, v.y * s);
}

CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle) {
  f32 rads = angle * (CARBON_PI / 180);
  f32 c = carbon_math_cos(rads), s = carbon_math_sin(rads);
  return (CBN_Vec2) {
    .x = (v.x * c) - (v.y * s),
    .y = (v.x * s) + (v.y * c)
  };
}

CBN_Vec2 carbon_math_vec2_rotate_around_pivot(CBN_Vec2 v, f32 angle, CBN_Vec2 pivot) {
  CBN_Vec2 r = carbon_math_vec2_rotate(CARBON_VEC2(v.x - pivot.x, v.y - pivot.y), angle);
  return CARBON_VEC2(r.x + pivot.x, r.y + pivot.y);
}

char *carbon_math_vec2_to_cstr(CBN_Vec2 v) {
  return carbon_string_fmt("(%.3f, %.3f)", v.x, v.y);
}
