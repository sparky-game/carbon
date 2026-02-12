// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  auto canvas = cbn::DrawCanvas::New(800, 600);
  canvas->OpenWindow("Bouncing Square");
  cbn::win::SetMaxFPS(60);
  constexpr u32 colors[] = {
    0xff1818ff, 0x18ff18ff,
    0x1818ffff, 0xffff18ff,
    0xff18ffff, 0x18ffffff
  };
  constexpr auto n_colors = CARBON_ARRAY_LEN(colors);
  constexpr auto size = 100;
  auto position = cbn::math::Vec2{
    (f32)cbn::rng::LCGr(0, canvas->Width() - size),
    (f32)cbn::rng::LCGr(0, canvas->Height() - size)
  };
  auto velocity = cbn::math::Vec2(200);
  auto c = 0;
  cbn::win::ForFrame([&](const auto dt){
    canvas->Fill(0x181818ff);
    position += velocity * dt;
    if (position.x < 0 || position.x + size >= canvas->Width()) {
      velocity.x *= -1;
      c = (c + 1) % n_colors;
    }
    if (position.y < 0 || position.y + size >= canvas->Height()) {
      velocity.y *= -1;
      c = (c + 1) % n_colors;
    }
    position.Clamp(cbn::math::Vec2(), cbn::math::Vec2(canvas->Width(), canvas->Height()) - size);
    canvas->DrawRect(cbn::math::Rect(position, size), colors[c]);
    canvas->UpdateWindow();
  });
  cbn::win::Close();
}
