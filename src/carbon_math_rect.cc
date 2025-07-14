// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

u8 CBN_Rect::Contains(const CBN_Vec2 &p) const {
  return carbon_math_rect_contains_point(*this, p);
}

u8 CBN_Rect::Overlaps(const CBN_Rect &r) const {
  return carbon_math_rect_detect_collision(*this, r);
}

void CBN_Rect::Scale(const f32 s) {
  carbon_math_rect_scale(this, s);
}

bool CBN_Rect::operator==(const CBN_Rect &r) const {
  return (x == r.x) && (y == r.y) && (w == r.w) && (h == r.h);
}
