#ifdef __cplusplus

struct CBN_DrawCanvas {
  static cbn::Scope<CBN_DrawCanvas> New(usz width, usz height) {
    return {
      carbon_drawcanvas_create(width, height),
      [](cbn::DrawCanvas *dc){ if (dc) dc->Free(); }
    };
  }

  void Free(void) {
    carbon_drawcanvas_destroy(this);
  }

  void OpenWindow(const char *title) const {
    cbn::win::Open(this, title);
  }

  void UpdateWindow(void) const {
    cbn::win::Update(this);
  }

  cbn::Scope<CBN_Camera> CreateCamera(void) const {
    return cbn::Camera::New(this);
  }

  u32 *Pixels(void) const {
    return carbon_drawcanvas_pixels(this);
  }

  usz Width(void) const {
    return carbon_drawcanvas_width(this);
  }

  usz Height(void) const {
    return carbon_drawcanvas_height(this);
  }

  u32 Flags(void) const {
    return carbon_drawcanvas_flags(this);
  }

  void FlagsEnable(u32 fs) {
    carbon_drawcanvas_flags_enable(this, fs);
  }

  void FlagsDisable(u32 fs) {
    carbon_drawcanvas_flags_disable(this, fs);
  }

  void FlagsToggle(u32 fs) {
    carbon_drawcanvas_flags_toggle(this, fs);
  }

  void Fill(u32 color) {
    carbon_drawcanvas_fill(this, color);
  }

  void DrawLine(CBN_Vec2 v1, CBN_Vec2 v2, u32 color) {
    carbon_drawcanvas_line(this, v1, v2, color);
  }

  void DrawTriangle(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) {
    carbon_drawcanvas_triangle(this, v1, v2, v3, color);
  }

  void DrawRect(CBN_Rect r, u32 color) {
    carbon_drawcanvas_rect(this, r, color);
  }

  void DrawCircle(CBN_Vec2 center, usz radius, u32 color) {
    carbon_drawcanvas_circle(this, center, radius, color);
  }

  void DrawSprite(const CBN_Sprite *s, CBN_Vec2 position, CBN_Vec2 scale = cbn::math::Vec2(1)) {
    carbon_drawcanvas_sprite(this, s, position, scale);
  }

  void DrawMesh(const CBN_Camera &c, const CBN_Mesh *m, CBN_Transform t, u32 color) {
    carbon_drawcanvas_mesh(this, &c, m, t, color);
  }

  void DrawPlaneXZ(const CBN_Camera &c, CBN_Vec3 center, CBN_Vec2 size, u32 color) {
    carbon_drawcanvas_plane_xz(this, &c, center, size, color);
  }

  void DrawTextRaw(const char *txt, CBN_Vec2 position, usz size, u32 color) {
    carbon_drawcanvas_text(this, txt, position, size, color);
  }

  void DrawText(const char *txt, CBN_Vec2 position, usz size, u32 color) {
    carbon_drawcanvas_text_with_shadow(this, txt, position, size, color);
  }

  void DrawText(const CBN_Font *f, const char *txt, CBN_Vec2 position, usz size, u32 color) {
    carbon_drawcanvas_text_with_font(this, f, txt, position, size, color);
  }

  static usz TextWidth(const char *txt, usz size) {
    return carbon_drawcanvas_get_text_width(txt, size);
  }

  static usz TextWidth(const CBN_Font *f, const char *txt, usz size) {
    return carbon_drawcanvas_get_text_width_with_font(f, txt, size);
  }

  static usz TextHeight(usz size) {
    return carbon_drawcanvas_get_text_height(size);
  }

  static usz TextHeight(const CBN_Font *f, usz size) {
    return carbon_drawcanvas_get_text_height_with_font(f, size);
  }

  u32 &operator()(usz i, usz j) const {
    return Pixels()[j*Width() + i];
  }
};

#endif
