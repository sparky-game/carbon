// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_Vec2 operator+(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return carbon_math_vec2_add(u, v);
}

void operator+=(CBN_Vec2 &u, const CBN_Vec2 &v) {
  u = carbon_math_vec2_add(u, v);
}

CBN_Vec2 operator-(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return carbon_math_vec2_sub(u, v);
}

f32 operator*(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return carbon_math_vec2_dot(u, v);
}

CBN_Vec2 operator*(const CBN_Vec2 &v, const f32 s) {
  return carbon_math_vec2_scale(v, s);
}

void operator*=(CBN_Vec2 &v, const f32 s) {
  v = carbon_math_vec2_scale(v, s);
}

CBN_Vec3 operator+(const CBN_Vec3 &u, const CBN_Vec3 &v) {
  return carbon_math_vec3_add(u, v);
}

CBN_Vec3 operator-(const CBN_Vec3 &u, const CBN_Vec3 &v) {
  return carbon_math_vec3_sub(u, v);
}

f32 operator*(const CBN_Vec3 &u, const CBN_Vec3 &v) {
  return carbon_math_vec3_dot(u, v);
}

#endif  // __cplusplus
