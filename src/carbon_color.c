// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CBN_COLOR__R(c)    (((c) >> 24) & 0xff)
#define CBN_COLOR__G(c)    (((c) >> 16) & 0xff)
#define CBN_COLOR__B(c)    (((c) >>  8) & 0xff)
#define CBN_COLOR__A(c)    (((c) >>  0) & 0xff)
#define CBN_COLOR__RGBA(c) (CBN_Color){.r=CBN_COLOR__R(c),.g=CBN_COLOR__G(c),.b=CBN_COLOR__B(c),.a=CBN_COLOR__A(c)}
#define CBN_COLOR__U32_4(r, g, b, a) (((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | ((a & 0xff) << 0))
#define CBN_COLOR__U32_1(rgba) CBN_COLOR__U32_4(rgba.r, rgba.g, rgba.b, rgba.a)

u32 carbon_color_from_hsv(f32 h, f32 s, f32 v) {
  f32 k = carbon_math_fmod(5 + h/60, 6);
  k = carbon_math_clamp(carbon_math_min(4 - k, k), 0, 1);
  u32 r = (v - v*s*k) * 0xff;
  k = carbon_math_fmod(3 + h/60, 6);
  k = carbon_math_clamp(carbon_math_min(4 - k, k), 0, 1);
  u32 g = (v - v*s*k) * 0xff;
  k = carbon_math_fmod(1 + h/60, 6);
  k = carbon_math_clamp(carbon_math_min(4 - k, k), 0, 1);
  u32 b = (v - v*s*k) * 0xff;
  return CBN_COLOR__U32_4(r, g, b, 0xff);
}

CBN_Vec3 carbon_color_to_hsv(u32 color) {
  CBN_Vec3 hsv = {0};
  f32 r = (f32)CBN_COLOR__R(color)/0xff;
  f32 g = (f32)CBN_COLOR__G(color)/0xff;
  f32 b = (f32)CBN_COLOR__B(color)/0xff;
  f32 min = carbon_math_min_3(r, g, b), max = carbon_math_max_3(r, g, b);
  f32 delta = max - min;
  hsv.z = max;
  if (delta < CARBON_EPS || max <= 0) {
    hsv.x = hsv.y = 0;
    return hsv;
  }
  hsv.y = delta/max;
  if (r >= max) hsv.x = (g - b)/delta;
  else if (g >= max) hsv.x = 2 + (b - r)/delta;
  else hsv.x = 4 + (r - g)/delta;
  hsv.x *= 60;
  if (hsv.x < 0) hsv.x += 360;
  return hsv;
}

u32 carbon_color_scale(u32 color, f32 s) {
  CBN_Color rgba = CBN_COLOR__RGBA(color);
  rgba.r = carbon_math_clamp(rgba.r * s, 0, 0xff);
  rgba.g = carbon_math_clamp(rgba.g * s, 0, 0xff);
  rgba.b = carbon_math_clamp(rgba.b * s, 0, 0xff);
  return CBN_COLOR__U32_1(rgba);
}

u32 carbon_color_add(u32 c1, u32 c2) {
  CBN_Color c1_rgba = CBN_COLOR__RGBA(c1), c2_rgba = CBN_COLOR__RGBA(c2);
  u32 r = carbon_math_clamp(c1_rgba.r + c2_rgba.r, 0, 0xff);
  u32 g = carbon_math_clamp(c1_rgba.g + c2_rgba.g, 0, 0xff);
  u32 b = carbon_math_clamp(c1_rgba.b + c2_rgba.b, 0, 0xff);
  u32 a = carbon_math_clamp(c1_rgba.a + c2_rgba.a, 0, 0xff);
  return CBN_COLOR__U32_4(r, g, b, a);
}

u32 carbon_color_lerp(u32 c1, u32 c2, f32 t) {
  CBN_Color c1_rgba = CBN_COLOR__RGBA(c1), c2_rgba = CBN_COLOR__RGBA(c2);
  u32 r = carbon_math_lerp(c1_rgba.r, c2_rgba.r, t);
  u32 g = carbon_math_lerp(c1_rgba.g, c2_rgba.g, t);
  u32 b = carbon_math_lerp(c1_rgba.b, c2_rgba.b, t);
  u32 a = carbon_math_lerp(c1_rgba.a, c2_rgba.a, t);
  return CBN_COLOR__U32_4(r, g, b, a);
}

u32 carbon_color_complementary(u32 color) {
  CBN_Vec3 hsv = carbon_color_to_hsv(color);
  hsv.x = carbon_math_fmod(hsv.x + 180, 360);
  return carbon_color_from_hsv(hsv.x, hsv.y, hsv.z);
}

CBN_Vec4 carbon_color_to_normalized(u32 color) {
  CBN_Color rgba = CBN_COLOR__RGBA(color);
  return carbon_math_vec4((f32)rgba.r/0xff, (f32)rgba.g/0xff, (f32)rgba.b/0xff, (f32)rgba.a/0xff);
}

u32 carbon_color_from_normalized(CBN_Vec4 v) {
  v = carbon_math_vec4_clamp(v, carbon_math_vec4_1(0), carbon_math_vec4_1(1));
  u32 r = carbon_math_clamp(v.x * 0xff, 0, 0xff);
  u32 g = carbon_math_clamp(v.y * 0xff, 0, 0xff);
  u32 b = carbon_math_clamp(v.z * 0xff, 0, 0xff);
  // u32 a = carbon_math_clamp(v.w * 0xff, 0, 0xff);
  return CBN_COLOR__U32_4(r, g, b, 0xff);
}
