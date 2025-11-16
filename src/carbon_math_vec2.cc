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

CBN_Vec2 CBN_Vec2::Abs(void) const {
  return carbon_math_vec2_abs(*this);
}

CBN_Vec2 CBN_Vec2::Exp(void) const {
  return carbon_math_vec2_exp(*this);
}

CBN_Vec2 CBN_Vec2::Sin(void) const {
  return carbon_math_vec2_sin(*this);
}

CBN_Vec2 CBN_Vec2::Cos(void) const {
  return carbon_math_vec2_cos(*this);
}

CBN_Vec2 CBN_Vec2::Tan(void) const {
  return carbon_math_vec2_tan(*this);
}

CBN_Vec2 CBN_Vec2::Tanh(void) const {
  return carbon_math_vec2_tanh(*this);
}

CBN_Vec2 CBN_Vec2::Lerp(const CBN_Vec2 &v, f32 t) const {
  return carbon_math_vec2_lerp(*this, v, t);
}

CBN_Vec2 CBN_Vec2::Rotate(f32 angle) const {
  return carbon_math_vec2_rotate(*this, angle);
}

CBN_Vec2 CBN_Vec2::RotatePivot(f32 angle, const CBN_Vec2 &pivot) const {
  return carbon_math_vec2_rotate_around_pivot(*this, angle, pivot);
}

const char *CBN_Vec2::ToString(void) const {
  return carbon_math_vec2_to_cstr(*this);
}

CBN_Vec2 CBN_Vec2::operator+(const CBN_Vec2 &v) const {
  return carbon_math_vec2_add(*this, v);
}

CBN_Vec2 CBN_Vec2::operator+(f32 s) const {
  return CARBON_VEC2(x + s, y + s);
}

CBN_Vec2 operator+(f32 s, const CBN_Vec2 &v) {
  return v + s;
}

CBN_Vec2 &CBN_Vec2::operator+=(const CBN_Vec2 &v) {
  *this = *this + v;
  return *this;
}

CBN_Vec2 &CBN_Vec2::operator+=(f32 s) {
  *this = *this + s;
  return *this;
}

CBN_Vec2 CBN_Vec2::operator-(void) const {
  return *this * -1;
}

CBN_Vec2 CBN_Vec2::operator-(const CBN_Vec2 &v) const {
  return carbon_math_vec2_sub(*this, v);
}

CBN_Vec2 CBN_Vec2::operator-(f32 s) const {
  return CARBON_VEC2(x - s, y - s);
}

CBN_Vec2 operator-(f32 s, const CBN_Vec2 &v) {
  return -v + s;
}

void CBN_Vec2::operator-=(const CBN_Vec2 &v) {
  *this = *this - v;
}

CBN_Vec2 CBN_Vec2::operator*(const CBN_Vec2 &v) const {
  return carbon_math_vec2_mult(*this, v);
}

f32 CBN_Vec2::Dot(const CBN_Vec2 &v) const {
  return carbon_math_vec2_dot(*this, v);
}

f32 CBN_Vec2::Dot(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return u.Dot(v);
}

CBN_Vec2 CBN_Vec2::operator*(f32 s) const {
  return carbon_math_vec2_scale(*this, s);
}

CBN_Vec2 operator*(f32 s, const CBN_Vec2 &v) {
  return v * s;
}

void CBN_Vec2::operator*=(f32 s) {
  *this = *this * s;
}

CBN_Vec2 CBN_Vec2::operator/(f32 s) const {
  return carbon_math_vec2_scale(*this, 1/s);
}

void CBN_Vec2::operator/=(f32 s) {
  *this = *this / s;
}

CBN_Vec2 CBN_Vec2::xx(void)   const { return CARBON_VEC_xx(*this);   }
CBN_Vec2 CBN_Vec2::xy(void)   const { return CARBON_VEC_xy(*this);   }
CBN_Vec2 CBN_Vec2::yx(void)   const { return CARBON_VEC_yx(*this);   }
CBN_Vec2 CBN_Vec2::yy(void)   const { return CARBON_VEC_yy(*this);   }
CBN_Vec3 CBN_Vec2::xxx(void)  const { return CARBON_VEC_xxx(*this);  }
CBN_Vec3 CBN_Vec2::xxy(void)  const { return CARBON_VEC_xxy(*this);  }
CBN_Vec3 CBN_Vec2::xyx(void)  const { return CARBON_VEC_xyx(*this);  }
CBN_Vec3 CBN_Vec2::xyy(void)  const { return CARBON_VEC_xyy(*this);  }
CBN_Vec3 CBN_Vec2::yxx(void)  const { return CARBON_VEC_yxx(*this);  }
CBN_Vec3 CBN_Vec2::yxy(void)  const { return CARBON_VEC_yxy(*this);  }
CBN_Vec3 CBN_Vec2::yyx(void)  const { return CARBON_VEC_yyx(*this);  }
CBN_Vec3 CBN_Vec2::yyy(void)  const { return CARBON_VEC_yyy(*this);  }
CBN_Vec4 CBN_Vec2::xyyx(void) const { return CARBON_VEC_xyyx(*this); }
