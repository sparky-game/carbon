// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_DrawCanvas {
  static cbn::Scope<CBN_DrawCanvas> New(usz width, usz height) {
    return {
      carbon_drawcanvas_create(width, height),
      [](cbn::DrawCanvas *dc){ carbon_drawcanvas_destroy(dc); }
    };
  }

  void OpenWindow(const char *title) const { cbn::win::Open(this, title); }

  void UpdateWindow(void) const { cbn::win::Update(this); }

  cbn::Scope<CBN_Camera> CreateCamera(void) const { return cbn::Camera::New(*this); }

  u32 *Pixels(void) const { return carbon_drawcanvas_pixels(this); }

  usz Width(void) const { return carbon_drawcanvas_width(this); }

  usz Height(void) const { return carbon_drawcanvas_height(this); }

  CBN_Rect Extent(void) const { return carbon_drawcanvas_extent(this); }

  u32 Flags(void) const { return carbon_drawcanvas_flags(this); }

  void FlagsEnable(u32 fs) { carbon_drawcanvas_flags_enable(this, fs); }

  void FlagsDisable(u32 fs) { carbon_drawcanvas_flags_disable(this, fs); }

  void FlagsToggle(u32 fs) { carbon_drawcanvas_flags_toggle(this, fs); }

  isz LightAdd(CBN_Light l) { return carbon_drawcanvas_light_add(this, l); }

  void LightEnable(isz idx) { carbon_drawcanvas_light_enable(this, idx); }

  void LightDisable(isz idx) { carbon_drawcanvas_light_disable(this, idx); }

  void LightToggle(isz idx) { carbon_drawcanvas_light_toggle(this, idx); }

  void LightSetIntensity(isz idx, f32 intensity) { carbon_drawcanvas_light_set_intensity(this, idx, intensity); }

  void LightSetColor(isz idx, u32 color) { carbon_drawcanvas_light_set_color(this, idx, color); }

  void Fill(u32 color) { carbon_drawcanvas_fill(this, color); }

  void DrawLine(CBN_Vec2 v1, CBN_Vec2 v2, u32 color) { carbon_drawcanvas_line(this, v1, v2, color); }

  void DrawTriangle(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) { carbon_drawcanvas_triangle(this, v1, v2, v3, color); }

  void DrawTriangle3(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 c1, u32 c2, u32 c3) { carbon_drawcanvas_triangle_3(this, v1, v2, v3, c1, c2, c3); }

  void DrawRect(CBN_Rect r, u32 color) { carbon_drawcanvas_rect(this, r, color); }

  void DrawCircle(CBN_Vec2 center, usz radius, u32 color) { carbon_drawcanvas_circle(this, center, radius, color); }

  void DrawAnnulus(CBN_Vec2 center, usz radius_out, usz radius_in, u32 color) { carbon_drawcanvas_annulus(this, center, radius_out, radius_in, color); }

  void DrawSprite(const CBN_Sprite *s, CBN_Vec2 position, CBN_Vec2 scale = cbn::math::Vec2(1)) { carbon_drawcanvas_sprite(this, s, position, scale); }

  void DrawMesh(const CBN_Camera &c, const CBN_Mesh *m, CBN_Transform t, u32 color) { carbon_drawcanvas_mesh(this, &c, m, t, color); }

  void DrawCube(const CBN_Camera &c, CBN_Transform t, u32 color) { carbon_drawcanvas_cube(this, &c, t, color); }

  void DrawPlaneXZ(const CBN_Camera &c, CBN_Vec3 center, CBN_Vec2 size, u32 color) { carbon_drawcanvas_plane_xz(this, &c, center, size, color); }

  void DrawTextRaw(const char *txt, CBN_Vec2 position, usz size, u32 color) { carbon_drawcanvas_text(this, txt, position, size, color); }

  void DrawText(const char *txt, CBN_Vec2 position, usz size, u32 color) { carbon_drawcanvas_text_with_shadow(this, txt, position, size, color); }

  void DrawText(const CBN_Font *f, const char *txt, CBN_Vec2 position, usz size, u32 color) { carbon_drawcanvas_text_with_font(this, f, txt, position, size, color); }

  static usz TextWidth(const char *txt, usz size) { return carbon_drawcanvas_get_text_width(txt, size); }

  static usz TextWidth(const CBN_Font *f, const char *txt, usz size) { return carbon_drawcanvas_get_text_width_with_font(f, txt, size); }

  static usz TextHeight(usz size) { return carbon_drawcanvas_get_text_height(size); }

  static usz TextHeight(const CBN_Font *f, usz size) { return carbon_drawcanvas_get_text_height_with_font(f, size); }

  u32 &operator()(usz i, usz j) const { return Pixels()[j*Width() + i]; }
};

#endif
