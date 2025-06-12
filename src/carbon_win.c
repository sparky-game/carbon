// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define RGFW_ALLOC   CARBON_MALLOC
#define RGFW_FREE    CARBON_FREE
#define RGFW_ASSERT  CARBON_ASSERT
#define RGFW_MEMCPY  carbon_memory_copy
#define RGFW_STRNCMP carbon_string_cmp_n
#define RGFW_STRTOL  strtol
#define RGFW_NO_TYPES
// TODO: Support other rendering methods (e.g. Vulkan, Metal, DirectX, OpenGL).
#define RGFW_BUFFER  // i.e. CPU software rendering
#define RGFW_IMPLEMENTATION
#include "../vendor/RGFW/RGFW.h"

static RGFW_window *carbon_win__handle;
static u32 carbon_win__max_fps;
static CBN_Image carbon_win__icon;

void carbon_win_open(u16 width, u16 height, const char *title) {
  RGFW_area screen_size = RGFW_getScreenSize();
  // TODO: let user resize the window.
  carbon_win__handle = RGFW_createWindow(title, RGFW_RECT((screen_size.w - width) / 2,
                                                          (screen_size.h - height) / 2,
                                                          width, height), RGFW_windowNoResize);
  // TODO: use `RGFW_window_initBuffer` instead, so the buffer isn't tied up to the initial window size.
  RGFW_window_initBufferSize(carbon_win__handle, RGFW_AREA(carbon_win__handle->r.w, carbon_win__handle->r.h));
  carbon_log_info("Opened window at (%$, %$) of size %$x%$",
                  $(carbon_win__handle->r.x), $(carbon_win__handle->r.y),
                  $(carbon_win__handle->r.w), $(carbon_win__handle->r.h));
}

void carbon_win_close(void) {
  if (carbon_win__icon.data) carbon_fs_destroy_img(&carbon_win__icon);
  RGFW_window_close(carbon_win__handle);
  carbon_log_info("Window closed successfully");
}

void carbon_win_set_max_fps(u32 fps) {
  carbon_win__max_fps = fps;
  carbon_log_info("Window max FPS set to %$", $(carbon_win__max_fps));
}

void carbon_win_set_icon(CBN_Image img) {
  carbon_win__icon = img;
  RGFW_bool status = RGFW_window_setIcon(carbon_win__handle,
                                         carbon_win__icon.data,
                                         RGFW_AREA(carbon_win__icon.metadata.width,
                                                   carbon_win__icon.metadata.height),
                                         carbon_win__icon.metadata.channels);
  if (!status) carbon_log_error("Unable to set the window icon");
  else carbon_log_info("Set window icon correctly");
}

f64 carbon_win_get_deltatime(void) {
  static f64 last_frame_time;
  static u8 is_first_time = true;
  if (is_first_time) {
    last_frame_time = carbon_time_get();
    is_first_time = false;
  }
  f64 curr_frame_time = carbon_time_get();
  f64 dt = curr_frame_time - last_frame_time;
  last_frame_time = curr_frame_time;
  return dt;
}

void carbon_win_update(CBN_DrawCanvas dc) {
  for (usz i = 0; i < dc.width * dc.height; ++i) {
    u32 color = dc.pixels[i];
    u8 r = (color >> 24) & 0xff;
    u8 g = (color >> 16) & 0xff;
    u8 b = (color >> 8)  & 0xff;
    u8 a = (color >> 0)  & 0xff;
    carbon_win__handle->buffer[i * 4 + 0] = r;
    carbon_win__handle->buffer[i * 4 + 1] = g;
    carbon_win__handle->buffer[i * 4 + 2] = b;
    carbon_win__handle->buffer[i * 4 + 3] = a;
  }
  RGFW_window_swapBuffers(carbon_win__handle);
  RGFW_window_checkFPS(carbon_win__handle, carbon_win__max_fps);
}

u8 carbon_win_shouldclose(void) {
  RGFW_window_checkEvent(carbon_win__handle);
  return RGFW_window_shouldClose(carbon_win__handle);
}
