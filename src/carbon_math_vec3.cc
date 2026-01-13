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
  return carbon_math_vec3(x + s, y + s, z + s);
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

Vec2 Vec3::xx(void)  const { return Vec2(x);       }
Vec2 Vec3::xz(void)  const { return Vec2(x, z);    }
Vec2 Vec3::yx(void)  const { return Vec2(y, x);    }
Vec2 Vec3::yy(void)  const { return Vec2(y);       }
Vec2 Vec3::zx(void)  const { return Vec2(z, x);    }
Vec2 Vec3::zy(void)  const { return Vec2(z, y);    }
Vec2 Vec3::zz(void)  const { return Vec2(z);       }
Vec3 Vec3::xxx(void) const { return Vec3(x);       }
Vec3 Vec3::xxy(void) const { return Vec3(x, x, y); }
Vec3 Vec3::xxz(void) const { return Vec3(x, x, z); }
Vec3 Vec3::xyx(void) const { return Vec3(x, y, x); }
Vec3 Vec3::xyy(void) const { return Vec3(x, y, y); }
Vec3 Vec3::xyz(void) const { return Vec3(x, y, z); }
Vec3 Vec3::xzx(void) const { return Vec3(x, z, x); }
Vec3 Vec3::xzy(void) const { return Vec3(x, z, y); }
Vec3 Vec3::xzz(void) const { return Vec3(x, z, z); }
Vec3 Vec3::yxx(void) const { return Vec3(y, x, x); }
Vec3 Vec3::yxy(void) const { return Vec3(y, x, y); }
Vec3 Vec3::yxz(void) const { return Vec3(y, x, z); }
Vec3 Vec3::yyx(void) const { return Vec3(y, y, x); }
Vec3 Vec3::yyy(void) const { return Vec3(y);       }
Vec3 Vec3::yyz(void) const { return Vec3(y, y, z); }
Vec3 Vec3::yzx(void) const { return Vec3(y, z, x); }
Vec3 Vec3::yzy(void) const { return Vec3(y, z, y); }
Vec3 Vec3::yzz(void) const { return Vec3(y, z, z); }
Vec3 Vec3::zxx(void) const { return Vec3(z, x, x); }
Vec3 Vec3::zxy(void) const { return Vec3(z, x, y); }
Vec3 Vec3::zxz(void) const { return Vec3(z, x, z); }
Vec3 Vec3::zyx(void) const { return Vec3(z, y, x); }
Vec3 Vec3::zyy(void) const { return Vec3(z, y, y); }
Vec3 Vec3::zyz(void) const { return Vec3(z, y, z); }
Vec3 Vec3::zzx(void) const { return Vec3(z, z, x); }
Vec3 Vec3::zzy(void) const { return Vec3(z, z, y); }
Vec3 Vec3::zzz(void) const { return Vec3(z);       }
