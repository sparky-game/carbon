// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

static constexpr auto c_Background = 0x181818ff;

namespace res {
  static cbn::SKAP s_AssetPack;
  static constexpr auto c_Color_d4 = 0x1ba242ff;
  static cbn::mesh_mgr::UID s_Mesh_d4;
  static constexpr auto c_Color_d6 = 0x1fc0d3ff;
  static cbn::mesh_mgr::UID s_Mesh_d6;
  static constexpr auto c_Color_d8 = 0x973ae3ff;
  static cbn::mesh_mgr::UID s_Mesh_d8;
  static constexpr auto c_Color_d10 = 0xe32b96ff;
  static constexpr auto c_Color_d12 = 0xdb3c41ff;
  static constexpr auto c_Color_d20 = 0xf38000ff;

  static inline void LoadPack(const char *path, auto &var) {
    if (auto i = cbn::SKAP::make(path)) var = *i;
    else CARBON_UNREACHABLE;
  }

  static inline void LoadAsset(auto load, const char *path, auto &uid) {
    if (auto i = (s_AssetPack.*load)(path)) uid = *i;
    else CARBON_UNREACHABLE;
  }

  static inline void LoadMesh(const char *path, auto &uid) {
    LoadAsset(&cbn::SKAP::LoadMesh, path, uid);
  }

  static void Init(void) {
    CBN_ASSERT(cbn::fs::cd(cbn::fs::GetBinDir()));
    LoadPack("assets.skap", s_AssetPack);
    cbn::mesh_mgr::Init();
    LoadMesh("./3d_models/tetrahedron.obj", s_Mesh_d4);
    LoadMesh("./3d_models/cube.obj", s_Mesh_d6);
    LoadMesh("./3d_models/octahedron.obj", s_Mesh_d8);
    CBN_INFO("Initialized resource manager successfully");
  }

  static void Shutdown(void) {
    cbn::mesh_mgr::Shutdown();
    CBN_INFO("Shutdowned resource manager successfully");
  }
}

struct Die {
  // ...
};

void roll_die(f32 &phi, f32 &phi_speed, cbn::math::Vec2 &phi_dir, const f32 theta, const f32 delta, const f64 dt) {
  static constexpr auto epsilon = 2;
  static constexpr auto phi_accel = -600;
  static constexpr auto phi_speed_max = 900;
  static constexpr auto phi_speed_min = 200;
  if (!phi_speed && cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
    phi_speed = phi_speed_max;
    auto r = cbn::rng::LCGf() < 0.5 ? 1 : -1;
    phi_dir = cbn::math::Vec2(r);
  }
  if (phi_speed) {
    phi_speed = cbn::math::Max(phi_speed_min, phi_speed + phi_accel * dt);
    phi += phi_speed * dt;
    if (phi_speed <= phi_speed_min) {
      auto k = cbn::math::Round((phi - delta)/theta);
      auto tgt = k * theta + delta;
      if (cbn::math::Abs(phi - tgt) < epsilon) {
        phi = tgt;
        phi_speed = 0;
      }
    }
  }
}

void d4_render(cbn::DrawCanvas &dc, const cbn::Camera &c, const f64 dt) {
  static f32 phi = 45;
  static f32 phi_speed = 0;
  static auto phi_dir = cbn::math::Vec2(1);
  static cbn::Transform t = {
    .position = cbn::math::Vec3(0, 0, -3),
    .rotation = cbn::math::Vec3(phi, phi, 90),
    .scale    = cbn::math::Vec3(1)
  };
  roll_die(phi, phi_speed, phi_dir, 90, 45, dt);
  t.rotation.xy = phi_dir * phi;
  if (cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
    auto r = cbn::rng::RollDice("1d4");
    CBN_DEBUG("1d4 := %u", r);
  }
  {// Draw 3D mesh
    static const auto * const mp = cbn::mesh_mgr::Lookup(res::s_Mesh_d4);
    dc.DrawMesh(c, mp, t, res::c_Color_d4);
  }
}

