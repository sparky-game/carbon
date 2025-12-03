// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

bool CBN_Rect::Contains(const CBN_Vec2 &p) const {
  return carbon_math_rect_contains(*this, p);
}

bool CBN_Rect::Overlaps(const CBN_Rect &r) const {
  return carbon_math_rect_overlaps(*this, r);
}

CBN_Rect CBN_Rect::Intersection(const CBN_Rect &r) const {
  return carbon_math_rect_intersection(*this, r);
}

void CBN_Rect::Scale(const f32 s) {
  carbon_math_rect_scale(this, s);
}

bool CBN_Rect::operator==(const CBN_Rect &r) const {
  return (x == r.x) && (y == r.y) && (w == r.w) && (h == r.h);
}
