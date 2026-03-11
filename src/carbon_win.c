// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

// NOTE: we ignore here these warnings because RGFW do strange things
// like shadowing variables, leaving labels unused, using deprecated functions, and such.
CARBON_COMPILER_DIAG_BEGIN;
CARBON_COMPILER_DIAG_IGNORE("-Wshadow");
CARBON_COMPILER_DIAG_IGNORE("-Wunused-label");
CARBON_COMPILER_DIAG_IGNORE("-Wdeprecated-declarations");
#define RGFWDEF      static inline
#define RGFW_ALLOC   carbon_memory_alloc
#define RGFW_FREE    carbon_memory_free
#define RGFW_ASSERT  CBN_ASSERT
#define RGFW_MEMCPY  carbon_memory_copy
#define RGFW_STRNCMP carbon_string_cmp_n
#define RGFW_STRTOL  strtol
#define RGFW_NO_TYPES
#define RGFW_BUFFER
#define RGFW_IMPLEMENTATION
#include "../thirdparty/RGFW/RGFW.h"
CARBON_COMPILER_DIAG_END;

static RGFW_window *carbon_win__handle;
static usz *carbon_win__xtable;
static usz *carbon_win__ytable;
static CBN_Image carbon_win__icon;
static bool carbon_win__cursor_visible = true;
static bool carbon_win__cursor_inside;

static u32 carbon_win__max_fps;
static u32 carbon_win__curr_fps;
static u32 carbon_win__fps;
static CBN_Chrono carbon_win__fps_timer;

static bool carbon_win__keys[CBN_Win_KeyCode_Count];
static bool carbon_win__prev_keys[CBN_Win_KeyCode_Count];
static bool carbon_win__mouse_buttons[CBN_Win_MouseButton_Count];
static bool carbon_win__prev_mouse_buttons[CBN_Win_MouseButton_Count];

