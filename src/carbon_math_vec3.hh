#ifdef __cplusplus

struct CBN_Vec3 : CBN_Vec3_t {
  constexpr CBN_Vec3(f32 _x, f32 _y, f32 _z) : CBN_Vec3_t{_x, _y, _z} {}
  constexpr CBN_Vec3(f32 _x = 0) : CBN_Vec3(_x, _x, _x) {}

  CBN_Vec3 operator+(const CBN_Vec3 &v) const {
    return carbon_math_vec3_add(*this, v);
  }
  
  CBN_Vec3 operator+(f32 s) const {
    return CBN_Vec3(x + s, y + s, z + s);
  }
  
  friend CBN_Vec3 operator+(f32 s, const CBN_Vec3 &v) {
    return v + s;
  }
  
  CBN_Vec3 &operator+=(const CBN_Vec3 &v) {
    *this = *this + v;
    return *this;
  }
  
  CBN_Vec3 &operator+=(f32 s) {
    *this = *this + s;
    return *this;
  }

  CBN_Vec3 operator-(const CBN_Vec3 &v) const {
    return carbon_math_vec3_sub(*this, v);
  }

  f32 operator*(const CBN_Vec3 &v) const {
    return carbon_math_vec3_dot(*this, v);
  }

  CBN_Vec3 Cross(const CBN_Vec3 &v) const {
    return carbon_math_vec3_cross(*this, v);
  }

  CBN_Vec3 Lerp(const CBN_Vec3 &v, f32 t) const {
    return carbon_math_vec3_lerp(*this, v, t);
  }

  CBN_Quat ToQuat(void) const;
  //{
  //  return carbon_math_quat_from_euler(*this);
  //}

  const char *ToString(void) const {
    return carbon_math_vec3_to_cstr(*this);
  }

  CBN_Vec3 RotateX(f32 angle) const {
    return carbon_math_vec3_rotate_x(*this, angle);
  }

  CBN_Vec3 RotateY(f32 angle) const {
    return carbon_math_vec3_rotate_y(*this, angle);
  }

  CBN_Vec3 RotateZ(f32 angle) const {
    return carbon_math_vec3_rotate_z(*this, angle);
  }

  CBN_Vec3 Rotate(CBN_Quat q) const;
  //{
  //  return carbon_math_vec3_rotate(*this, q);
  //}

  cbn::Opt<CBN_Vec2> Project2D(f32 near_z) const {
    CBN_Vec2 v;
    if (!carbon_math_vec3_project_2d(*this, near_z, &v)) return {};
    return v;
  }

  CBN_Vec2 xx(void)  const { return CBN_Vec2(x);       }
  CBN_Vec2 xz(void)  const { return CBN_Vec2(x, z);    }
  CBN_Vec2 yx(void)  const { return CBN_Vec2(y, x);    }
  CBN_Vec2 yy(void)  const { return CBN_Vec2(y);       }
  CBN_Vec2 zx(void)  const { return CBN_Vec2(z, x);    }
  CBN_Vec2 zy(void)  const { return CBN_Vec2(z, y);    }
  CBN_Vec2 zz(void)  const { return CBN_Vec2(z);       }
  CBN_Vec3 xxx(void) const { return CBN_Vec3(x);       }
  CBN_Vec3 xxy(void) const { return CBN_Vec3(x, x, y); }
  CBN_Vec3 xxz(void) const { return CBN_Vec3(x, x, z); }
  CBN_Vec3 xyx(void) const { return CBN_Vec3(x, y, x); }
  CBN_Vec3 xyy(void) const { return CBN_Vec3(x, y, y); }
  CBN_Vec3 xyz(void) const { return CBN_Vec3(x, y, z); }
  CBN_Vec3 xzx(void) const { return CBN_Vec3(x, z, x); }
  CBN_Vec3 xzy(void) const { return CBN_Vec3(x, z, y); }
  CBN_Vec3 xzz(void) const { return CBN_Vec3(x, z, z); }
  CBN_Vec3 yxx(void) const { return CBN_Vec3(y, x, x); }
  CBN_Vec3 yxy(void) const { return CBN_Vec3(y, x, y); }
  CBN_Vec3 yxz(void) const { return CBN_Vec3(y, x, z); }
  CBN_Vec3 yyx(void) const { return CBN_Vec3(y, y, x); }
  CBN_Vec3 yyy(void) const { return CBN_Vec3(y);       }
  CBN_Vec3 yyz(void) const { return CBN_Vec3(y, y, z); }
  CBN_Vec3 yzx(void) const { return CBN_Vec3(y, z, x); }
  CBN_Vec3 yzy(void) const { return CBN_Vec3(y, z, y); }
  CBN_Vec3 yzz(void) const { return CBN_Vec3(y, z, z); }
  CBN_Vec3 zxx(void) const { return CBN_Vec3(z, x, x); }
  CBN_Vec3 zxy(void) const { return CBN_Vec3(z, x, y); }
  CBN_Vec3 zxz(void) const { return CBN_Vec3(z, x, z); }
  CBN_Vec3 zyx(void) const { return CBN_Vec3(z, y, x); }
  CBN_Vec3 zyy(void) const { return CBN_Vec3(z, y, y); }
  CBN_Vec3 zyz(void) const { return CBN_Vec3(z, y, z); }
  CBN_Vec3 zzx(void) const { return CBN_Vec3(z, z, x); }
  CBN_Vec3 zzy(void) const { return CBN_Vec3(z, z, y); }
  CBN_Vec3 zzz(void) const { return CBN_Vec3(z);       }
};

#endif
