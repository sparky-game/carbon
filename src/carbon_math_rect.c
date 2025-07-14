// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

u8 carbon_math_rect_contains_point(CBN_Rect r, CBN_Vec2 p) {
  return (p.x >= r.x && p.x <= r.x + r.w) && (p.y >= r.y && p.y <= r.y + r.h);
}

u8 carbon_math_rect_detect_collision(CBN_Rect r1, CBN_Rect r2) {
  return (r1.x <= r2.x + r2.w) && (r2.x <= r1.x + r1.w) && (r1.y <= r2.y + r2.h) && (r2.y <= r1.y + r1.h);
}

void carbon_math_rect_scale(CBN_Rect *r, const f32 s) {
  r->x = CARBON_LERP(r->x, r->x + r->w/2, 1 - s);
  r->y = CARBON_LERP(r->y, r->y + r->h/2, 1 - s);
  r->w = CARBON_LERP(0, r->w, s);
  r->h = CARBON_LERP(0, r->h, s);
}
