// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct alignas(16) CBN_Vec4 : CBN_Vec4_t {
  constexpr CBN_Vec4(f32 _x, f32 _y, f32 _z, f32 _w) : CBN_Vec4_t{.c = {_x, _y, _z, _w}} {}
  constexpr CBN_Vec4(f32 _x = 0) : CBN_Vec4(_x, _x, _x, _x) {}

  CBN_Vec4 operator+(CBN_Vec4 v) const { return carbon_math_vec4_add(*this, v); }
  
  CBN_Vec4 operator+(f32 s) const { return CBN_Vec4(x + s, y + s, z + s, w + s); }
  friend CBN_Vec4 operator+(f32 s, CBN_Vec4 v) { return v + s; }
  
  CBN_Vec4 &operator+=(CBN_Vec4 v) { return *this = *this + v; }
  
  CBN_Vec4 &operator+=(f32 s) { return *this = *this + s; }

  CBN_Vec4 operator-(f32 s) const { return CBN_Vec4(x - s, y - s, z - s, w - s); }
  friend CBN_Vec4 operator-(f32 s, CBN_Vec4 v) { return -v + s; }

  CBN_Vec4 operator-(void) const { return *this * -1; }
  
  CBN_Vec4 operator*(CBN_Vec4 v) const { return carbon_math_vec4_mult(*this, v); }
  
  CBN_Vec4 operator*(f32 s) const { return carbon_math_vec4_scale(*this, s); }
  
  friend CBN_Vec4 operator*(f32 s, CBN_Vec4 v) { return v * s; }

  CBN_Vec4 operator/(CBN_Vec4 v) const { return carbon_math_vec4_mult(*this, 1/v); }
  
  friend CBN_Vec4 operator/(f32 s, CBN_Vec4 v) { return carbon_math_vec4(s/v.x, s/v.y, s/v.z, s/v.w); }

  void Clamp(CBN_Vec4 min, CBN_Vec4 max) { *this = carbon_math_vec4_clamp(*this, min, max); }

  CBN_Vec4 Map(auto &&f) const { return carbon_math_vec4_map(*this, f); }

  CBN_Vec4 Lerp(CBN_Vec4 v, f32 t) const { return carbon_math_vec4_lerp(*this, v, t); }

  const char *ToString(void) const { return carbon_math_vec4_to_cstr(*this); }
};

CBN_Vec4 CBN_Vec2::xyyx(void) const { return CBN_Vec4(x, y, y, x); }

#endif
