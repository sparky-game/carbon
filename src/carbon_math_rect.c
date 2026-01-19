// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Rect carbon_math_rect(f32 x, f32 y, f32 w, f32 h) {
  return (CBN_Rect){x, y, w, h};
}

CBN_Rect carbon_math_rect_v(CBN_Vec2 v, f32 w, f32 h) {
  return carbon_math_rect(v.x, v.y, w, h);
}

CBN_Rect carbon_math_rect_sq(f32 x, f32 y, f32 sz) {
  return carbon_math_rect(x, y, sz, sz);
}

CBN_Rect carbon_math_rect_sq_v(CBN_Vec2 v, f32 sz) {
  return carbon_math_rect_sq(v.x, v.y, sz);
}

bool carbon_math_rect_contains(CBN_Rect r, CBN_Vec2 p) {
  return (p.x >= r.x && p.x <= r.x + r.w) && (p.y >= r.y && p.y <= r.y + r.h);
}

bool carbon_math_rect_overlaps(CBN_Rect r1, CBN_Rect r2) {
  return (r1.x <= r2.x + r2.w) && (r2.x <= r1.x + r1.w) && (r1.y <= r2.y + r2.h) && (r2.y <= r1.y + r1.h);
}

CBN_Rect carbon_math_rect_intersection(CBN_Rect r1, CBN_Rect r2) {
  f32 left   = CARBON_MAX(r1.x, r2.x);
  f32 top    = CARBON_MAX(r1.y, r2.y);
  f32 right  = CARBON_MIN(r1.x + r1.w, r2.x + r2.w);
  f32 bottom = CARBON_MIN(r1.y + r1.h, r2.y + r2.h);
  CBN_Rect r = carbon_math_rect_sq(left, top, 0);
  if (right > left && bottom > top) {
    r.w = right - left;
    r.h = bottom - top;
  }
  return r;
}

void carbon_math_rect_scale(CBN_Rect *r, const f32 s) {
  r->x = CARBON_LERP(r->x, r->x + r->w/2, 1 - s);
  r->y = CARBON_LERP(r->y, r->y + r->h/2, 1 - s);
  r->w = CARBON_LERP(0, r->w, s);
  r->h = CARBON_LERP(0, r->h, s);
}