CBNINL CBN_Win_KeyCode carbon_win__map_key_code(RGFW_key key) {
  switch (key) {
  case RGFW_a:        return CBN_Win_KeyCode_A;
  case RGFW_b:        return CBN_Win_KeyCode_B;
  case RGFW_c:        return CBN_Win_KeyCode_C;
  case RGFW_d:        return CBN_Win_KeyCode_D;
  case RGFW_e:        return CBN_Win_KeyCode_E;
  case RGFW_f:        return CBN_Win_KeyCode_F;
  case RGFW_g:        return CBN_Win_KeyCode_G;
  case RGFW_h:        return CBN_Win_KeyCode_H;
  case RGFW_i:        return CBN_Win_KeyCode_I;
  case RGFW_j:        return CBN_Win_KeyCode_J;
  case RGFW_k:        return CBN_Win_KeyCode_K;
  case RGFW_l:        return CBN_Win_KeyCode_L;
  case RGFW_m:        return CBN_Win_KeyCode_M;
  case RGFW_n:        return CBN_Win_KeyCode_N;
  case RGFW_o:        return CBN_Win_KeyCode_O;
  case RGFW_p:        return CBN_Win_KeyCode_P;
  case RGFW_q:        return CBN_Win_KeyCode_Q;
  case RGFW_r:        return CBN_Win_KeyCode_R;
  case RGFW_s:        return CBN_Win_KeyCode_S;
  case RGFW_t:        return CBN_Win_KeyCode_T;
  case RGFW_u:        return CBN_Win_KeyCode_U;
  case RGFW_v:        return CBN_Win_KeyCode_V;
  case RGFW_w:        return CBN_Win_KeyCode_W;
  case RGFW_x:        return CBN_Win_KeyCode_X;
  case RGFW_y:        return CBN_Win_KeyCode_Y;
  case RGFW_z:        return CBN_Win_KeyCode_Z;
  case RGFW_0:        return CBN_Win_KeyCode_Zero;
  case RGFW_1:        return CBN_Win_KeyCode_One;
  case RGFW_2:        return CBN_Win_KeyCode_Two;
  case RGFW_3:        return CBN_Win_KeyCode_Three;
  case RGFW_4:        return CBN_Win_KeyCode_Four;
  case RGFW_5:        return CBN_Win_KeyCode_Five;
  case RGFW_6:        return CBN_Win_KeyCode_Six;
  case RGFW_7:        return CBN_Win_KeyCode_Seven;
  case RGFW_8:        return CBN_Win_KeyCode_Eight;
  case RGFW_9:        return CBN_Win_KeyCode_Nine;
  case RGFW_backtick: return CBN_Win_KeyCode_BackQuote;
  case RGFW_F1:       return CBN_Win_KeyCode_F1;
  case RGFW_F2:       return CBN_Win_KeyCode_F2;
  case RGFW_F3:       return CBN_Win_KeyCode_F3;
  case RGFW_F4:       return CBN_Win_KeyCode_F4;
  case RGFW_F5:       return CBN_Win_KeyCode_F5;
  case RGFW_F6:       return CBN_Win_KeyCode_F6;
  case RGFW_F7:       return CBN_Win_KeyCode_F7;
  case RGFW_F8:       return CBN_Win_KeyCode_F8;
  case RGFW_F9:       return CBN_Win_KeyCode_F9;
  case RGFW_F10:      return CBN_Win_KeyCode_F10;
  case RGFW_F11:      return CBN_Win_KeyCode_F11;
  case RGFW_F12:      return CBN_Win_KeyCode_F12;
  case RGFW_escape:   return CBN_Win_KeyCode_Escape;
  case RGFW_tab:      return CBN_Win_KeyCode_Tab;
  case RGFW_capsLock: return CBN_Win_KeyCode_CapsLock;
  case RGFW_shiftL:   return CBN_Win_KeyCode_LeftShift;
  case RGFW_controlL: return CBN_Win_KeyCode_LeftCtrl;
  case RGFW_superL:   return CBN_Win_KeyCode_LeftMeta;
  case RGFW_altL:     return CBN_Win_KeyCode_LeftAlt;
  case RGFW_space:    return CBN_Win_KeyCode_Space;
  case RGFW_altR:     return CBN_Win_KeyCode_RightAlt;
  case RGFW_superR:   return CBN_Win_KeyCode_RightMeta;
  case RGFW_controlR: return CBN_Win_KeyCode_RightCtrl;
  case RGFW_shiftR:   return CBN_Win_KeyCode_RightShift;
  case RGFW_return:   return CBN_Win_KeyCode_Return;
  case RGFW_up:       return CBN_Win_KeyCode_UpArrow;
  case RGFW_down:     return CBN_Win_KeyCode_DownArrow;
  case RGFW_left:     return CBN_Win_KeyCode_LeftArrow;
  case RGFW_right:    return CBN_Win_KeyCode_RightArrow;
  default:
    CARBON_UNREACHABLE;
    return CBN_Win_KeyCode_Count;
  }
}

CBNINL CBN_Win_MouseButton carbon_win__map_mouse_button(RGFW_mouseButton btn) {
  switch (btn) {
  case RGFW_mouseLeft:       return CBN_Win_MouseButton_Left;
  case RGFW_mouseRight:      return CBN_Win_MouseButton_Right;
  case RGFW_mouseMiddle:     return CBN_Win_MouseButton_Middle;
  case RGFW_mouseScrollUp:   return CBN_Win_MouseButton_ScrollUp;
  case RGFW_mouseScrollDown: return CBN_Win_MouseButton_ScrollDown;
  default:
    CARBON_UNREACHABLE;
    return CBN_Win_MouseButton_Count;
  }
}

CBNINL void carbon_win__rebuild_xtable(usz dst_w, usz src_w) {
  carbon_win__xtable = (usz *) carbon_memory_realloc(carbon_win__xtable, dst_w * sizeof(usz));
  const f32 sf = (f32)src_w/dst_w;
  for (usz i = 0; i < dst_w; ++i) {
    carbon_win__xtable[i] = carbon_math_min(i * sf, src_w - 1);
  }
}

CBNINL void carbon_win__rebuild_ytable(usz dst_h, usz src_h) {
  carbon_win__ytable = (usz *) carbon_memory_realloc(carbon_win__ytable, dst_h * sizeof(usz));
  const f32 sf = (f32)src_h/dst_h;
  for (usz j = 0; j < dst_h; ++j) {
    carbon_win__ytable[j] = carbon_math_min(j * sf, src_h - 1);
  }
}

