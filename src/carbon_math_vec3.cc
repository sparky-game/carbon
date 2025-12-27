// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

cbn::Vec3 cbn::Vec3::Cross(const cbn::Vec3 &v) const {
  return carbon_math_vec3_cross(*this, v);
}

cbn::Vec3 cbn::Vec3::Lerp(const cbn::Vec3 &v, f32 t) const {
  return carbon_math_vec3_lerp(*this, v, t);
}

cbn::Quat cbn::Vec3::ToQuat(void) const {
  return carbon_math_quat_from_euler(*this);
}

const char *cbn::Vec3::ToString(void) const {
  return carbon_math_vec3_to_cstr(*this);
}

cbn::Vec3 cbn::Vec3::RotateX(f32 angle) const {
  return carbon_math_vec3_rotate_x(*this, angle);
}

cbn::Vec3 cbn::Vec3::RotateY(f32 angle) const {
  return carbon_math_vec3_rotate_y(*this, angle);
}

cbn::Vec3 cbn::Vec3::RotateZ(f32 angle) const {
  return carbon_math_vec3_rotate_z(*this, angle);
}

cbn::Vec3 cbn::Vec3::Rotate(cbn::Quat q) const {
  return carbon_math_vec3_rotate(*this, q);
}

cbn::Opt<cbn::Vec2> cbn::Vec3::Project2D(f32 near_z) const {
  cbn::Vec2 v;
  if (!carbon_math_vec3_project_2d(*this, near_z, &v)) return {};
  return v;
}

cbn::Vec3 cbn::Vec3::operator+(const cbn::Vec3 &v) const {
  return carbon_math_vec3_add(*this, v);
}

cbn::Vec3 cbn::Vec3::operator+(f32 s) const {
  return CARBON_VEC3(x + s, y + s, z + s);
}

cbn::Vec3 operator+(f32 s, const cbn::Vec3 &v) {
  return v + s;
}

cbn::Vec3 &cbn::Vec3::operator+=(const cbn::Vec3 &v) {
  *this = *this + v;
  return *this;
}

cbn::Vec3 &cbn::Vec3::operator+=(f32 s) {
  *this = *this + s;
  return *this;
}

cbn::Vec3 cbn::Vec3::operator-(const cbn::Vec3 &v) const {
  return carbon_math_vec3_sub(*this, v);
}

f32 cbn::Vec3::operator*(const cbn::Vec3 &v) const {
  return carbon_math_vec3_dot(*this, v);
}

cbn::Vec2 cbn::Vec3::xx(void)  const { return CARBON_VEC_xx(*this);  }
cbn::Vec2 cbn::Vec3::xy(void)  const { return CARBON_VEC_xy(*this);  }
cbn::Vec2 cbn::Vec3::xz(void)  const { return CARBON_VEC_xz(*this);  }
cbn::Vec2 cbn::Vec3::yx(void)  const { return CARBON_VEC_yx(*this);  }
cbn::Vec2 cbn::Vec3::yy(void)  const { return CARBON_VEC_yy(*this);  }
cbn::Vec2 cbn::Vec3::yz(void)  const { return CARBON_VEC_yz(*this);  }
cbn::Vec2 cbn::Vec3::zx(void)  const { return CARBON_VEC_zx(*this);  }
cbn::Vec2 cbn::Vec3::zy(void)  const { return CARBON_VEC_zy(*this);  }
cbn::Vec2 cbn::Vec3::zz(void)  const { return CARBON_VEC_zz(*this);  }
cbn::Vec3 cbn::Vec3::xxx(void) const { return CARBON_VEC_xxx(*this); }
cbn::Vec3 cbn::Vec3::xxy(void) const { return CARBON_VEC_xxy(*this); }
cbn::Vec3 cbn::Vec3::xxz(void) const { return CARBON_VEC_xxz(*this); }
cbn::Vec3 cbn::Vec3::xyx(void) const { return CARBON_VEC_xyx(*this); }
cbn::Vec3 cbn::Vec3::xyy(void) const { return CARBON_VEC_xyy(*this); }
cbn::Vec3 cbn::Vec3::xyz(void) const { return CARBON_VEC_xyz(*this); }
cbn::Vec3 cbn::Vec3::xzx(void) const { return CARBON_VEC_xzx(*this); }
cbn::Vec3 cbn::Vec3::xzy(void) const { return CARBON_VEC_xzy(*this); }
cbn::Vec3 cbn::Vec3::xzz(void) const { return CARBON_VEC_xzz(*this); }
cbn::Vec3 cbn::Vec3::yxx(void) const { return CARBON_VEC_yxx(*this); }
cbn::Vec3 cbn::Vec3::yxy(void) const { return CARBON_VEC_yxy(*this); }
cbn::Vec3 cbn::Vec3::yxz(void) const { return CARBON_VEC_yxz(*this); }
cbn::Vec3 cbn::Vec3::yyx(void) const { return CARBON_VEC_yyx(*this); }
cbn::Vec3 cbn::Vec3::yyy(void) const { return CARBON_VEC_yyy(*this); }
cbn::Vec3 cbn::Vec3::yyz(void) const { return CARBON_VEC_yyz(*this); }
cbn::Vec3 cbn::Vec3::yzx(void) const { return CARBON_VEC_yzx(*this); }
cbn::Vec3 cbn::Vec3::yzy(void) const { return CARBON_VEC_yzy(*this); }
cbn::Vec3 cbn::Vec3::yzz(void) const { return CARBON_VEC_yzz(*this); }
cbn::Vec3 cbn::Vec3::zxx(void) const { return CARBON_VEC_zxx(*this); }
cbn::Vec3 cbn::Vec3::zxy(void) const { return CARBON_VEC_zxy(*this); }
cbn::Vec3 cbn::Vec3::zxz(void) const { return CARBON_VEC_zxz(*this); }
cbn::Vec3 cbn::Vec3::zyx(void) const { return CARBON_VEC_zyx(*this); }
cbn::Vec3 cbn::Vec3::zyy(void) const { return CARBON_VEC_zyy(*this); }
cbn::Vec3 cbn::Vec3::zyz(void) const { return CARBON_VEC_zyz(*this); }
cbn::Vec3 cbn::Vec3::zzx(void) const { return CARBON_VEC_zzx(*this); }
cbn::Vec3 cbn::Vec3::zzy(void) const { return CARBON_VEC_zzy(*this); }
cbn::Vec3 cbn::Vec3::zzz(void) const { return CARBON_VEC_zzz(*this); }
