// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_IMPLEMENTATION
#define CARBON_IMPLEMENTATION_WINDOWING
#include "../carbon.h"

int main(void) {
  CBN_DrawCanvas canvas = carbon_drawcanvas_create(800, 600);
  carbon_win_open(canvas.width, canvas.height, "Bouncing Square");
  carbon_win_set_max_fps(60);
  constexpr u32 colors[] = {
    0xff1818ff,
    0x18ff18ff,
    0x1818ffff,
    0xffff18ff,
    0xff18ffff,
    0x18ffffff
  };
  constexpr auto n_colors = CARBON_ARRAY_LEN(colors);
  constexpr auto size = 100;
  auto position = CARBON_VEC2(carbon_math_rand_between(0, canvas.width - size),
                              carbon_math_rand_between(0, canvas.height - size));
  auto velocity = CARBON_VEC2_1(200);
  auto c = 0;
  while (!carbon_win_shouldclose()) {
    const auto dt = carbon_win_get_deltatime();
    carbon_drawcanvas_fill(canvas, 0x181818ff);
    position += velocity * dt;
    if (position.x < 0 || position.x + size >= canvas.width) {
      velocity.x *= -1;
      c = (c + 1) % n_colors;
    }
    if (position.y < 0 || position.y + size >= canvas.height) {
      velocity.y *= -1;
      c = (c + 1) % n_colors;
    }
    position.Clamp(CARBON_VEC2_ZERO, CARBON_VEC2(canvas.width, canvas.height) - size);
    carbon_drawcanvas_rect(canvas, CARBON_RECT_SQUARE_V(position, size), colors[c]);
    carbon_win_update(canvas);
  }
  carbon_win_close();
  carbon_drawcanvas_destroy(&canvas);
  return 0;
}
