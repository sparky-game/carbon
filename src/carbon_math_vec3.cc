// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec3 CBN_Vec3::Cross(const CBN_Vec3 &v) const {
  return carbon_math_vec3_cross(*this, v);
}

CBN_Vec3 CBN_Vec3::Lerp(const CBN_Vec3 &v, f32 t) const {
  return carbon_math_vec3_lerp(*this, v, t);
}

const char *CBN_Vec3::ToString(void) const {
  return carbon_math_vec3_to_cstr(*this);
}

CBN_Vec3 CBN_Vec3::RotateX(f32 angle) const {
  return carbon_math_vec3_rotate_x(*this, angle);
}

CBN_Vec3 CBN_Vec3::RotateY(f32 angle) const {
  return carbon_math_vec3_rotate_y(*this, angle);
}

CBN_Vec3 CBN_Vec3::RotateZ(f32 angle) const {
  return carbon_math_vec3_rotate_z(*this, angle);
}

CBN_Vec3 CBN_Vec3::Rotate(CBN_Quat q) const {
  return carbon_math_vec3_rotate(*this, q);
}

cbn::Opt<CBN_Vec2> CBN_Vec3::Project2D(f32 near_z) const {
  CBN_Vec2 v;
  if (!carbon_math_vec3_project_2d(*this, near_z, &v)) return {};
  return v;
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

CBN_Vec2 CBN_Vec3::xx(void)  const { return CARBON_VEC_xx(*this);  }
CBN_Vec2 CBN_Vec3::xy(void)  const { return CARBON_VEC_xy(*this);  }
CBN_Vec2 CBN_Vec3::xz(void)  const { return CARBON_VEC_xz(*this);  }
CBN_Vec2 CBN_Vec3::yx(void)  const { return CARBON_VEC_yx(*this);  }
CBN_Vec2 CBN_Vec3::yy(void)  const { return CARBON_VEC_yy(*this);  }
CBN_Vec2 CBN_Vec3::yz(void)  const { return CARBON_VEC_yz(*this);  }
CBN_Vec2 CBN_Vec3::zx(void)  const { return CARBON_VEC_zx(*this);  }
CBN_Vec2 CBN_Vec3::zy(void)  const { return CARBON_VEC_zy(*this);  }
CBN_Vec2 CBN_Vec3::zz(void)  const { return CARBON_VEC_zz(*this);  }
CBN_Vec3 CBN_Vec3::xxx(void) const { return CARBON_VEC_xxx(*this); }
CBN_Vec3 CBN_Vec3::xxy(void) const { return CARBON_VEC_xxy(*this); }
CBN_Vec3 CBN_Vec3::xxz(void) const { return CARBON_VEC_xxz(*this); }
CBN_Vec3 CBN_Vec3::xyx(void) const { return CARBON_VEC_xyx(*this); }
CBN_Vec3 CBN_Vec3::xyy(void) const { return CARBON_VEC_xyy(*this); }
CBN_Vec3 CBN_Vec3::xyz(void) const { return CARBON_VEC_xyz(*this); }
CBN_Vec3 CBN_Vec3::xzx(void) const { return CARBON_VEC_xzx(*this); }
CBN_Vec3 CBN_Vec3::xzy(void) const { return CARBON_VEC_xzy(*this); }
CBN_Vec3 CBN_Vec3::xzz(void) const { return CARBON_VEC_xzz(*this); }
CBN_Vec3 CBN_Vec3::yxx(void) const { return CARBON_VEC_yxx(*this); }
CBN_Vec3 CBN_Vec3::yxy(void) const { return CARBON_VEC_yxy(*this); }
CBN_Vec3 CBN_Vec3::yxz(void) const { return CARBON_VEC_yxz(*this); }
CBN_Vec3 CBN_Vec3::yyx(void) const { return CARBON_VEC_yyx(*this); }
CBN_Vec3 CBN_Vec3::yyy(void) const { return CARBON_VEC_yyy(*this); }
CBN_Vec3 CBN_Vec3::yyz(void) const { return CARBON_VEC_yyz(*this); }
CBN_Vec3 CBN_Vec3::yzx(void) const { return CARBON_VEC_yzx(*this); }
CBN_Vec3 CBN_Vec3::yzy(void) const { return CARBON_VEC_yzy(*this); }
CBN_Vec3 CBN_Vec3::yzz(void) const { return CARBON_VEC_yzz(*this); }
CBN_Vec3 CBN_Vec3::zxx(void) const { return CARBON_VEC_zxx(*this); }
CBN_Vec3 CBN_Vec3::zxy(void) const { return CARBON_VEC_zxy(*this); }
CBN_Vec3 CBN_Vec3::zxz(void) const { return CARBON_VEC_zxz(*this); }
CBN_Vec3 CBN_Vec3::zyx(void) const { return CARBON_VEC_zyx(*this); }
CBN_Vec3 CBN_Vec3::zyy(void) const { return CARBON_VEC_zyy(*this); }
CBN_Vec3 CBN_Vec3::zyz(void) const { return CARBON_VEC_zyz(*this); }
CBN_Vec3 CBN_Vec3::zzx(void) const { return CARBON_VEC_zzx(*this); }
CBN_Vec3 CBN_Vec3::zzy(void) const { return CARBON_VEC_zzy(*this); }
CBN_Vec3 CBN_Vec3::zzz(void) const { return CARBON_VEC_zzz(*this); }
