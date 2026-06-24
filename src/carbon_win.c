// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

static bool carbon_win__should_close;
static usz carbon_win__renderer_w, carbon_win__renderer_h;

static bool carbon_win__cursor_visible = true;
static CBN_Vec2 carbon_win__mouse_delta;
static f32 carbon_win__mouse_scroll;

static u32 carbon_win__max_fps;
static u32 carbon_win__curr_fps;
static u32 carbon_win__fps;
static CBN_Chrono carbon_win__fps_timer;

static bool carbon_win__keys[CBN_Win_KeyCode_Count];
static bool carbon_win__prev_keys[CBN_Win_KeyCode_Count];
static bool carbon_win__mouse_buttons[CBN_Win_MouseButton_Count];
static bool carbon_win__prev_mouse_buttons[CBN_Win_MouseButton_Count];

// Internal platform API
CBNINL void carbon_win__create_window(usz w, usz h, const char *title);
CBNINL void carbon_win__destroy_window(void);
CBNINL CBN_Vec2 carbon_win__get_window_size(void);
CBNINL void carbon_win__renderer_present(const u32 *pixels, usz w, usz h);
CBNINL bool carbon_win__poll_event(void);

#if defined(__APPLE__)
#include "carbon_win_macos.m"
#elif defined(_WIN32)
#include "carbon_win_win32.inl"
#elif defined(__linux__) || defined(__FreeBSD__)
#include "carbon_win_x11.inl"
#else
#error Target platform is not supported
#endif

void carbon_win_open(const CBN_DrawCanvas *dc, const char *title) {
  const usz w = carbon_drawcanvas_width(dc), h = carbon_drawcanvas_height(dc);
  carbon_win__create_window(w, h, title);
  CBN_INFO("Opened a %zux%zu window", w, h);
}

void carbon_win_close(void) {
  carbon_win__destroy_window();
  CBN_INFO("Window closed successfully");
}

void carbon_win_set_max_fps(u32 fps) {
  carbon_win__max_fps = fps;
  CBN_INFO("Window max FPS set to %u", carbon_win__max_fps);
}

usz carbon_win_width(void) {
  return (usz)carbon_win__get_window_size().x;
}

usz carbon_win_height(void) {
  return (usz)carbon_win__get_window_size().y;
}

f64 carbon_win_get_deltatime(void) {
  static f64 last_frame_time;
  static bool is_first_time = true;
  if (is_first_time) {
    last_frame_time = carbon_time_get();
    is_first_time = false;
  }
  f64 curr_frame_time = carbon_time_get();
  f64 dt = curr_frame_time - last_frame_time;
  last_frame_time = curr_frame_time;
  return dt;
}

u32 carbon_win_get_fps(void) {
  if (!carbon_chrono_is_running(&carbon_win__fps_timer)) carbon_chrono_restart(&carbon_win__fps_timer);
  else carbon_chrono_update(&carbon_win__fps_timer);
  if (carbon_win__fps_timer.elapsed >= 0.5) {
    carbon_chrono_restart(&carbon_win__fps_timer);
    carbon_win__fps = carbon_win__curr_fps;
  }
  return carbon_win__fps;
}

CBNINL void carbon_win__update_fps(void) {
  static f64 start;
  f64 t = start ? carbon_time_get() - start : 0;
  if (carbon_win__max_fps) {
    f64 tgt = 1./carbon_win__max_fps;
    if (t < tgt) carbon_time_sleep((tgt - t) * 1e3);
  }
  carbon_win__curr_fps = t > 0 ? 1/t : 0;
  start = carbon_time_get();
}

void carbon_win_update(const CBN_DrawCanvas *dc) {
  carbon_win__renderer_present(carbon_drawcanvas_pixels(dc), carbon_drawcanvas_width(dc), carbon_drawcanvas_height(dc));
  carbon_win__update_fps();
}

CBNINL void carbon_win__update_mouse_delta(void) {
  static CBN_Vec2 last_p;
  CBN_Vec2 p = carbon_win_get_mouse_position();
  carbon_win__mouse_delta = carbon_math_vec2_sub(p, last_p);
  last_p = p;
}

bool carbon_win_shouldclose(void) {
  for (CBN_Win_KeyCode i = 0; i < CBN_Win_KeyCode_Count; ++i) {
    carbon_win__prev_keys[i] = carbon_win__keys[i];
  }
  for (CBN_Win_MouseButton i = 0; i < CBN_Win_MouseButton_Count; ++i) {
    carbon_win__prev_mouse_buttons[i] = carbon_win__mouse_buttons[i];
  }
  carbon_win__mouse_scroll = 0;
  while (carbon_win__poll_event());
  carbon_win__update_mouse_delta();
  return carbon_win__should_close;
}

void carbon_win_exit(void) {
  carbon_win__should_close = true;
}

bool carbon_win_get_key_down(CBN_Win_KeyCode key) {
  return carbon_win__keys[key] && !carbon_win__prev_keys[key];
}

bool carbon_win_get_key(CBN_Win_KeyCode key) {
  return carbon_win__keys[key];
}

bool carbon_win_get_key_up(CBN_Win_KeyCode key) {
  return !carbon_win__keys[key] && carbon_win__prev_keys[key];
}

bool carbon_win_get_mouse_button_down(CBN_Win_MouseButton btn) {
  return carbon_win__mouse_buttons[btn] && !carbon_win__prev_mouse_buttons[btn];
}

bool carbon_win_get_mouse_button(CBN_Win_MouseButton btn) {
  return carbon_win__mouse_buttons[btn];
}

bool carbon_win_get_mouse_button_up(CBN_Win_MouseButton btn) {
  return !carbon_win__mouse_buttons[btn] && carbon_win__prev_mouse_buttons[btn];
}

bool carbon_win_get_any_down(void) {
  for (CBN_Win_KeyCode i = 0; i < CBN_Win_KeyCode_Count; ++i) {
    if (carbon_win_get_key_down(i)) return true;
  }
  for (CBN_Win_MouseButton i = 0; i < CBN_Win_MouseButton_Count; ++i) {
    if (carbon_win_get_mouse_button_down(i)) return true;
  }
  return false;
}

CBN_Vec2 carbon_win_get_mouse_delta(void) {
  return carbon_win__mouse_delta;
}

f32 carbon_win_get_mouse_scroll(void) {
  return carbon_win__mouse_scroll;
}
