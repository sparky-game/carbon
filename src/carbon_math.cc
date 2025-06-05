// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_Vec2 CBN_Vec2::Floor(void) const {
  return carbon_math_vec2_floor(*this);
}

f32 CBN_Vec2::Length(void) const {
  return carbon_math_vec2_len(*this);
}

f32 CBN_Vec2::LengthSquared(void) const {
  return carbon_math_vec2_len_squared(*this);
}

CBN_Vec2 CBN_Vec2::Normalize(void) const {
  return carbon_math_vec2_norm(*this);
}

CBN_Vec2 CBN_Vec2::Lerp(const CBN_Vec2 &v, f32 t) const {
  return carbon_math_vec2_lerp(*this, v, t);
}

CBN_Vec2 CBN_Vec2::Rotate(f32 angle) const {
  return carbon_math_vec2_rotate(*this, angle);
}

const char *CBN_Vec2::ToString(void) const {
  return carbon_math_vec2_to_cstr(*this);
}

CBN_Vec2 CBN_Vec2::operator+(const CBN_Vec2 &v) const {
  return carbon_math_vec2_add(*this, v);
}

void CBN_Vec2::operator+=(const CBN_Vec2 &v) {
  *this = *this + v;
}

CBN_Vec2 CBN_Vec2::operator-(const CBN_Vec2 &v) const {
  return carbon_math_vec2_sub(*this, v);
}

void CBN_Vec2::operator-=(const CBN_Vec2 &v) {
  *this = *this - v;
}

f32 CBN_Vec2::operator*(const CBN_Vec2 &v) const {
  return carbon_math_vec2_dot(*this, v);
}

CBN_Vec2 CBN_Vec2::operator*(const f32 s) const {
  return carbon_math_vec2_scale(*this, s);
}

CBN_Vec2 operator*(const f32 s, const CBN_Vec2 &v) {
  return v * s;
}

void CBN_Vec2::operator*=(const f32 s) {
  *this = *this * s;
}

CBN_Vec2 CBN_Vec2::operator/(const f32 s) const {
  return carbon_math_vec2_scale(*this, 1/s);
}

void CBN_Vec2::operator/=(const f32 s) {
  *this = *this / s;
}

const char *CBN_Vec3::ToString(void) const {
  return carbon_math_vec3_to_cstr(*this);
}

CBN_Vec3 CBN_Vec3::operator+(const CBN_Vec3 &v) const {
  return carbon_math_vec3_add(*this, v);
}

CBN_Vec3 CBN_Vec3::operator-(const CBN_Vec3 &v) const {
  return carbon_math_vec3_sub(*this, v);
}

f32 CBN_Vec3::operator*(const CBN_Vec3 &v) const {
  return carbon_math_vec3_dot(*this, v);
}

u8 CBN_Rect::Contains(const CBN_Vec2 &p) const {
  return carbon_math_rect_contains_point(*this, p);
}

u8 CBN_Rect::Overlaps(const CBN_Rect &r) const {
  return carbon_math_rect_detect_collision(*this, r);
}

#endif  // __cplusplus
