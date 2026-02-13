#ifdef __cplusplus

struct CBN_Vec3;  // Forward declaration
struct CBN_Vec4;  // Forward declaration

struct CBN_Vec2 : CBN_Vec2_t {
  constexpr CBN_Vec2(f32 _x, f32 _y) : CBN_Vec2_t{.c = {_x, _y}} {}
  constexpr CBN_Vec2(f32 _x = 0) : CBN_Vec2(_x, _x) {}

  CBN_Vec2 operator+(const CBN_Vec2 &v) const {
    return carbon_math_vec2_add(*this, v);
  }
  
  CBN_Vec2 operator+(f32 s) const {
    return CBN_Vec2(x + s, y + s);
  }
  
  friend CBN_Vec2 operator+(f32 s, const CBN_Vec2 &v) {
    return v + s;
  }
  
  CBN_Vec2 &operator+=(const CBN_Vec2 &v) {
    *this = *this + v;
    return *this;
  }
  
  CBN_Vec2 &operator+=(f32 s) {
    *this = *this + s;
    return *this;
  }
  
  CBN_Vec2 operator-(const CBN_Vec2 &v) const {
    return carbon_math_vec2_sub(*this, v);
  }
  
  CBN_Vec2 operator-(f32 s) const {
    return CBN_Vec2(x - s, y - s);
  }
  
  friend CBN_Vec2 operator-(f32 s, const CBN_Vec2 &v) {
    return -v + s;
  }

  CBN_Vec2 operator-(void) const {
    return *this * -1;
  }
  
  void operator-=(const CBN_Vec2 &v) {
    *this = *this - v;
  }

  CBN_Vec2 operator*(const CBN_Vec2 &v) const {
    return carbon_math_vec2_mult(*this, v);
  }
  
  CBN_Vec2 operator*(f32 s) const {
    return carbon_math_vec2_scale(*this, s);
  }
  
  friend CBN_Vec2 operator*(f32 s, const CBN_Vec2 &v) {
    return v * s;
  }
  
  void operator*=(f32 s) {
    *this = *this * s;
  }

  CBN_Vec2 operator/(const CBN_Vec2 &v) const;

  CBN_Vec2 operator/(f32 s) const {
    return carbon_math_vec2_scale(*this, 1/s);
  }
  
  void operator/=(f32 s) {
    *this = *this / s;
  }
  
  f32 Dot(const CBN_Vec2 &v) const {
    return carbon_math_vec2_dot(*this, v);
  }
  
  static f32 Dot(const CBN_Vec2 &u, const CBN_Vec2 &v) {
    return u.Dot(v);
  }
  
  f32 Length(void) const {
    return carbon_math_vec2_len(*this);
  }

  f32 LengthSquared(void) const {
    return carbon_math_vec2_len_squared(*this);
  }

  CBN_Vec2 Normalize(void) const {
    return carbon_math_vec2_norm(*this);
  }

  void Clamp(const CBN_Vec2 &min, const CBN_Vec2 &max) {
    *this = carbon_math_vec2_clamp(*this, min, max);
  }

  CBN_Vec2 Floor(void) const {
    return carbon_math_vec2_floor(*this);
  }

  CBN_Vec2 Abs(void) const {
    return carbon_math_vec2_abs(*this);
  }

  CBN_Vec2 Exp(void) const {
    return carbon_math_vec2_exp(*this);
  }

  CBN_Vec2 Sin(void) const {
    return carbon_math_vec2_sin(*this);
  }

  CBN_Vec2 Cos(void) const {
    return carbon_math_vec2_cos(*this);
  }

  CBN_Vec2 Tan(void) const {
    return carbon_math_vec2_tan(*this);
  }

  CBN_Vec2 Tanh(void) const {
    return carbon_math_vec2_tanh(*this);
  }

  CBN_Vec2 Lerp(const CBN_Vec2 &v, f32 t) const {
    return carbon_math_vec2_lerp(*this, v, t);
  }

  CBN_Vec2 Rotate(f32 angle) const {
    return carbon_math_vec2_rotate(*this, angle);
  }

  CBN_Vec2 RotatePivot(f32 angle, const CBN_Vec2 &pivot) const {
    return carbon_math_vec2_rotate_around_pivot(*this, angle, pivot);
  }

  const char *ToString(void) const {
    return carbon_math_vec2_to_cstr(*this);
  }

  CBN_Vec2 xx(void)   const { return CBN_Vec2(x);          }
  CBN_Vec2 xy(void)   const { return CBN_Vec2(x, y);       }
  CBN_Vec2 yx(void)   const { return CBN_Vec2(y, x);       }
  CBN_Vec2 yy(void)   const { return CBN_Vec2(y);          }
  //CBN_Vec3 xxx(void)  const { return CBN_Vec3(x);          }
  //CBN_Vec3 xxy(void)  const { return CBN_Vec3(x, x, y);    }
  //CBN_Vec3 xyx(void)  const { return CBN_Vec3(x, y, x);    }
  //CBN_Vec3 xyy(void)  const { return CBN_Vec3(x, y, y);    }
  //CBN_Vec3 yxx(void)  const { return CBN_Vec3(y, x, x);    }
  //CBN_Vec3 yxy(void)  const { return CBN_Vec3(y, x, y);    }
  //CBN_Vec3 yyx(void)  const { return CBN_Vec3(y, y, x);    }
  //CBN_Vec3 yyy(void)  const { return CBN_Vec3(y);          }
  CBN_Vec4 xyyx(void) const;
};

#endif
