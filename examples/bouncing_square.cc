// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  auto canvas = cbn::DrawCanvas::make(800, 600);
  cbn::win::Open(canvas, "Bouncing Square");
  cbn::win::SetMaxFPS(60);
  constexpr u32 colors[] = {
    0xff1818ff, 0x18ff18ff,
    0x1818ffff, 0xffff18ff,
    0xff18ffff, 0x18ffffff
  };
  constexpr auto n_colors = CARBON_ARRAY_LEN(colors);
  constexpr auto size = 100;
  auto position = cbn::math::Vec2{
    (f32)cbn::rng::LCGr(0, canvas.width - size),
    (f32)cbn::rng::LCGr(0, canvas.height - size)
  };
  auto velocity = cbn::math::Vec2(200);
  auto c = 0;
  cbn::win::ForFrame([&](const auto dt){
    canvas.Fill(0x181818ff);
    position += velocity * dt;
    if (position.x < 0 || position.x + size >= canvas.width) {
      velocity.x *= -1;
      c = (c + 1) % n_colors;
    }
    if (position.y < 0 || position.y + size >= canvas.height) {
      velocity.y *= -1;
      c = (c + 1) % n_colors;
    }
    position.Clamp(cbn::math::Vec2(), cbn::math::Vec2(canvas.width, canvas.height) - size);
    canvas.DrawRect(CARBON_RECT_SQUARE_V(position, size), colors[c]);
    cbn::win::Update(canvas);
  });
  cbn::win::Close();
  canvas.Free();
}
