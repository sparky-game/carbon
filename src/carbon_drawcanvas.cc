// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

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

void CBN_DrawCanvas::DrawLine(CBN_Vec2 v1, CBN_Vec2 v2, u32 color) {
  carbon_drawcanvas_line(*this, v1, v2, color);
}

void CBN_DrawCanvas::DrawTriangle(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) {
  carbon_drawcanvas_triangle(*this, v1, v2, v3, color);
}

void CBN_DrawCanvas::DrawTriangle3D(CBN_Vec3 v1, CBN_Vec3 v2, CBN_Vec3 v3, u32 color) {
  carbon_drawcanvas_triangle_3d(*this, v1, v2, v3, color);
}

void CBN_DrawCanvas::DrawRect(CBN_Rect r, u32 color) {
  carbon_drawcanvas_rect(*this, r, color);
}

void CBN_DrawCanvas::DrawCircle(CBN_Vec2 center, usz radius, u32 color) {
  carbon_drawcanvas_circle(*this, center, radius, color);
}

void CBN_DrawCanvas::DrawSprite(const CBN_Sprite *s, CBN_Vec2 position) {
  carbon_drawcanvas_sprite(*this, s, position);
}

void CBN_DrawCanvas::DrawMesh(const CBN_Camera *c, const CBN_Mesh *m, CBN_Transform t, u32 color) {
  carbon_drawcanvas_mesh(*this, c, m, t, color);
}

void CBN_DrawCanvas::DrawPlaneXZ(const CBN_Camera *c, CBN_Vec3 center, CBN_Vec2 size, u32 color) {
  carbon_drawcanvas_plane_xz(*this, c, center, size, color);
}

void CBN_DrawCanvas::DrawBox(CBN_Rect r) {
  carbon_drawcanvas_box(*this, r);
}

void CBN_DrawCanvas::DrawText(const char *txt, CBN_Vec2 position, usz size, u32 color) {
  carbon_drawcanvas_text_with_shadow(*this, txt, position, size, color);
}

void CBN_DrawCanvas::DrawTextRaw(const char *txt, CBN_Vec2 position, usz size, u32 color) {
  carbon_drawcanvas_text(*this, txt, position, size, color);
}

usz CBN_DrawCanvas::TextWidth(const char *txt, usz size) {
  return carbon_drawcanvas_get_text_width(txt, size);
}

usz CBN_DrawCanvas::TextHeight(usz size) {
  return carbon_drawcanvas_get_text_height(size);
}

u32 &CBN_DrawCanvas::operator()(usz i, usz j) const {
  return carbon_drawcanvas_at(*this, i, j);
}

CBN_DrawCanvas::operator bool(void) const {
  return pixels ? true : false;
}
