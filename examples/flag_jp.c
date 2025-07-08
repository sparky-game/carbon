// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  CBN_Vec2 flag_size = {{900, 600}};
  CBN_DrawCanvas canvas = carbon_drawcanvas_create(flag_size.x * 1.0555, flag_size.y * 1.08);
  carbon_drawcanvas_fill(canvas, 0x181818ff);
  carbon_drawcanvas_rect(canvas, CARBON_RECT(canvas.width/2  - flag_size.x/2,
                                             canvas.height/2 - flag_size.y/2,
                                             flag_size.x, flag_size.y), 0xffffffff);
  carbon_drawcanvas_circle(canvas, CARBON_VEC2(canvas.width/2, canvas.height/2), 180, 0xbc002dff);
  carbon_win_open(canvas.width, canvas.height, "Flag of Japan");
  carbon_win_set_max_fps(60);
  while (!carbon_win_shouldclose()) carbon_win_update(canvas);
  carbon_win_close();
  const char *filepath = "flag_jp.png";
  u8 *canvas_bytes = carbon_fs_img_32bit_to_8bit(canvas.pixels, canvas.width, canvas.height);
  carbon_fs_change_directory(carbon_fs_get_bin_directory());
  if (carbon_fs_write_img_to_file_linearly(canvas_bytes, CBN_FILE_FORMAT_PNG, canvas.width, canvas.height, 4, filepath)) {
    CBN_INFO("written canvas to `%$` successfully", $(filepath));
  }
  CBN_FREE(canvas_bytes);
  carbon_drawcanvas_destroy(&canvas);
  return 0;
}
