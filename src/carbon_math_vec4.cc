// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

using namespace cbn::math;

Vec4 Vec4::operator+(const Vec4 &v) const {
  return carbon_math_vec4_add(*this, v);
}

Vec4 Vec4::operator+(f32 s) const {
  return carbon_math_vec4(x + s, y + s, z + s, w + s);
}

Vec4 operator+(f32 s, const Vec4 &v) {
  return v + s;
}

Vec4 &Vec4::operator+=(const Vec4 &v) {
  *this = *this + v;
  return *this;
}

Vec4 &Vec4::operator+=(f32 s) {
  *this = *this + s;
  return *this;
}

Vec4 Vec4::operator-(f32 s) const {
  return carbon_math_vec4(x - s, y - s, z - s, w - s);
}

Vec4 operator-(f32 s, const Vec4 &v) {
  return -v + s;
}

Vec4 Vec4::operator/(const Vec4 &v) const {
  return carbon_math_vec4_mult(*this, 1/v);
}

Vec4 Vec4::operator-(void) const {
  return *this * -1;
}

Vec4 Vec4::operator*(f32 s) const {
  return carbon_math_vec4_scale(*this, s);
}

Vec4 operator*(f32 s, const Vec4 &v) {
  return v * s;
}

Vec4 operator/(f32 s, const Vec4 &v) {
  return carbon_math_vec4(s/v.x, s/v.y, s/v.z, s/v.w);
}

void Vec4::Clamp(const Vec4 &min, const Vec4 &max) {
  *this = carbon_math_vec4_clamp(*this, min, max);
}

Vec4 Vec4::Abs(void) const {
  return carbon_math_vec4_abs(*this);
}

Vec4 Vec4::Exp(void) const {
  return carbon_math_vec4_exp(*this);
}

Vec4 Vec4::Sin(void) const {
  return carbon_math_vec4_sin(*this);
}

Vec4 Vec4::Cos(void) const {
  return carbon_math_vec4_cos(*this);
}

Vec4 Vec4::Tan(void) const {
  return carbon_math_vec4_tan(*this);
}

Vec4 Vec4::Tanh(void) const {
  return carbon_math_vec4_tanh(*this);
}

Vec4 Vec4::Lerp(const Vec4 &v, f32 t) const {
  return carbon_math_vec4_lerp(*this, v, t);
}

const char *Vec4::ToString(void) const {
  return carbon_math_vec4_to_cstr(*this);
}
