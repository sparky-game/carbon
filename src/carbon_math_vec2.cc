// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

using namespace cbn::math;

void Vec2::Clamp(const Vec2 &min, const Vec2 &max) {
  *this = carbon_math_vec2_clamp(*this, min, max);
}

Vec2 Vec2::Floor(void) const {
  return carbon_math_vec2_floor(*this);
}

f32 Vec2::Length(void) const {
  return carbon_math_vec2_len(*this);
}

f32 Vec2::LengthSquared(void) const {
  return carbon_math_vec2_len_squared(*this);
}

Vec2 Vec2::Normalize(void) const {
  return carbon_math_vec2_norm(*this);
}

Vec2 Vec2::Abs(void) const {
  return carbon_math_vec2_abs(*this);
}

Vec2 Vec2::Exp(void) const {
  return carbon_math_vec2_exp(*this);
}

Vec2 Vec2::Sin(void) const {
  return carbon_math_vec2_sin(*this);
}

Vec2 Vec2::Cos(void) const {
  return carbon_math_vec2_cos(*this);
}

Vec2 Vec2::Tan(void) const {
  return carbon_math_vec2_tan(*this);
}

Vec2 Vec2::Tanh(void) const {
  return carbon_math_vec2_tanh(*this);
}

Vec2 Vec2::Lerp(const Vec2 &v, f32 t) const {
  return carbon_math_vec2_lerp(*this, v, t);
}

Vec2 Vec2::Rotate(f32 angle) const {
  return carbon_math_vec2_rotate(*this, angle);
}

Vec2 Vec2::RotatePivot(f32 angle, const Vec2 &pivot) const {
  return carbon_math_vec2_rotate_around_pivot(*this, angle, pivot);
}

const char *Vec2::ToString(void) const {
  return carbon_math_vec2_to_cstr(*this);
}

Vec2 Vec2::operator+(const Vec2 &v) const {
  return carbon_math_vec2_add(*this, v);
}

Vec2 Vec2::operator+(f32 s) const {
  return Vec2(x + s, y + s);
}

Vec2 operator+(f32 s, const Vec2 &v) {
  return v + s;
}

Vec2 &Vec2::operator+=(const Vec2 &v) {
  *this = *this + v;
  return *this;
}

Vec2 &Vec2::operator+=(f32 s) {
  *this = *this + s;
  return *this;
}

Vec2 Vec2::operator-(void) const {
  return *this * -1;
}

Vec2 Vec2::operator-(const Vec2 &v) const {
  return carbon_math_vec2_sub(*this, v);
}

Vec2 Vec2::operator-(f32 s) const {
  return Vec2(x - s, y - s);
}

Vec2 operator-(f32 s, const Vec2 &v) {
  return -v + s;
}

void Vec2::operator-=(const Vec2 &v) {
  *this = *this - v;
}

Vec2 Vec2::operator*(const Vec2 &v) const {
  return carbon_math_vec2_mult(*this, v);
}

f32 Vec2::Dot(const Vec2 &v) const {
  return carbon_math_vec2_dot(*this, v);
}

f32 Vec2::Dot(const Vec2 &u, const Vec2 &v) {
  return u.Dot(v);
}

Vec2 Vec2::operator*(f32 s) const {
  return carbon_math_vec2_scale(*this, s);
}

Vec2 operator*(f32 s, const Vec2 &v) {
  return v * s;
}

void Vec2::operator*=(f32 s) {
  *this = *this * s;
}

Vec2 Vec2::operator/(f32 s) const {
  return carbon_math_vec2_scale(*this, 1/s);
}

void Vec2::operator/=(f32 s) {
  *this = *this / s;
}

Vec2 Vec2::xx(void)   const { return Vec2(x);    }
Vec2 Vec2::xy(void)   const { return Vec2(x, y); }
Vec2 Vec2::yx(void)   const { return Vec2(y, x); }
Vec2 Vec2::yy(void)   const { return Vec2(y);    }
Vec3 Vec2::xxx(void)  const { return CARBON_VEC_xxx(*this);  }
Vec3 Vec2::xxy(void)  const { return CARBON_VEC_xxy(*this);  }
Vec3 Vec2::xyx(void)  const { return CARBON_VEC_xyx(*this);  }
Vec3 Vec2::xyy(void)  const { return CARBON_VEC_xyy(*this);  }
Vec3 Vec2::yxx(void)  const { return CARBON_VEC_yxx(*this);  }
Vec3 Vec2::yxy(void)  const { return CARBON_VEC_yxy(*this);  }
Vec3 Vec2::yyx(void)  const { return CARBON_VEC_yyx(*this);  }
Vec3 Vec2::yyy(void)  const { return CARBON_VEC_yyy(*this);  }
Vec4 Vec2::xyyx(void) const { return CARBON_VEC_xyyx(*this); }
