// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifdef __cplusplus

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

// Vector Swizzling Operations
constexpr auto CBN_Vec2::xx(void)  const { return CARBON_VEC_xx(*this);  }
constexpr auto CBN_Vec3::xx(void)  const { return CARBON_VEC_xx(*this);  }
constexpr auto CBN_Vec2::xy(void)  const { return CARBON_VEC_xy(*this);  }
constexpr auto CBN_Vec3::xy(void)  const { return CARBON_VEC_xy(*this);  }
constexpr auto CBN_Vec3::xz(void)  const { return CARBON_VEC_xz(*this);  }
constexpr auto CBN_Vec2::yx(void)  const { return CARBON_VEC_yx(*this);  }
constexpr auto CBN_Vec3::yx(void)  const { return CARBON_VEC_yx(*this);  }
constexpr auto CBN_Vec2::yy(void)  const { return CARBON_VEC_yy(*this);  }
constexpr auto CBN_Vec3::yy(void)  const { return CARBON_VEC_yy(*this);  }
constexpr auto CBN_Vec3::yz(void)  const { return CARBON_VEC_yz(*this);  }
constexpr auto CBN_Vec3::zx(void)  const { return CARBON_VEC_zx(*this);  }
constexpr auto CBN_Vec3::zy(void)  const { return CARBON_VEC_zy(*this);  }
constexpr auto CBN_Vec3::zz(void)  const { return CARBON_VEC_zz(*this);  }
constexpr auto CBN_Vec2::xxx(void) const { return CARBON_VEC_xxx(*this); }
constexpr auto CBN_Vec3::xxx(void) const { return CARBON_VEC_xxx(*this); }
constexpr auto CBN_Vec2::xxy(void) const { return CARBON_VEC_xxy(*this); }
constexpr auto CBN_Vec3::xxy(void) const { return CARBON_VEC_xxy(*this); }
constexpr auto CBN_Vec3::xxz(void) const { return CARBON_VEC_xxz(*this); }
constexpr auto CBN_Vec2::xyx(void) const { return CARBON_VEC_xyx(*this); }
constexpr auto CBN_Vec3::xyx(void) const { return CARBON_VEC_xyx(*this); }
constexpr auto CBN_Vec2::xyy(void) const { return CARBON_VEC_xyy(*this); }
constexpr auto CBN_Vec3::xyy(void) const { return CARBON_VEC_xyy(*this); }
constexpr auto CBN_Vec3::xyz(void) const { return CARBON_VEC_xyz(*this); }
constexpr auto CBN_Vec3::xzx(void) const { return CARBON_VEC_xzx(*this); }
constexpr auto CBN_Vec3::xzy(void) const { return CARBON_VEC_xzy(*this); }
constexpr auto CBN_Vec3::xzz(void) const { return CARBON_VEC_xzz(*this); }
constexpr auto CBN_Vec2::yxx(void) const { return CARBON_VEC_yxx(*this); }
constexpr auto CBN_Vec3::yxx(void) const { return CARBON_VEC_yxx(*this); }
constexpr auto CBN_Vec2::yxy(void) const { return CARBON_VEC_yxy(*this); }
constexpr auto CBN_Vec3::yxy(void) const { return CARBON_VEC_yxy(*this); }
constexpr auto CBN_Vec3::yxz(void) const { return CARBON_VEC_yxz(*this); }
constexpr auto CBN_Vec2::yyx(void) const { return CARBON_VEC_yyx(*this); }
constexpr auto CBN_Vec3::yyx(void) const { return CARBON_VEC_yyx(*this); }
constexpr auto CBN_Vec2::yyy(void) const { return CARBON_VEC_yyy(*this); }
constexpr auto CBN_Vec3::yyy(void) const { return CARBON_VEC_yyy(*this); }
constexpr auto CBN_Vec3::yyz(void) const { return CARBON_VEC_yyz(*this); }
constexpr auto CBN_Vec3::yzx(void) const { return CARBON_VEC_yzx(*this); }
constexpr auto CBN_Vec3::yzy(void) const { return CARBON_VEC_yzy(*this); }
constexpr auto CBN_Vec3::yzz(void) const { return CARBON_VEC_yzz(*this); }
constexpr auto CBN_Vec3::zxx(void) const { return CARBON_VEC_zxx(*this); }
constexpr auto CBN_Vec3::zxy(void) const { return CARBON_VEC_zxy(*this); }
constexpr auto CBN_Vec3::zxz(void) const { return CARBON_VEC_zxz(*this); }
constexpr auto CBN_Vec3::zyx(void) const { return CARBON_VEC_zyx(*this); }
constexpr auto CBN_Vec3::zyy(void) const { return CARBON_VEC_zyy(*this); }
constexpr auto CBN_Vec3::zyz(void) const { return CARBON_VEC_zyz(*this); }
constexpr auto CBN_Vec3::zzx(void) const { return CARBON_VEC_zzx(*this); }
constexpr auto CBN_Vec3::zzy(void) const { return CARBON_VEC_zzy(*this); }
constexpr auto CBN_Vec3::zzz(void) const { return CARBON_VEC_zzz(*this); }

u8 CBN_Rect::Contains(const CBN_Vec2 &p) const {
  return carbon_math_rect_contains_point(*this, p);
}

u8 CBN_Rect::Overlaps(const CBN_Rect &r) const {
  return carbon_math_rect_detect_collision(*this, r);
}

void CBN_Rect::Scale(const f32 s) {
  carbon_math_rect_scale(this, s);
}

bool CBN_Rect::operator==(const CBN_Rect &r) const {
  return (x == r.x) && (y == r.y) && (w == r.w) && (h == r.h);
}

#endif  // __cplusplus