void d6_render(cbn::DrawCanvas &dc, const cbn::Camera &c, const f64 dt) {
  static f32 phi = 0;
  static f32 phi_speed = 0;
  static auto phi_dir = cbn::math::Vec2(1);
  static cbn::Transform t = {
    .position = cbn::math::Vec3(0, 0, -3),
    .rotation = cbn::math::Vec3(phi, phi, 0),
    .scale    = cbn::math::Vec3(1)
  };
  roll_die(phi, phi_speed, phi_dir, 90, 0, dt);
  t.rotation.xy = phi_dir * phi;
  if (cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
    auto r = cbn::rng::RollDice("1d6");
    CBN_DEBUG("1d6 := %u", r);
  }
  {// Draw 3D mesh
    static const auto * const mp = cbn::mesh_mgr::Lookup(res::s_Mesh_d6);
    dc.DrawMesh(c, mp, t, res::c_Color_d6);
  }
}

void d8_render(cbn::DrawCanvas &dc, const cbn::Camera &c, const f64 dt) {
  static f32 phi = 45;
  static f32 phi_speed = 0;
  static auto phi_dir = cbn::math::Vec2(1);
  static cbn::Transform t = {
    .position = cbn::math::Vec3(0, 0, -3),
    .rotation = cbn::math::Vec3(phi, phi, 90),
    .scale    = cbn::math::Vec3(1.75)
  };
  roll_die(phi, phi_speed, phi_dir, 90, 45, dt);
  t.rotation.xy = phi_dir * phi;
  if (cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
    auto r = cbn::rng::RollDice("1d8");
    CBN_DEBUG("1d8 := %u", r);
  }
  {// Draw 3D mesh
    static const auto * const mp = cbn::mesh_mgr::Lookup(res::s_Mesh_d8);
    dc.DrawMesh(c, mp, t, res::c_Color_d8);
  }
}

template <cbn::meta::String T, usz TSize, u32 TColor>
struct Button {
  consteval Button(const cbn::math::Rect &xywh, u32 color)
    : m_Rect{xywh}, m_RectColor{color}
  {}

  consteval cbn::math::Rect VNext(usz padding) const {
    auto r = m_Rect;
    r.y += r.h + padding;
    return r;
  }

  consteval cbn::math::Rect HNext(usz padding) const {
    auto r = m_Rect;
    r.x += r.w + padding;
    return r;
  }

  template <typename F>
  void Update(F &&callback) const {
    if (!cbn::win::GetMouseButtonDown(cbn::win::MouseButton::Left)) return;
    const auto p = cbn::win::GetMousePosition();
    if (m_Rect.Contains(p)) {
      callback();
    }
  }

  void Render(cbn::DrawCanvas &dc) const {
    static const auto tw = dc.TextWidth(c_Text, c_TextSize);
    static const auto th = dc.TextHeight(c_TextSize);
    static const cbn::math::Vec2 tp {
      m_Rect.x + (m_Rect.w - tw)/2,
      m_Rect.y + (m_Rect.h - th)/2
    };
    dc.DrawRect(m_Rect, m_RectColor);
    dc.DrawText(c_Text, tp, c_TextSize, c_TextColor);
  }
  
private:
  static constexpr auto c_Text {T.value};
  static constexpr auto c_TextSize {TSize};
  static constexpr auto c_TextColor {TColor};
  cbn::math::Rect m_Rect;
  u32 m_RectColor;
};

void iter(cbn::DrawCanvas &dc, const cbn::Camera &c, const f64 dt) {
  static constexpr auto btn_color = 0xdcdcdcff;
  static constexpr auto text_color = 0x181818ff;
  static constexpr auto text_size = 3;
  static constexpr auto padding = 10;
  static constexpr Button<"D4", text_size, text_color> btn_d4 {cbn::math::Rect(10, 10, 60, 40), btn_color};
  static constexpr Button<"D6", text_size, text_color> btn_d6 {btn_d4.VNext(padding), btn_color};
  static constexpr Button<"D8", text_size, text_color> btn_d8 {btn_d6.VNext(padding), btn_color};
  static auto dice_render = d6_render;
  btn_d4.Update([](){
    dice_render = d4_render;
  });
  btn_d4.Render(dc);
  btn_d6.Update([](){
    dice_render = d6_render;
  });
  btn_d6.Render(dc);
  btn_d8.Update([](){
    dice_render = d8_render;
  });
  btn_d8.Render(dc);
  dice_render(dc, c, dt);
}

int main(void) {
  auto canvas = cbn::DrawCanvas::New(960, 540);
  auto cam = canvas->CreateCamera();
  res::Init();
  canvas->OpenWindow("Dice Roller");
  cbn::win::ForFrame([&](const auto dt){
    canvas->Fill(c_Background);
    iter(*canvas, *cam, dt);
    canvas->UpdateWindow();
  });
  cbn::win::Close();
  res::Shutdown();
}
