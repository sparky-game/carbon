// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

namespace res {
  static cbn::SKAP s_AssetPack;
  static cbn::mesh_mgr::UID s_Mesh_Teapot;

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
    LoadPack("3d.skap", s_AssetPack);
    cbn::mesh_mgr::Init();
    LoadMesh("./teapot.obj", s_Mesh_Teapot);
    CBN_INFO("Initialized resource manager successfully");
  }

  static void Shutdown(void) {
    cbn::mesh_mgr::Shutdown();
    CBN_INFO("Shutdowned resource manager successfully");
  }
}

static constexpr auto Color_BG = 0x181818ff;
static constexpr auto Color_1  = 0xffdd33ff;
static const     auto Color_2  = cbn::color::Complementary(Color_1);

void camera_update(cbn::Camera *c, const f64 dt) {
  {// Translation
    static constexpr auto speed = 4;
    if (cbn::win::GetKey(cbn::win::KeyCode::W)) c->MoveForward(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::S)) c->MoveBackward(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::A)) c->MoveLeft(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::D)) c->MoveRight(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::Space)) c->MoveUp(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::LeftControl)) c->MoveDown(speed * dt);
  }
  {// Rotation
    static constexpr auto speed = 70;
    c->Pitch((cbn::win::GetKey(cbn::win::KeyCode::I) - cbn::win::GetKey(cbn::win::KeyCode::K)) * speed * dt);
    c->Yaw((cbn::win::GetKey(cbn::win::KeyCode::J) - cbn::win::GetKey(cbn::win::KeyCode::L)) * speed * dt);
  }
}

void update(cbn::DrawCanvas &dc, cbn::Camera *c, const f64 dt) {
  camera_update(c, dt);
  if (cbn::win::GetKeyDown(cbn::win::KeyCode::B)) dc.FlagsToggle(CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING);
}

void mesh_render(cbn::DrawCanvas &dc, const cbn::Camera *c, const f64 dt) {
  static constexpr auto color = Color_1;
  static const auto * const mp = cbn::mesh_mgr::Lookup(res::s_Mesh_Teapot);
  static cbn::Transform transform = {
    .position = cbn::math::Vec3(0, -1, -3),
    .rotation = cbn::math::Vec3(),
    .scale    = cbn::math::Vec3(1)
  };
  dc.DrawMesh(c, mp, transform, color);
  transform.rotation.y += 50 * dt;
}

void hud_render(cbn::DrawCanvas &dc, const cbn::Camera *c) {
  static constexpr auto color = 0x737373ff;
  static constexpr auto text_size = 2;
  static constexpr auto text_padding = 10;
  static const auto text_height = dc.TextHeight(text_size);
  {// Top-left stats
    static const std::string render_res = cbn::str::fmt("Render res: %zux%zu", dc.width, dc.height);
    const char *text[] = {
      cbn::str::fmt(CARBON_NAME " %s", cbn::Version(0, 0, 0)),
      cbn::str::fmt("%u fps", cbn::win::GetFPS()),
      render_res.c_str(),
      cbn::str::fmt("Back-face culling [b]: %s", dc.flags & CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING ? "ON" : "OFF")
    };
    for (usz i = 0; i < CARBON_ARRAY_LEN(text); ++i) {
      dc.DrawText(text[i], cbn::math::Vec2(text_padding, text_padding + i*text_height), text_size, color);
    }
  }
  {// Bottom-left info
    const char *text = cbn::str::fmt("Camera: [Pos = %s, Rot = %s]", c->GetPosition().ToString(), c->GetRotation().ToString());
    static const auto text_pos = cbn::math::Vec2(text_padding, dc.height - text_padding - text_height);
    dc.DrawText(text, text_pos, text_size, color);
  }
  {// Crosshair
    static constexpr auto size = 11;
    const auto hw = dc.width/2, hh = dc.height/2;
    dc.DrawLine(cbn::math::Vec2(hw, hh - size), cbn::math::Vec2(hw, hh + size), color);
    dc.DrawLine(cbn::math::Vec2(hw - size, hh), cbn::math::Vec2(hw + size, hh), color);
  }
}

void render(cbn::DrawCanvas &dc, const cbn::Camera *c, const f64 dt) {
  dc.Fill(Color_BG);
  dc.DrawPlaneXZ(c, cbn::math::Vec3(0, -2, 0), cbn::math::Vec2(25), Color_2);
  mesh_render(dc, c, dt);
  hud_render(dc, c);
}

int main(void) {
  static constexpr auto RRF = 80;
  auto canvas = cbn::DrawCanvas::make(16*RRF, 9*RRF);
  auto cam = cbn::Camera::make(canvas);
  res::Init();
  cbn::win::Open(canvas, "3D");
  cbn::win::SetMaxFPS(0);  // Set to unlimited for debug purposes
  cbn::win::ForFrame([&](const auto dt){
    update(canvas, cam, dt);
    render(canvas, cam, dt);
    cbn::win::Update(canvas);
  });
  cbn::win::Close();
  res::Shutdown();
  cam->Free();
  canvas.Free();
}
