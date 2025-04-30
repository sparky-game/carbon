#define CARBON_USE_WINDOWING
#define CARBON_IMPLEMENTATION
#include "../carbon.h"

int main(void) {
  usz width = 640, height = 360;
  CBN_DrawCanvas canvas = carbon_drawcanvas_create(width, height);
  carbon_win_open(width, height, "Triangle");
  f32 angle = 0;
  usz size = 250;
  CBN_Vec2 center = {{width/2, height/2}};
  CBN_Vec2 v1 = {{center.x, center.y - (size / CARBON_SQRT3)}};
  CBN_Vec2 v2 = {{center.x - (size / 2), center.y + (size / (2 * CARBON_SQRT3))}};
  CBN_Vec2 v3 = {{center.x + (size / 2), center.y + (size / (2 * CARBON_SQRT3))}};
  while (!carbon_win_shouldclose()) {
    carbon_drawcanvas_fill(canvas, 0xff181818);
    carbon_drawcanvas_triangle(canvas,
                               carbon_math_vec2_rotate_around_pivot(v1, angle, center),
                               carbon_math_vec2_rotate_around_pivot(v2, angle, center),
                               carbon_math_vec2_rotate_around_pivot(v3, angle, center),
                               0xffffdd33);
    angle += 1e-1;
    carbon_win_update(canvas);
  }
  carbon_win_close();
  carbon_drawcanvas_destroy(&canvas);
  return 0;
}
