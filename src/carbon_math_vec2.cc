// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

void CBN_Vec2::Clamp(const CBN_Vec2 &min, const CBN_Vec2 &max) {
  *this = carbon_math_vec2_clamp(*this, min, max);
}

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

CBN_Vec2 CBN_Vec2::operator+(const f32 s) const {
  return CARBON_VEC2(x + s, y + s);
}

CBN_Vec2 operator+(const f32 s, const CBN_Vec2 &v) {
  return v + s;
}

void CBN_Vec2::operator+=(const CBN_Vec2 &v) {
  *this = *this + v;
}

CBN_Vec2 CBN_Vec2::operator-(void) const {
  return *this * -1;
}

CBN_Vec2 CBN_Vec2::operator-(const CBN_Vec2 &v) const {
  return carbon_math_vec2_sub(*this, v);
}

CBN_Vec2 CBN_Vec2::operator-(const f32 s) const {
  return CARBON_VEC2(x - s, y - s);
}

CBN_Vec2 operator-(const f32 s, const CBN_Vec2 &v) {
  return -v + s;
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

constexpr auto CBN_Vec2::xx(void)  const { return CARBON_VEC_xx(*this);  }
constexpr auto CBN_Vec2::xy(void)  const { return CARBON_VEC_xy(*this);  }
constexpr auto CBN_Vec2::yx(void)  const { return CARBON_VEC_yx(*this);  }
constexpr auto CBN_Vec2::yy(void)  const { return CARBON_VEC_yy(*this);  }
constexpr auto CBN_Vec2::xxx(void) const { return CARBON_VEC_xxx(*this); }
constexpr auto CBN_Vec2::xxy(void) const { return CARBON_VEC_xxy(*this); }
constexpr auto CBN_Vec2::xyx(void) const { return CARBON_VEC_xyx(*this); }
constexpr auto CBN_Vec2::xyy(void) const { return CARBON_VEC_xyy(*this); }
constexpr auto CBN_Vec2::yxx(void) const { return CARBON_VEC_yxx(*this); }
constexpr auto CBN_Vec2::yxy(void) const { return CARBON_VEC_yxy(*this); }
constexpr auto CBN_Vec2::yyx(void) const { return CARBON_VEC_yyx(*this); }
constexpr auto CBN_Vec2::yyy(void) const { return CARBON_VEC_yyy(*this); }
