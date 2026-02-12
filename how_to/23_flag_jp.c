// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  const usz width = 900, height = 600;
  CBN_Vec2 flag_size = carbon_math_vec2(width, height);
  CBN_DrawCanvas *canvas = carbon_drawcanvas_create(flag_size.x * 1.0555, flag_size.y * 1.08);
  carbon_drawcanvas_fill(canvas, 0x181818ff);
  carbon_drawcanvas_rect(canvas, carbon_math_rect(width/2  - flag_size.x/2,
                                                  height/2 - flag_size.y/2,
                                                  flag_size.x, flag_size.y), 0xffffffff);
  carbon_drawcanvas_circle(canvas, carbon_math_vec2(width/2, height/2), 180, 0xbc002dff);
  carbon_win_open(canvas, "Flag of Japan");
  carbon_win_set_max_fps(60);
  while (!carbon_win_shouldclose()) carbon_win_update(canvas);
  carbon_win_close();
  const char *filepath = "flag_jp.png";
  u8 *canvas_bytes = carbon_fs_img_32bit_to_8bit(carbon_drawcanvas_pixels(canvas), width, height);
  CBN_ASSERT(carbon_fs_change_directory(carbon_fs_get_bin_directory()));
  if (carbon_fs_write_img_to_file_linearly(canvas_bytes, CBN_FILE_FORMAT_PNG, width, height, 4, filepath)) {
    CBN_INFO("written canvas to `%$` successfully", $(filepath));
  }
  carbon_memory_free(canvas_bytes);
  carbon_drawcanvas_destroy(canvas);
  return 0;
}