CBNINL void carbon_win__resize_callback(RGFW_window *win, RGFW_rect r) {
  // TODO: investigate the height offset thing (-28) in other systems
  CBN_DEBUG("r = (%d, %d, %d, %d - 28)", r.x, r.y, r.w, r.h);
  win->r.h -= 28;
}

CBNINL void carbon_win__key_callback(RGFW_window *win, u8 key, char keyChar, RGFW_keymod keyMod, RGFW_bool pressed) {
  CARBON_UNUSED(keyChar), CARBON_UNUSED(keyMod);
  if (win != carbon_win__handle) return;
  carbon_win__keys[carbon_win__map_key_code(key)] = pressed ? true : false;
}

CBNINL void carbon_win__mouse_button_callback(RGFW_window* win, RGFW_mouseButton button, f64 scroll, RGFW_bool pressed) {
  CARBON_UNUSED(scroll);
  if (win != carbon_win__handle) return;
  carbon_win__mouse_buttons[carbon_win__map_mouse_button(button)] = pressed ? true : false;
}

CBNINL void carbon_win__resize_buf(const CBN_DrawCanvas *dc) {
  RGFW_window *w = carbon_win__handle;
  if (w->bufferSize.w == (u32) w->r.w && w->bufferSize.h == (u32) w->r.h) return;
  w->bufferSize.w = w->r.w;
  w->bufferSize.h = w->r.h;
  CBN_DEBUG("buffer = %zux%zu", w->bufferSize.w, w->bufferSize.h);
  const usz sz = w->bufferSize.w * w->bufferSize.h * 4;
  w->buffer = (u8 *) carbon_memory_realloc(w->buffer, sz);
  carbon_memory_set(w->buffer, 0, sz);
  carbon_win__rebuild_xtable(w->bufferSize.w, carbon_drawcanvas_width(dc));
  carbon_win__rebuild_ytable(w->bufferSize.h, carbon_drawcanvas_height(dc));
}

CBNINL void carbon_win__upscale_buf(const CBN_DrawCanvas *dc) {
  // Nearest-neighbor interpolation algorithm
  const u32 * restrict src = carbon_drawcanvas_pixels(dc);
  const usz src_w = carbon_drawcanvas_width(dc);
  u32 * restrict dst = (u32 *) carbon_win__handle->buffer;
  const usz dst_w = carbon_win__handle->bufferSize.w, dst_h = carbon_win__handle->bufferSize.h;
  for (usz j = 0; j < dst_h; ++j) {
    const usz y = carbon_win__ytable[j];
    const u32 *r = src + y * src_w;
    for (usz i = 0; i < dst_w; ++i) {
      const usz x = carbon_win__xtable[i];
      *dst++ = carbon_math_bswap32(r[x]);
    }
  }
}

void carbon_win_open(const CBN_DrawCanvas *dc, const char *title) {
  const usz w = carbon_drawcanvas_width(dc), h = carbon_drawcanvas_height(dc);
  carbon_win__handle = RGFW_createWindow(title, RGFW_RECT(0, 0, w, h), RGFW_windowCenter | RGFW_windowCenterCursor);
  RGFW_window_raise(carbon_win__handle);
  RGFW_window_initBufferSize(carbon_win__handle, RGFW_AREA(w, h));
  // TODO: investigate the height offset thing (+28) in other systems
  RGFW_window_setMinSize(carbon_win__handle, RGFW_AREA(w, h + 28));
  i32 gcd = carbon_math_egcd(w, h);
  RGFW_window_setAspectRatio(carbon_win__handle, RGFW_AREA(w/gcd, h/gcd));
  carbon_win__rebuild_xtable(w, w);
  carbon_win__rebuild_ytable(h, h);
  CBN_INFO("Opened a %$x%$ window", $(w), $(h));
  RGFW_setWindowResizeCallback(carbon_win__resize_callback);
  RGFW_setKeyCallback(carbon_win__key_callback);
  RGFW_setMouseButtonCallback(carbon_win__mouse_button_callback);
}

