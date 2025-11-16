// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

u32 carbon_color_from_hsv(f32 h, f32 s, f32 v) {
  f32 k = carbon_math_fmod(5 + h/60, 6);
  k = CARBON_CLAMP(CARBON_MIN(4 - k, k), 0, 1);
  u32 r = (v - v*s*k) * 255;
  k = carbon_math_fmod(3 + h/60, 6);
  k = CARBON_CLAMP(CARBON_MIN(4 - k, k), 0, 1);
  u32 g = (v - v*s*k) * 255;
  k = carbon_math_fmod(1 + h/60, 6);
  k = CARBON_CLAMP(CARBON_MIN(4 - k, k), 0, 1);
  u32 b = (v - v*s*k) * 255;
  return ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (255 & 0xff);
}

CBN_Vec3 carbon_color_to_hsv(u32 color) {
  CBN_Vec3 hsv = {0};
  f32 r = ((color >> 24) & 0xff) / 255.f;
  f32 g = ((color >> 16) & 0xff) / 255.f;
  f32 b = ((color >> 8)  & 0xff) / 255.f;
  f32 min = CARBON_MIN3(r, g, b), max = CARBON_MAX3(r, g, b), delta = max - min;
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
  f32 r = CARBON_CLAMP(((color >> 24) & 0xff) * s, 0, 255);
  f32 g = CARBON_CLAMP(((color >> 16) & 0xff) * s, 0, 255);
  f32 b = CARBON_CLAMP(((color >> 8)  & 0xff) * s, 0, 255);
  u32 a = (color >> 0) & 0xff;
  return ((u32) r << 24) | ((u32) g << 16) | ((u32) b << 8) | a;
}

u32 carbon_color_add(u32 c1, u32 c2) {
  u32 r = CARBON_CLAMP(((c1 >> 24) & 0xff) + ((c2 >> 24) & 0xff), 0, 255);
  u32 g = CARBON_CLAMP(((c1 >> 16) & 0xff) + ((c2 >> 16) & 0xff), 0, 255);
  u32 b = CARBON_CLAMP(((c1 >> 8)  & 0xff) + ((c2 >> 8)  & 0xff), 0, 255);
  u32 a = CARBON_CLAMP(((c1 >> 0)  & 0xff) + ((c2 >> 0)  & 0xff), 0, 255);
  return (r << 24) | (g << 16) | (b << 8) | a;
}

u32 carbon_color_complementary(u32 color) {
  CBN_Vec3 hsv = carbon_color_to_hsv(color);
  hsv.x = carbon_math_fmod(hsv.x + 180, 360);
  return carbon_color_from_hsv(hsv.x, hsv.y, hsv.z);
}

CBN_Vec4 carbon_color_to_normalized(u32 color) {
  f32 r = (color >> 24) & 0xff;
  f32 g = (color >> 16) & 0xff;
  f32 b = (color >> 8)  & 0xff;
  f32 a = (color >> 0)  & 0xff;
  return CARBON_VEC4(r/255, g/255, b/255, a/255);
}

u32 carbon_color_from_normalized(CBN_Vec4 v) {
  v = carbon_math_vec4_clamp(v, CARBON_VEC4_ZERO, CARBON_VEC4_ONE);
  u32 r = CARBON_CLAMP(v.x * 255, 0, 255);
  u32 g = CARBON_CLAMP(v.y * 255, 0, 255);
  u32 b = CARBON_CLAMP(v.z * 255, 0, 255);
  // u32 a = CARBON_CLAMP(v.w * 255, 0, 255);
  return ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (255 & 0xff);
}
