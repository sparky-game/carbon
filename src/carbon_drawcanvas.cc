// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifdef __cplusplus

#include "carbon.inc"

CBN_DrawCanvas CBN_DrawCanvas::make(usz width, usz height) {
  return carbon_drawcanvas_create(width, height);
}

void CBN_DrawCanvas::Free(void) {
  carbon_drawcanvas_destroy(this);
}

void CBN_DrawCanvas::Fill(u32 color) {
  carbon_drawcanvas_fill(*this, color);
}

void CBN_DrawCanvas::DrawTriangle(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) {
  carbon_drawcanvas_triangle(*this, v1, v2, v3, color);
}

void CBN_DrawCanvas::DrawRect(CBN_Rect r, u32 color) {
  carbon_drawcanvas_rect(*this, r, color);
}

void CBN_DrawCanvas::DrawCircle(CBN_Vec2 center, usz radius, u32 color) {
  carbon_drawcanvas_circle(*this, center, radius, color);
}

u32 CBN_DrawCanvas::HSVToRGB(f32 h, f32 s, f32 v) {
  return carbon_drawcanvas_hsv_to_rgb(h, s, v);
}

u32 &CBN_DrawCanvas::operator()(usz i, usz j) {
  return const_cast<u32 &>(static_cast<const CBN_DrawCanvas &>(*this)(i, j));
}

const u32 &CBN_DrawCanvas::operator()(usz i, usz j) const {
  return pixels[j * stride + i];
}

#endif  // __cplusplus
