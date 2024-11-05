// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_Vec2 operator+(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return carbon_math_vec2_add(u, v);
}

CBN_Vec3 operator+(const CBN_Vec3 &u, const CBN_Vec3 &v) {
  return carbon_math_vec3_add(u, v);
}

CBN_Vec2 operator-(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return carbon_math_vec2_sub(u, v);
}

CBN_Vec3 operator-(const CBN_Vec3 &u, const CBN_Vec3 &v) {
  return carbon_math_vec3_sub(u, v);
}

f32 operator*(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return carbon_math_vec2_dot(u, v);
}

f32 operator*(const CBN_Vec3 &u, const CBN_Vec3 &v) {
  return carbon_math_vec3_dot(u, v);
}

#endif  // __cplusplus
