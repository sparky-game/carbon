// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec4 CBN_Vec4::operator+(const CBN_Vec4 &v) const {
  return carbon_math_vec4_add(*this, v);
}

CBN_Vec4 CBN_Vec4::operator+(f32 s) const {
  return CARBON_VEC4(x + s, y + s, z + s, w + s);
}

CBN_Vec4 operator+(f32 s, const CBN_Vec4 &v) {
  return v + s;
}

CBN_Vec4 &CBN_Vec4::operator+=(const CBN_Vec4 &v) {
  *this = *this + v;
  return *this;
}

CBN_Vec4 &CBN_Vec4::operator+=(f32 s) {
  *this = *this + s;
  return *this;
}

CBN_Vec4 CBN_Vec4::operator-(f32 s) const {
  return CARBON_VEC4(x - s, y - s, z - s, w - s);
}

CBN_Vec4 operator-(f32 s, const CBN_Vec4 &v) {
  return -v + s;
}

CBN_Vec4 CBN_Vec4::operator/(const CBN_Vec4 &v) const {
  return carbon_math_vec4_mult(*this, 1/v);
}

CBN_Vec4 CBN_Vec4::operator-(void) const {
  return *this * -1;
}

CBN_Vec4 CBN_Vec4::operator*(f32 s) const {
  return carbon_math_vec4_scale(*this, s);
}

CBN_Vec4 operator*(f32 s, const CBN_Vec4 &v) {
  return v * s;
}

CBN_Vec4 operator/(f32 s, const CBN_Vec4 &v) {
  return CARBON_VEC4(s/v.x, s/v.y, s/v.z, s/v.w);
}

void CBN_Vec4::Clamp(const CBN_Vec4 &min, const CBN_Vec4 &max) {
  *this = carbon_math_vec4_clamp(*this, min, max);
}

CBN_Vec4 CBN_Vec4::Abs(void) const {
  return carbon_math_vec4_abs(*this);
}

CBN_Vec4 CBN_Vec4::Exp(void) const {
  return carbon_math_vec4_exp(*this);
}

CBN_Vec4 CBN_Vec4::Sin(void) const {
  return carbon_math_vec4_sin(*this);
}

CBN_Vec4 CBN_Vec4::Cos(void) const {
  return carbon_math_vec4_cos(*this);
}

CBN_Vec4 CBN_Vec4::Tan(void) const {
  return carbon_math_vec4_tan(*this);
}

CBN_Vec4 CBN_Vec4::Tanh(void) const {
  return carbon_math_vec4_tanh(*this);
}

CBN_Vec4 CBN_Vec4::Lerp(const CBN_Vec4 &v, f32 t) const {
  return carbon_math_vec4_lerp(*this, v, t);
}

const char *CBN_Vec4::ToString(void) const {
  return carbon_math_vec4_to_cstr(*this);
}
