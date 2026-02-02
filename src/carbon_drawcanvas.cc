// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

using namespace cbn::math;

cbn::DrawCanvas cbn::DrawCanvas::make(usz w, usz h) {
  return carbon_drawcanvas_create(w, h);
}

void cbn::DrawCanvas::Free(void) {
  carbon_drawcanvas_destroy(this);
}

void cbn::DrawCanvas::OpenWindow(const char *title) const {
  cbn::win::Open(this, title);
}

void cbn::DrawCanvas::UpdateWindow(void) const {
  cbn::win::Update(this);
}

cbn::Scope<cbn::Camera> cbn::DrawCanvas::CreateCamera(void) const {
  return cbn::Camera::make_unique(this);
}

void cbn::DrawCanvas::FlagsEnable(u32 fs) {
  carbon_drawcanvas_flags_enable(this, fs);
}

void cbn::DrawCanvas::FlagsDisable(u32 fs) {
  carbon_drawcanvas_flags_disable(this, fs);
}

void cbn::DrawCanvas::FlagsToggle(u32 fs) {
  carbon_drawcanvas_flags_toggle(this, fs);
}

void cbn::DrawCanvas::Fill(u32 color) {
  carbon_drawcanvas_fill(this, color);
}

void cbn::DrawCanvas::DrawLine(Vec2 v1, Vec2 v2, u32 color) {
  carbon_drawcanvas_line(this, v1, v2, color);
}

void cbn::DrawCanvas::DrawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, u32 color) {
  carbon_drawcanvas_triangle(this, v1, v2, v3, color);
}

void cbn::DrawCanvas::DrawRect(Rect r, u32 color) {
  carbon_drawcanvas_rect(this, r, color);
}

void cbn::DrawCanvas::DrawCircle(Vec2 center, usz radius, u32 color) {
  carbon_drawcanvas_circle(this, center, radius, color);
}

void cbn::DrawCanvas::DrawSprite(const cbn::Sprite *s, Vec2 position, Vec2 scale) {
  carbon_drawcanvas_sprite(this, s, position, scale);
}

void cbn::DrawCanvas::DrawMesh(const cbn::Camera &c, const cbn::Mesh *m, cbn::Transform t, u32 color) {
  carbon_drawcanvas_mesh(this, &c, m, t, color);
}

void cbn::DrawCanvas::DrawPlaneXZ(const cbn::Camera &c, Vec3 center, Vec2 size, u32 color) {
  carbon_drawcanvas_plane_xz(this, &c, center, size, color);
}

void cbn::DrawCanvas::DrawBox(Rect r) {
  carbon_drawcanvas_box(this, r);
}

void cbn::DrawCanvas::DrawText(const char *txt, Vec2 position, usz size, u32 color) {
  carbon_drawcanvas_text_with_shadow(this, txt, position, size, color);
}

void cbn::DrawCanvas::DrawTextRaw(const char *txt, Vec2 position, usz size, u32 color) {
  carbon_drawcanvas_text(this, txt, position, size, color);
}

usz cbn::DrawCanvas::TextWidth(const char *txt, usz size) {
  return carbon_drawcanvas_get_text_width(txt, size);
}

usz cbn::DrawCanvas::TextHeight(usz size) {
  return carbon_drawcanvas_get_text_height(size);
}

u32 &cbn::DrawCanvas::operator()(usz i, usz j) const {
  return carbon_drawcanvas_at(this, i, j);
}
