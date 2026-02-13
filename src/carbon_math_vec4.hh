#ifdef __cplusplus

struct alignas(16) CBN_Vec4 : CBN_Vec4_t {
  constexpr CBN_Vec4(f32 _x, f32 _y, f32 _z, f32 _w) : CBN_Vec4_t{.c = {_x, _y, _z, _w}} {}
  constexpr CBN_Vec4(f32 _x = 0) : CBN_Vec4(_x, _x, _x, _x) {}

  CBN_Vec4 operator+(const CBN_Vec4 &v) const {
    return carbon_math_vec4_add(*this, v);
  }
  
  CBN_Vec4 operator+(f32 s) const {
    return CBN_Vec4(x + s, y + s, z + s, w + s);
  }
  
  friend CBN_Vec4 operator+(f32 s, const CBN_Vec4 &v) {
    return v + s;
  }
  
  CBN_Vec4 &operator+=(const CBN_Vec4 &v) {
    *this = *this + v;
    return *this;
  }
  
  CBN_Vec4 &operator+=(f32 s) {
    *this = *this + s;
    return *this;
  }

  CBN_Vec4 operator-(f32 s) const {
    return CBN_Vec4(x - s, y - s, z - s, w - s);
  }

  CBN_Vec4 operator-(void) const {
    return *this * -1;
  }
  
  friend CBN_Vec4 operator-(f32 s, const CBN_Vec4 &v) {
    return -v + s;
  }

  CBN_Vec4 operator*(const CBN_Vec4 &v) const;
  
  CBN_Vec4 operator*(f32 s) const {
    return carbon_math_vec4_scale(*this, s);
  }
  
  friend CBN_Vec4 operator*(f32 s, const CBN_Vec4 &v) {
    return v * s;
  }

  CBN_Vec4 operator/(const CBN_Vec4 &v) const {
    return carbon_math_vec4_mult(*this, 1/v);
  }
  
  friend CBN_Vec4 operator/(f32 s, const CBN_Vec4 &v) {
    return carbon_math_vec4(s/v.x, s/v.y, s/v.z, s/v.w);
  }

  void Clamp(const CBN_Vec4 &min, const CBN_Vec4 &max) {
    *this = carbon_math_vec4_clamp(*this, min, max);
  }

  CBN_Vec4 Abs(void) const {
    return carbon_math_vec4_abs(*this);
  }

  CBN_Vec4 Exp(void) const {
    return carbon_math_vec4_exp(*this);
  }

  CBN_Vec4 Sin(void) const {
    return carbon_math_vec4_sin(*this);
  }

  CBN_Vec4 Cos(void) const {
    return carbon_math_vec4_cos(*this);
  }

  CBN_Vec4 Tan(void) const {
    return carbon_math_vec4_tan(*this);
  }

  CBN_Vec4 Tanh(void) const {
    return carbon_math_vec4_tanh(*this);
  }

  CBN_Vec4 Lerp(const CBN_Vec4 &v, f32 t) const {
    return carbon_math_vec4_lerp(*this, v, t);
  }

  const char *ToString(void) const {
    return carbon_math_vec4_to_cstr(*this);
  }
};

CBN_Vec4 CBN_Vec2::xyyx(void) const { return CBN_Vec4(x, y, y, x); }

#endif
