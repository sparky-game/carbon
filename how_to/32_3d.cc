// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

namespace res {
  static cbn::Opt<cbn::SKAP> s_AssetPack;
  static cbn::mesh_mgr::UID s_Mesh_Teapot;
  static cbn::mesh_mgr::UID s_Mesh_Tree;
  static cbn::sprite_mgr::UID s_Sprite_Tree;

  static inline void LoadAsset(auto load, const char *name, auto &uid) {
    if (auto i = ((*s_AssetPack).*load)(name)) uid = *i;
    else CARBON_UNREACHABLE;
  }

  static inline void LoadMesh(const char *name, auto &uid) {
    LoadAsset(&cbn::SKAP::LoadMesh, name, uid);
  }

  static inline void LoadSprite(const char *name, auto &uid) {
    LoadAsset(&cbn::SKAP::LoadSprite, name, uid);
  }

  static void Init(void) {
    CBN_ASSERT(cbn::fs::cd(cbn::fs::GetBinDir()));
    if (auto i = cbn::SKAP::Open("assets.skap")) s_AssetPack = cbn::meta::Move(i);
    else CARBON_UNREACHABLE;
    cbn::mesh_mgr::Init();
    LoadMesh("./3d_models/teapot.obj", s_Mesh_Teapot);
    LoadMesh("./3d_models/tree.obj", s_Mesh_Tree);
    cbn::sprite_mgr::Init();
    LoadSprite("./3d_models/tree.png", s_Sprite_Tree);
    CBN_INFO("Initialized resource manager successfully");
  }

  static void Shutdown(void) {
    cbn::sprite_mgr::Shutdown();
    cbn::mesh_mgr::Shutdown();
    s_AssetPack->Free();
    CBN_INFO("Shutdowned resource manager successfully");
  }
}

static constexpr auto Color_BG = 0x181818ff;
static constexpr auto Color_FG  = 0xffdd33ff;

void camera_update(cbn::Camera &c, const f64 dt) {
  {// Translation
    static constexpr auto speed = 4;
    if (cbn::win::GetKey(cbn::win::KeyCode::W))        c.MoveForward(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::S))        c.MoveBackward(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::A))        c.MoveLeft(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::D))        c.MoveRight(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::Space))    c.MoveUp(speed * dt);
    if (cbn::win::GetKey(cbn::win::KeyCode::LeftCtrl)) c.MoveDown(speed * dt);
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

void update(cbn::DrawCanvas &dc, cbn::Camera &c, const f64 dt) {
  camera_update(c, dt);
  if (cbn::win::GetKeyDown(cbn::win::KeyCode::B)) dc.FlagsToggle(CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING);
}

void meshes_render(cbn::DrawCanvas &dc, const cbn::Camera &c, const f64 dt) {
  {// Plane
    // dc.DrawPlaneXZ(c, cbn::math::Vec3(-3, -2, -3), cbn::math::Vec2(/*6*/ 25), 0xff0000ff);
    static constexpr cbn::Transform t {
      .position = {-3, -2, -3},
      .rotation = {0},
      .scale    = {25}
    };
    dc.DrawMesh(c, &dc.Plane, t, 0x888888ff);
  }
  {// Cube #1
    static constexpr cbn::Transform t {
      .position = {-2, -1.5, -2},
      .rotation = {0},
      .scale    = {1}
    };
    dc.DrawMesh(c, &dc.Cube, t, Color_FG);
  }
  {// Cube #2
    static constexpr cbn::Transform t {
      .position = {-3, -1.5, -3},
      .rotation = {0},
      .scale    = {1}
    };
    dc.DrawMesh(c, &dc.Cube, t, Color_FG);
  }
  {// Teapot
    static const auto * const m = cbn::mesh_mgr::Lookup(res::s_Mesh_Teapot);
    static cbn::Transform t {
      .position = {-5, -1.5, -5},
      .rotation = {0},
      .scale    = {1}
    };
    dc.DrawMesh(c, m, t, Color_FG);
    t.rotation.y += 50 * dt;
  }
  {// Tree
    static const auto * const m = cbn::mesh_mgr::Lookup(res::s_Mesh_Tree);
    static const auto * const s = cbn::sprite_mgr::Lookup(res::s_Sprite_Tree);
    static cbn::Transform t {
      .position = {5, -1.5, -5},
      .rotation = {0},
      .scale    = {3}
    };
    dc.DrawMesh(c, m, t, s);
    t.rotation.y += 50 * dt;
  }
}

void hud_render(cbn::DrawCanvas &dc, const cbn::Camera &c) {
  static constexpr auto color = 0x737373ff;
  static constexpr auto text_size = 2;
  static constexpr auto text_padding = 10;
  static const auto text_height = dc.TextHeight(text_size);
  {// Top-left stats
    static const std::string render_res = cbn::str::fmt("Render res: %zux%zu", dc.Width(), dc.Height());
    const char *text[] = {
      cbn::str::fmt(CARBON_LIBNAME " %s", cbn::VersionStr()),
      cbn::str::fmt("%u fps", cbn::win::GetFPS()),
      render_res.c_str(),
      cbn::str::fmt("Back-face culling [b]: %s", dc.Flags() & CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING ? "ON" : "OFF")
    };
    for (usz i = 0; i < CARBON_ARRAY_LEN(text); ++i) {
      dc.DrawText(text[i], cbn::math::Vec2(text_padding, text_padding + i*text_height), text_size, color);
    }
  }
  {// Bottom-left info
    const auto text = cbn::str::fmt("Camera: [Pos = %s, Rot = %s]", c.GetPosition().ToString(), c.GetRotation().ToString());
    static const auto text_pos = cbn::math::Vec2(text_padding, dc.Height() - text_padding - text_height);
    dc.DrawText(text, text_pos, text_size, color);
  }
  {// Crosshair
    static constexpr auto size = 11;
    const auto hw = dc.Width()/2, hh = dc.Height()/2;
    dc.DrawLine(cbn::math::Vec2(hw, hh - size), cbn::math::Vec2(hw, hh + size), color);
    dc.DrawLine(cbn::math::Vec2(hw - size, hh), cbn::math::Vec2(hw + size, hh), color);
  }
}

void render(cbn::DrawCanvas &dc, const cbn::Camera &c, const f64 dt) {
  dc.Fill(Color_BG);
  meshes_render(dc, c, dt);
  hud_render(dc, c);
}

int main(void) {
  auto canvas = cbn::DrawCanvas::New(1280, 720);
  auto cam = canvas->CreateCamera();
  // cam->SetType(cbn::Camera::Type::Orthographic);
  canvas->LightAdd({
      .type = cbn::LightType::Directional,
      .color = 0xffffffff,
      .intensity = 1,
      .as_dir = {
        .direction = cbn::math::Vec3(1, 2, 1).Normalize()
      }
    });
  res::Init();
  canvas->OpenWindow("3D");
  cbn::win::ForFrame([&](const auto dt){
    update(*canvas, *cam, dt);
    render(*canvas, *cam, dt);
    canvas->UpdateWindow();
  });
  cbn::win::Close();
  res::Shutdown();
}
