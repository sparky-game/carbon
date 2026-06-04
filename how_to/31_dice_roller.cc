// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

static constexpr auto c_Background = 0x181818ff;

template <u32 Color, cbn::meta::String Label, f32 Phi, f32 ZRot, f32 Delta, f32 Scale, f32 YOff, auto Mesh>
struct Die {
  static void Render(cbn::DrawCanvas &dc, const cbn::Camera &c, const f64 dt) {
    static f32 phi = Phi;
    static f32 phi_speed = 0;
    static cbn::math::Vec2 phi_dir = {1};
    static cbn::Transform t = {
      .position = {0},
      .rotation = {phi, phi, ZRot},
      .scale    = {Scale}
    };
    Roll(phi, phi_speed, phi_dir, dt);
    t.rotation.xy = phi_dir * phi;
    t.rotation.y += YOff;
    if (cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
      auto r = cbn::rng::RollDice(Label.value);
      CBN_DEBUG("%s := %u", Label.value, r);
    }
    dc.DrawMesh(c, Mesh, t, Color);
  }

private:
  static void Roll(f32 &phi, f32 &phi_speed, cbn::math::Vec2 &phi_dir, const f64 dt) {
    static constexpr auto eps = 2;
    static constexpr auto phi_accel = -600;
    static constexpr auto phi_speed_max = 900;
    static constexpr auto phi_speed_min = 200;
    if (!phi_speed && cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
      phi_speed = phi_speed_max;
      phi_dir = {cbn::rng::LCGf() < 0.5 ? 1.f : -1.f};
    }
    if (!phi_speed) return;
    phi_speed = cbn::math::Max(phi_speed_min, phi_speed + phi_accel * dt);
    phi += phi_speed * dt;
    if (phi_speed > phi_speed_min) return;
    auto k = cbn::math::Round((phi - Delta)/90.f);
    auto tgt = k*90.f + Delta;
    if (cbn::math::Abs(phi - tgt) < eps) {
      phi = tgt;
      phi_speed = 0;
    }
  }
};

using D4  = Die<0x1ba242ff,  "1d4", 45.f, 90.f, 45.f,   1.f,  0.f, &cbn::DrawCanvas::Tetrahedron>;
using D6  = Die<0x1fc0d3ff,  "1d6",  0.f,  0.f,  0.f,   1.f,  0.f, &cbn::DrawCanvas::Cube>;
using D8  = Die<0x973ae3ff,  "1d8", 45.f, 90.f, 45.f, 1.75f,  0.f, &cbn::DrawCanvas::Octahedron>;
using D10 = Die<0xe32b96ff, "1d10",  0.f, 90.f,  0.f,  1.5f, 60.f, &cbn::DrawCanvas::PentaTrapezohedron>;
using D12 = Die<0xdb3c41ff, "1d12",  0.f, 18.f,  0.f,   1.f, 32.f, &cbn::DrawCanvas::Dodecahedron>;
using D20 = Die<0xf38000ff, "1d20",  0.f, 30.f,  0.f,   1.f, 21.f, &cbn::DrawCanvas::Icosahedron>;

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
  static constexpr Button<"D4", text_size, text_color> btn_d4 {{10, 10, 60, 40}, btn_color};
  static constexpr Button<"D6", text_size, text_color> btn_d6 {btn_d4.VNext(padding), btn_color};
  static constexpr Button<"D8", text_size, text_color> btn_d8 {btn_d6.VNext(padding), btn_color};
  static constexpr Button<"D10", text_size, text_color> btn_d10 {btn_d8.VNext(padding), btn_color};
  static constexpr Button<"D12", text_size, text_color> btn_d12 {btn_d10.VNext(padding), btn_color};
  static constexpr Button<"D20", text_size, text_color> btn_d20 {btn_d12.VNext(padding), btn_color};
  static auto dice_render = D6::Render;
  btn_d4.Update([]{ dice_render = D4::Render; });
  btn_d4.Render(dc);
  btn_d6.Update([]{ dice_render = D6::Render; });
  btn_d6.Render(dc);
  btn_d8.Update([]{ dice_render = D8::Render; });
  btn_d8.Render(dc);
  btn_d10.Update([]{ dice_render = D10::Render; });
  btn_d10.Render(dc);
  btn_d12.Update([]{ dice_render = D12::Render; });
  btn_d12.Render(dc);
  btn_d20.Update([]{ dice_render = D20::Render; });
  btn_d20.Render(dc);
  dice_render(dc, c, dt);
}

void camera_update(cbn::Camera &c, const f64 dt) {
  {// Translation
    static constexpr auto speed = 4;
    if (cbn::win::GetKey(cbn::win::KeyCode::W))         c.MoveForward(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::S))         c.MoveBackward(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::A))         c.MoveLeft(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::D))         c.MoveRight(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::LeftShift)) c.MoveUp(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::LeftCtrl))  c.MoveDown(speed * dt);
  }
  {// Rotation
    static constexpr auto speed = 70;
    c.Pitch((cbn::win::GetKey(cbn::win::KeyCode::I) - cbn::win::GetKey(cbn::win::KeyCode::K)) * speed * dt);
    c.Yaw((cbn::win::GetKey(cbn::win::KeyCode::J)   - cbn::win::GetKey(cbn::win::KeyCode::L)) * speed * dt);
  }
  {// Orthographic size
    static constexpr auto speed = 3;
    if (cbn::win::GetKey(cbn::win::KeyCode::UpArrow))   c.OrthoZoom(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::DownArrow)) c.OrthoZoom(-speed * dt);
  }
}

int main(void) {
  auto canvas = cbn::DrawCanvas::New(960, 540);
  auto cam = canvas->CreateCamera();
  cam->SetPosition({0, 0, 3});
  canvas->LightAdd({
      .type = cbn::LightType::Directional,
      .color = 0xffffffff,
      .intensity = 1,
      .as_dir = {
        .direction = cbn::math::Vec3(1, 2, 10).Normalize()
      }
    });
  canvas->OpenWindow("Dice Roller");
  cbn::win::ForFrame([&](const auto dt){
    camera_update(*cam, dt);
    canvas->Fill(c_Background);
    iter(*canvas, *cam, dt);
    canvas->UpdateWindow();
  });
  cbn::win::Close();
}
