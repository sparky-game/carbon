// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CBN_COLOR__R(c) (((c) >> 24) & 0xff)
#define CBN_COLOR__G(c) (((c) >> 16) & 0xff)
#define CBN_COLOR__B(c) (((c) >>  8) & 0xff)
#define CBN_COLOR__A(c) (((c) >>  0) & 0xff)
#define CBN_COLOR__U32(r, g, b, a) (((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (a & 0xff))

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
  return CBN_COLOR__U32(r, g, b, 0xff);
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
  u32 sf = 0x100 * s;
  u32 r = carbon_math_clamp((CBN_COLOR__R(color) * sf) >> 8, 0, 0xff);
  u32 g = carbon_math_clamp((CBN_COLOR__G(color) * sf) >> 8, 0, 0xff);
  u32 b = carbon_math_clamp((CBN_COLOR__B(color) * sf) >> 8, 0, 0xff);
  u32 a = CBN_COLOR__A(color);
  return CBN_COLOR__U32(r, g, b, a);
}

u32 carbon_color_add(u32 c1, u32 c2) {
  u32 r = carbon_math_clamp(CBN_COLOR__R(c1) + CBN_COLOR__R(c2), 0, 0xff);
  u32 g = carbon_math_clamp(CBN_COLOR__G(c1) + CBN_COLOR__G(c2), 0, 0xff);
  u32 b = carbon_math_clamp(CBN_COLOR__B(c1) + CBN_COLOR__B(c2), 0, 0xff);
  u32 a = carbon_math_clamp(CBN_COLOR__A(c1) + CBN_COLOR__A(c2), 0, 0xff);
  return CBN_COLOR__U32(r, g, b, a);
}

u32 carbon_color_mult(u32 c1, u32 c2) {
  if (!c1 || !c2) return 0;
  if (c1 == 0xffffffff) return c2;
  if (c2 == 0xffffffff) return c1;
  u32 r = CBN_COLOR__R(c1) * CBN_COLOR__R(c2); r = (r + 1 + (r >> 8)) >> 8;
  u32 g = CBN_COLOR__G(c1) * CBN_COLOR__G(c2); g = (g + 1 + (g >> 8)) >> 8;
  u32 b = CBN_COLOR__B(c1) * CBN_COLOR__B(c2); b = (b + 1 + (b >> 8)) >> 8;
  u32 a = CBN_COLOR__A(c1) * CBN_COLOR__A(c2); a = (a + 1 + (a >> 8)) >> 8;
  return CBN_COLOR__U32(r, g, b, a);
}

u32 carbon_color_lerp(u32 c1, u32 c2, f32 t) {
  u32 tf = 0x100 * t, itf = 0x100 - tf;
  u32 r = (CBN_COLOR__R(c1)*itf + CBN_COLOR__R(c2)*tf) >> 8;
  u32 g = (CBN_COLOR__G(c1)*itf + CBN_COLOR__G(c2)*tf) >> 8;
  u32 b = (CBN_COLOR__B(c1)*itf + CBN_COLOR__B(c2)*tf) >> 8;
  u32 a = (CBN_COLOR__A(c1)*itf + CBN_COLOR__A(c2)*tf) >> 8;
  return CBN_COLOR__U32(r, g, b, a);
}

u32 carbon_color_lerp_3(u32 c1, u32 c2, u32 c3, CBN_Vec3 t) {
  u32 r = CBN_COLOR__R(c1)*t.x + CBN_COLOR__R(c2)*t.y + CBN_COLOR__R(c3)*t.z;
  u32 g = CBN_COLOR__G(c1)*t.x + CBN_COLOR__G(c2)*t.y + CBN_COLOR__G(c3)*t.z;
  u32 b = CBN_COLOR__B(c1)*t.x + CBN_COLOR__B(c2)*t.y + CBN_COLOR__B(c3)*t.z;
  u32 a = CBN_COLOR__A(c1)*t.x + CBN_COLOR__A(c2)*t.y + CBN_COLOR__A(c3)*t.z;
  return CBN_COLOR__U32(r, g, b, a);
}

u32 carbon_color_bilerp(u32 c1, u32 c2, u32 c3, u32 c4, CBN_Vec2 t) {
  u32 tx = 0x100 * t.x, itx = 0x100 - tx;
  u32 ty = 0x100 * t.y, ity = 0x100 - ty;
  u32 w1 = (itx*ity) >> 8, w2 = (tx*ity) >> 8, w3 = (itx*ty) >> 8, w4 = (tx*ty) >> 8;
  u32 rb1 = c1 & 0x00ff00ff, ag1 = (c1 >> 8) & 0x00ff00ff;
  u32 rb2 = c2 & 0x00ff00ff, ag2 = (c2 >> 8) & 0x00ff00ff;
  u32 rb3 = c3 & 0x00ff00ff, ag3 = (c3 >> 8) & 0x00ff00ff;
  u32 rb4 = c4 & 0x00ff00ff, ag4 = (c4 >> 8) & 0x00ff00ff;
  u32 rb = (rb1*w1 + rb2*w2 + rb3*w3 + rb4*w4) >> 8, ag = (ag1*w1 + ag2*w2 + ag3*w3 + ag4*w4) >> 8;
  return (rb & 0x00ff00ff) | ((ag & 0x00ff00ff) << 8);
}

u32 carbon_color_complementary(u32 color) {
  CBN_Vec3 hsv = carbon_color_to_hsv(color);
  hsv.x = carbon_math_fmod(hsv.x + 180, 360);
  return carbon_color_from_hsv(hsv.x, hsv.y, hsv.z);
}

CBN_Vec4 carbon_color_to_normalized(u32 color) {
  u32 r = CBN_COLOR__R(color);
  u32 g = CBN_COLOR__G(color);
  u32 b = CBN_COLOR__B(color);
  u32 a = CBN_COLOR__A(color);
  return carbon_math_vec4((f32)r/0xff, (f32)g/0xff, (f32)b/0xff, (f32)a/0xff);
}

u32 carbon_color_from_normalized(CBN_Vec4 v) {
  v = carbon_math_vec4_clamp(v, carbon_math_vec4_1(0), carbon_math_vec4_1(1));
  u32 r = carbon_math_clamp(v.x * 0xff, 0, 0xff);
  u32 g = carbon_math_clamp(v.y * 0xff, 0, 0xff);
  u32 b = carbon_math_clamp(v.z * 0xff, 0, 0xff);
  return CBN_COLOR__U32(r, g, b, 0xff);
}