void carbon_win_close(void) {
  carbon_memory_free(carbon_win__xtable);
  carbon_memory_free(carbon_win__ytable);
  carbon_image_destroy(&carbon_win__icon);
  RGFW_window_close(carbon_win__handle);
  CBN_INFO("Window closed successfully");
}

void carbon_win_set_max_fps(u32 fps) {
  carbon_win__max_fps = fps;
  CBN_INFO("Window max FPS set to %u", carbon_win__max_fps);
}

void carbon_win_set_icon(CBN_Image img) {
  if (!img.data) {
    CBN_ERROR("`img` is not valid");
    return;
  }
  carbon_win__icon = img;
  RGFW_bool status = RGFW_window_setIcon(carbon_win__handle,
                                         carbon_win__icon.data,
                                         RGFW_AREA(carbon_win__icon.metadata.width,
                                                   carbon_win__icon.metadata.height),
                                         carbon_win__icon.metadata.channels);
  if (!status) CBN_ERROR("Unable to set the window icon");
  else CBN_INFO("Set window icon correctly");
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

u32 carbon_win_get_fps(void) {
  if (!carbon_chrono_is_running(&carbon_win__fps_timer)) carbon_chrono_restart(&carbon_win__fps_timer);
  else carbon_chrono_update(&carbon_win__fps_timer);
  if (carbon_win__fps_timer.elapsed >= 0.5) {
    carbon_chrono_restart(&carbon_win__fps_timer);
    carbon_win__fps = carbon_win__curr_fps;
  }
  return carbon_win__fps;
}

void carbon_win_update(const CBN_DrawCanvas *dc) {
  carbon_win__resize_buf(dc);
  carbon_win__upscale_buf(dc);
  RGFW_window_swapBuffers(carbon_win__handle);
  carbon_win__curr_fps = RGFW_window_checkFPS(carbon_win__handle, carbon_win__max_fps);
}

bool carbon_win_shouldclose(void) {
  for (CBN_Win_KeyCode i = 0; i < CBN_Win_KeyCode_Count; ++i) carbon_win__prev_keys[i] = carbon_win__keys[i];
  for (CBN_Win_MouseButton i = 0; i < CBN_Win_MouseButton_Count; ++i) carbon_win__prev_mouse_buttons[i] = carbon_win__mouse_buttons[i];
  while (RGFW_window_checkEvent(carbon_win__handle)) {
    RGFW_eventType e = carbon_win__handle->event.type;
    if (e == RGFW_quit) return true;
    if (!carbon_win__cursor_visible) {
      if (e == RGFW_mouseEnter) {
        carbon_win__cursor_inside = true;
        RGFW_window_showMouse(carbon_win__handle, false);
      }
      else if (e == RGFW_mouseLeave) {
        carbon_win__cursor_inside = false;
        RGFW_window_showMouse(carbon_win__handle, true);
      }
    }
    else {
      if      (e == RGFW_mouseEnter) carbon_win__cursor_inside = true;
      else if (e == RGFW_mouseLeave) carbon_win__cursor_inside = false;
    }
  }
  return false;
}

void carbon_win_exit(void) {
  carbon_win__handle->event.type = RGFW_quit;
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

CBN_Vec2 carbon_win_get_mouse_position(void) {
  RGFW_point p = RGFW_window_getMousePoint(carbon_win__handle);
  usz w = carbon_win__handle->bufferSize.w, h = carbon_win__handle->bufferSize.h;
  usz x = carbon_math_clamp(p.x, 0, w-1), y = carbon_math_clamp(p.y, 0, h-1);
  return carbon_math_vec2(carbon_win__xtable[x], carbon_win__ytable[y]);
}

void carbon_win_set_mouse_visibility(bool visible) {
  carbon_win__cursor_visible = visible;
  if (carbon_win__cursor_inside) RGFW_window_showMouse(carbon_win__handle, visible);
}

void carbon_win_set_border_visibility(bool visible) {
  RGFW_window_setBorder(carbon_win__handle, visible);
}

void carbon_win_set_fullscreen(bool yn) {
  RGFW_window_setFullscreen(carbon_win__handle, yn);
}
