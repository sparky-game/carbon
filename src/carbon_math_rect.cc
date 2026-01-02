// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

using namespace cbn::math;

bool Rect::Contains(const Vec2 &p) const {
  return carbon_math_rect_contains(*this, p);
}

bool Rect::Overlaps(const Rect &r) const {
  return carbon_math_rect_overlaps(*this, r);
}

Rect Rect::Intersection(const Rect &r) const {
  return carbon_math_rect_intersection(*this, r);
}

void Rect::Scale(const f32 s) {
  carbon_math_rect_scale(this, s);
}

bool Rect::operator==(const Rect &r) const {
  return (x == r.x) && (y == r.y) && (w == r.w) && (h == r.h);
}
