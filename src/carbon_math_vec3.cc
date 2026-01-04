// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

using namespace cbn::math;

Vec3 Vec3::Cross(const Vec3 &v) const {
  return carbon_math_vec3_cross(*this, v);
}

Vec3 Vec3::Lerp(const Vec3 &v, f32 t) const {
  return carbon_math_vec3_lerp(*this, v, t);
}

Quat Vec3::ToQuat(void) const {
  return carbon_math_quat_from_euler(*this);
}

const char *Vec3::ToString(void) const {
  return carbon_math_vec3_to_cstr(*this);
}

Vec3 Vec3::RotateX(f32 angle) const {
  return carbon_math_vec3_rotate_x(*this, angle);
}

Vec3 Vec3::RotateY(f32 angle) const {
  return carbon_math_vec3_rotate_y(*this, angle);
}

Vec3 Vec3::RotateZ(f32 angle) const {
  return carbon_math_vec3_rotate_z(*this, angle);
}

Vec3 Vec3::Rotate(Quat q) const {
  return carbon_math_vec3_rotate(*this, q);
}

cbn::Opt<Vec2> Vec3::Project2D(f32 near_z) const {
  Vec2 v;
  if (!carbon_math_vec3_project_2d(*this, near_z, &v)) return {};
  return v;
}

Vec3 Vec3::operator+(const Vec3 &v) const {
  return carbon_math_vec3_add(*this, v);
}

Vec3 Vec3::operator+(f32 s) const {
  return CARBON_VEC3(x + s, y + s, z + s);
}

Vec3 operator+(f32 s, const Vec3 &v) {
  return v + s;
}

Vec3 &Vec3::operator+=(const Vec3 &v) {
  *this = *this + v;
  return *this;
}

Vec3 &Vec3::operator+=(f32 s) {
  *this = *this + s;
  return *this;
}

Vec3 Vec3::operator-(const Vec3 &v) const {
  return carbon_math_vec3_sub(*this, v);
}

f32 Vec3::operator*(const Vec3 &v) const {
  return carbon_math_vec3_dot(*this, v);
}

Vec2 Vec3::xx(void)  const { return Vec2(x);    }
Vec2 Vec3::xz(void)  const { return Vec2(x, z); }
Vec2 Vec3::yx(void)  const { return Vec2(y, x); }
Vec2 Vec3::yy(void)  const { return Vec2(y);    }
Vec2 Vec3::zx(void)  const { return Vec2(z, x); }
Vec2 Vec3::zy(void)  const { return Vec2(z, y); }
Vec2 Vec3::zz(void)  const { return Vec2(z);    }
Vec3 Vec3::xxx(void) const { return CARBON_VEC_xxx(*this); }
Vec3 Vec3::xxy(void) const { return CARBON_VEC_xxy(*this); }
Vec3 Vec3::xxz(void) const { return CARBON_VEC_xxz(*this); }
Vec3 Vec3::xyx(void) const { return CARBON_VEC_xyx(*this); }
Vec3 Vec3::xyy(void) const { return CARBON_VEC_xyy(*this); }
Vec3 Vec3::xyz(void) const { return CARBON_VEC_xyz(*this); }
Vec3 Vec3::xzx(void) const { return CARBON_VEC_xzx(*this); }
Vec3 Vec3::xzy(void) const { return CARBON_VEC_xzy(*this); }
Vec3 Vec3::xzz(void) const { return CARBON_VEC_xzz(*this); }
Vec3 Vec3::yxx(void) const { return CARBON_VEC_yxx(*this); }
Vec3 Vec3::yxy(void) const { return CARBON_VEC_yxy(*this); }
Vec3 Vec3::yxz(void) const { return CARBON_VEC_yxz(*this); }
Vec3 Vec3::yyx(void) const { return CARBON_VEC_yyx(*this); }
Vec3 Vec3::yyy(void) const { return CARBON_VEC_yyy(*this); }
Vec3 Vec3::yyz(void) const { return CARBON_VEC_yyz(*this); }
Vec3 Vec3::yzx(void) const { return CARBON_VEC_yzx(*this); }
Vec3 Vec3::yzy(void) const { return CARBON_VEC_yzy(*this); }
Vec3 Vec3::yzz(void) const { return CARBON_VEC_yzz(*this); }
Vec3 Vec3::zxx(void) const { return CARBON_VEC_zxx(*this); }
Vec3 Vec3::zxy(void) const { return CARBON_VEC_zxy(*this); }
Vec3 Vec3::zxz(void) const { return CARBON_VEC_zxz(*this); }
Vec3 Vec3::zyx(void) const { return CARBON_VEC_zyx(*this); }
Vec3 Vec3::zyy(void) const { return CARBON_VEC_zyy(*this); }
Vec3 Vec3::zyz(void) const { return CARBON_VEC_zyz(*this); }
Vec3 Vec3::zzx(void) const { return CARBON_VEC_zzx(*this); }
Vec3 Vec3::zzy(void) const { return CARBON_VEC_zzy(*this); }
Vec3 Vec3::zzz(void) const { return CARBON_VEC_zzz(*this); }
