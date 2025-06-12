// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  CBN_DrawCanvas canvas = carbon_drawcanvas_create(640, 360);
  carbon_win_open(canvas.width, canvas.height, "Triangle");
  carbon_win_set_max_fps(60);
  f32 angle = 0;
  usz size = 250;
  CBN_Vec2 center = {{canvas.width/2, canvas.height/2}};
  CBN_Vec2 v1 = {{center.x, center.y - (size / CARBON_SQRT3)}};
  CBN_Vec2 v2 = {{center.x - (size/2), center.y + (size / (2 * CARBON_SQRT3))}};
  CBN_Vec2 v3 = {{center.x + (size/2), center.y + (size / (2 * CARBON_SQRT3))}};
  while (!carbon_win_shouldclose()) {
    f64 dt = carbon_win_get_deltatime();
    carbon_drawcanvas_fill(canvas, 0x181818ff);
    carbon_drawcanvas_triangle(canvas,
                               carbon_math_vec2_rotate_around_pivot(v1, angle, center),
                               carbon_math_vec2_rotate_around_pivot(v2, angle, center),
                               carbon_math_vec2_rotate_around_pivot(v3, angle, center),
                               0xffdd33ff);
    angle += 50 * dt;
    carbon_win_update(canvas);
  }
  carbon_win_close();
  carbon_drawcanvas_destroy(&canvas);
  return 0;
}
