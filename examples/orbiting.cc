// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  auto canvas = cbn::DrawCanvas::make(600, 600);
  cbn::win::Open(canvas, "Orbiting");
  cbn::win::SetMaxFPS(60);
  constexpr auto radius = 50;
  auto rotation = 0.0f;
  auto position = cbn::math::Vec2(canvas.width - radius - 20, canvas.height/2);
  cbn::win::ForFrame([&](const auto dt){
    canvas.Fill(0x181818ff);
    rotation += 45 * dt;
    position += cbn::math::Vec2(0, 1).Rotate(rotation) * 180 * dt;
    canvas.DrawCircle(position, radius, 0xffdd33ff);
    cbn::win::Update(canvas);
  });
  cbn::win::Close();
  canvas.Free();
}
