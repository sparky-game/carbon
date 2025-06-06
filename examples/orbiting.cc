// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_IMPLEMENTATION
#define CARBON_IMPLEMENTATION_WINDOWING
#include "../carbon.h"

int main(void) {
  CBN_DrawCanvas canvas = carbon_drawcanvas_create(600, 600);
  cbn::win::Open(canvas.width, canvas.height, "Orbiting");
  cbn::win::SetMaxFPS(60);
  constexpr auto radius = 50;
  auto rotation = 0.0f;
  auto position = CARBON_VEC2(canvas.width - radius - 20, canvas.height/2);
  cbn::win::ForFrame([&](const auto dt){
    carbon_drawcanvas_fill(canvas, 0x181818ff);
    rotation += 45 * dt;
    position += CARBON_VEC2_UP.Rotate(rotation) * 180 * dt;
    carbon_drawcanvas_circle(canvas, position, radius, 0xffdd33ff);
    cbn::win::Update(canvas);
  });
  cbn::win::Close();
  carbon_drawcanvas_destroy(&canvas);
  return 0;
}
