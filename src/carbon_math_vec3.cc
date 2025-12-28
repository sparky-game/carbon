// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

cbn::math::Vec3 cbn::math::Vec3::Cross(const cbn::math::Vec3 &v) const {
  return carbon_math_vec3_cross(*this, v);
}

cbn::math::Vec3 cbn::math::Vec3::Lerp(const cbn::math::Vec3 &v, f32 t) const {
  return carbon_math_vec3_lerp(*this, v, t);
}

cbn::math::Quat cbn::math::Vec3::ToQuat(void) const {
  return carbon_math_quat_from_euler(*this);
}

const char *cbn::math::Vec3::ToString(void) const {
  return carbon_math_vec3_to_cstr(*this);
}

cbn::math::Vec3 cbn::math::Vec3::RotateX(f32 angle) const {
  return carbon_math_vec3_rotate_x(*this, angle);
}

cbn::math::Vec3 cbn::math::Vec3::RotateY(f32 angle) const {
  return carbon_math_vec3_rotate_y(*this, angle);
}

cbn::math::Vec3 cbn::math::Vec3::RotateZ(f32 angle) const {
  return carbon_math_vec3_rotate_z(*this, angle);
}

cbn::math::Vec3 cbn::math::Vec3::Rotate(cbn::math::Quat q) const {
  return carbon_math_vec3_rotate(*this, q);
}

cbn::Opt<cbn::math::Vec2> cbn::math::Vec3::Project2D(f32 near_z) const {
  cbn::math::Vec2 v;
  if (!carbon_math_vec3_project_2d(*this, near_z, &v)) return {};
  return v;
}

cbn::math::Vec3 cbn::math::Vec3::operator+(const cbn::math::Vec3 &v) const {
  return carbon_math_vec3_add(*this, v);
}

cbn::math::Vec3 cbn::math::Vec3::operator+(f32 s) const {
  return CARBON_VEC3(x + s, y + s, z + s);
}

cbn::math::Vec3 operator+(f32 s, const cbn::math::Vec3 &v) {
  return v + s;
}

cbn::math::Vec3 &cbn::math::Vec3::operator+=(const cbn::math::Vec3 &v) {
  *this = *this + v;
  return *this;
}

cbn::math::Vec3 &cbn::math::Vec3::operator+=(f32 s) {
  *this = *this + s;
  return *this;
}

cbn::math::Vec3 cbn::math::Vec3::operator-(const cbn::math::Vec3 &v) const {
  return carbon_math_vec3_sub(*this, v);
}

f32 cbn::math::Vec3::operator*(const cbn::math::Vec3 &v) const {
  return carbon_math_vec3_dot(*this, v);
}

cbn::math::Vec2 cbn::math::Vec3::xx(void)  const { return CARBON_VEC_xx(*this);  }
cbn::math::Vec2 cbn::math::Vec3::xy(void)  const { return CARBON_VEC_xy(*this);  }
cbn::math::Vec2 cbn::math::Vec3::xz(void)  const { return CARBON_VEC_xz(*this);  }
cbn::math::Vec2 cbn::math::Vec3::yx(void)  const { return CARBON_VEC_yx(*this);  }
cbn::math::Vec2 cbn::math::Vec3::yy(void)  const { return CARBON_VEC_yy(*this);  }
cbn::math::Vec2 cbn::math::Vec3::yz(void)  const { return CARBON_VEC_yz(*this);  }
cbn::math::Vec2 cbn::math::Vec3::zx(void)  const { return CARBON_VEC_zx(*this);  }
cbn::math::Vec2 cbn::math::Vec3::zy(void)  const { return CARBON_VEC_zy(*this);  }
cbn::math::Vec2 cbn::math::Vec3::zz(void)  const { return CARBON_VEC_zz(*this);  }
cbn::math::Vec3 cbn::math::Vec3::xxx(void) const { return CARBON_VEC_xxx(*this); }
cbn::math::Vec3 cbn::math::Vec3::xxy(void) const { return CARBON_VEC_xxy(*this); }
cbn::math::Vec3 cbn::math::Vec3::xxz(void) const { return CARBON_VEC_xxz(*this); }
cbn::math::Vec3 cbn::math::Vec3::xyx(void) const { return CARBON_VEC_xyx(*this); }
cbn::math::Vec3 cbn::math::Vec3::xyy(void) const { return CARBON_VEC_xyy(*this); }
cbn::math::Vec3 cbn::math::Vec3::xyz(void) const { return CARBON_VEC_xyz(*this); }
cbn::math::Vec3 cbn::math::Vec3::xzx(void) const { return CARBON_VEC_xzx(*this); }
cbn::math::Vec3 cbn::math::Vec3::xzy(void) const { return CARBON_VEC_xzy(*this); }
cbn::math::Vec3 cbn::math::Vec3::xzz(void) const { return CARBON_VEC_xzz(*this); }
cbn::math::Vec3 cbn::math::Vec3::yxx(void) const { return CARBON_VEC_yxx(*this); }
cbn::math::Vec3 cbn::math::Vec3::yxy(void) const { return CARBON_VEC_yxy(*this); }
cbn::math::Vec3 cbn::math::Vec3::yxz(void) const { return CARBON_VEC_yxz(*this); }
cbn::math::Vec3 cbn::math::Vec3::yyx(void) const { return CARBON_VEC_yyx(*this); }
cbn::math::Vec3 cbn::math::Vec3::yyy(void) const { return CARBON_VEC_yyy(*this); }
cbn::math::Vec3 cbn::math::Vec3::yyz(void) const { return CARBON_VEC_yyz(*this); }
cbn::math::Vec3 cbn::math::Vec3::yzx(void) const { return CARBON_VEC_yzx(*this); }
cbn::math::Vec3 cbn::math::Vec3::yzy(void) const { return CARBON_VEC_yzy(*this); }
cbn::math::Vec3 cbn::math::Vec3::yzz(void) const { return CARBON_VEC_yzz(*this); }
cbn::math::Vec3 cbn::math::Vec3::zxx(void) const { return CARBON_VEC_zxx(*this); }
cbn::math::Vec3 cbn::math::Vec3::zxy(void) const { return CARBON_VEC_zxy(*this); }
cbn::math::Vec3 cbn::math::Vec3::zxz(void) const { return CARBON_VEC_zxz(*this); }
cbn::math::Vec3 cbn::math::Vec3::zyx(void) const { return CARBON_VEC_zyx(*this); }
cbn::math::Vec3 cbn::math::Vec3::zyy(void) const { return CARBON_VEC_zyy(*this); }
cbn::math::Vec3 cbn::math::Vec3::zyz(void) const { return CARBON_VEC_zyz(*this); }
cbn::math::Vec3 cbn::math::Vec3::zzx(void) const { return CARBON_VEC_zzx(*this); }
cbn::math::Vec3 cbn::math::Vec3::zzy(void) const { return CARBON_VEC_zzy(*this); }
cbn::math::Vec3 cbn::math::Vec3::zzz(void) const { return CARBON_VEC_zzz(*this); }
