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
  return ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | ((255 & 0xff) << 0);
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
