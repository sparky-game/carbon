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

static u32 carbon_win__max_fps;
static u32 carbon_win__curr_fps;
static u32 carbon_win__fps;
static CBN_Chrono carbon_win__fps_timer;

static bool carbon_win__keys[CARBON_KEY_CODE_Count];
static bool carbon_win__prev_keys[CARBON_KEY_CODE_Count];
static bool carbon_win__mouse_buttons[CARBON_MOUSE_BUTTON_Count];
static bool carbon_win__prev_mouse_buttons[CARBON_MOUSE_BUTTON_Count];

static pthread_t carbon_win__thread_id;
static pthread_mutex_t carbon_win__thread_mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t carbon_win__thread_cond = PTHREAD_COND_INITIALIZER;
static _Atomic bool carbon_win__thread_running;
static _Atomic bool carbon_win__thread_ready;

CBNINL CBN_KeyCode carbon_win__map_key_code(RGFW_key key) {
  switch (key) {
  case RGFW_a:        return CARBON_KEY_CODE_A;
  case RGFW_b:        return CARBON_KEY_CODE_B;
  case RGFW_c:        return CARBON_KEY_CODE_C;
  case RGFW_d:        return CARBON_KEY_CODE_D;
  case RGFW_e:        return CARBON_KEY_CODE_E;
  case RGFW_f:        return CARBON_KEY_CODE_F;
  case RGFW_g:        return CARBON_KEY_CODE_G;
  case RGFW_h:        return CARBON_KEY_CODE_H;
  case RGFW_i:        return CARBON_KEY_CODE_I;
  case RGFW_j:        return CARBON_KEY_CODE_J;
  case RGFW_k:        return CARBON_KEY_CODE_K;
  case RGFW_l:        return CARBON_KEY_CODE_L;
  case RGFW_m:        return CARBON_KEY_CODE_M;
  case RGFW_n:        return CARBON_KEY_CODE_N;
  case RGFW_o:        return CARBON_KEY_CODE_O;
  case RGFW_p:        return CARBON_KEY_CODE_P;
  case RGFW_q:        return CARBON_KEY_CODE_Q;
  case RGFW_r:        return CARBON_KEY_CODE_R;
  case RGFW_s:        return CARBON_KEY_CODE_S;
  case RGFW_t:        return CARBON_KEY_CODE_T;
  case RGFW_u:        return CARBON_KEY_CODE_U;
  case RGFW_v:        return CARBON_KEY_CODE_V;
  case RGFW_w:        return CARBON_KEY_CODE_W;
  case RGFW_x:        return CARBON_KEY_CODE_X;
  case RGFW_y:        return CARBON_KEY_CODE_Y;
  case RGFW_z:        return CARBON_KEY_CODE_Z;
  case RGFW_0:        return CARBON_KEY_CODE_Zero;
  case RGFW_1:        return CARBON_KEY_CODE_One;
  case RGFW_2:        return CARBON_KEY_CODE_Two;
  case RGFW_3:        return CARBON_KEY_CODE_Three;
  case RGFW_4:        return CARBON_KEY_CODE_Four;
  case RGFW_5:        return CARBON_KEY_CODE_Five;
  case RGFW_6:        return CARBON_KEY_CODE_Six;
  case RGFW_7:        return CARBON_KEY_CODE_Seven;
  case RGFW_8:        return CARBON_KEY_CODE_Eight;
  case RGFW_9:        return CARBON_KEY_CODE_Nine;
  case RGFW_backtick: return CARBON_KEY_CODE_BackQuote;
  case RGFW_F1:       return CARBON_KEY_CODE_F1;
  case RGFW_F2:       return CARBON_KEY_CODE_F2;
  case RGFW_F3:       return CARBON_KEY_CODE_F3;
  case RGFW_F4:       return CARBON_KEY_CODE_F4;
  case RGFW_F5:       return CARBON_KEY_CODE_F5;
  case RGFW_F6:       return CARBON_KEY_CODE_F6;
  case RGFW_F7:       return CARBON_KEY_CODE_F7;
  case RGFW_F8:       return CARBON_KEY_CODE_F8;
  case RGFW_F9:       return CARBON_KEY_CODE_F9;
  case RGFW_F10:      return CARBON_KEY_CODE_F10;
  case RGFW_F11:      return CARBON_KEY_CODE_F11;
  case RGFW_F12:      return CARBON_KEY_CODE_F12;
  case RGFW_escape:   return CARBON_KEY_CODE_Escape;
  case RGFW_tab:      return CARBON_KEY_CODE_Tab;
  case RGFW_capsLock: return CARBON_KEY_CODE_CapsLock;
  case RGFW_shiftL:   return CARBON_KEY_CODE_LeftShift;
  case RGFW_controlL: return CARBON_KEY_CODE_LeftControl;
  case RGFW_superL:   return CARBON_KEY_CODE_LeftMeta;
  case RGFW_altL:     return CARBON_KEY_CODE_LeftAlt;
  case RGFW_space:    return CARBON_KEY_CODE_Space;
  case RGFW_altR:     return CARBON_KEY_CODE_RightAlt;
  case RGFW_superR:   return CARBON_KEY_CODE_RightMeta;
  case RGFW_controlR: return CARBON_KEY_CODE_RightControl;
  case RGFW_shiftR:   return CARBON_KEY_CODE_RightShift;
  case RGFW_return:   return CARBON_KEY_CODE_Return;
  case RGFW_up:       return CARBON_KEY_CODE_UpArrow;
  case RGFW_down:     return CARBON_KEY_CODE_DownArrow;
  case RGFW_left:     return CARBON_KEY_CODE_LeftArrow;
  case RGFW_right:    return CARBON_KEY_CODE_RightArrow;
  default:
    CARBON_UNREACHABLE;
    return CARBON_KEY_CODE_Count;
  }
}

CBNINL CBN_MouseButton carbon_win__map_mouse_button(RGFW_mouseButton btn) {
  switch (btn) {
  case RGFW_mouseLeft:       return CARBON_MOUSE_BUTTON_Left;
  case RGFW_mouseRight:      return CARBON_MOUSE_BUTTON_Right;
  case RGFW_mouseMiddle:     return CARBON_MOUSE_BUTTON_Middle;
  case RGFW_mouseScrollUp:   return CARBON_MOUSE_BUTTON_ScrollUp;
  case RGFW_mouseScrollDown: return CARBON_MOUSE_BUTTON_ScrollDown;
  default:
    CARBON_UNREACHABLE;
    return CARBON_MOUSE_BUTTON_Count;
  }
}

CBNINL void carbon_win__rebuild_xtable(usz dst_w, usz src_w) {
  carbon_win__xtable = (usz *) carbon_memory_realloc(carbon_win__xtable, dst_w * sizeof(usz));
  const f32 sf = (f32) src_w / dst_w;
  for (usz i = 0; i < dst_w; ++i) {
    carbon_win__xtable[i] = CARBON_MIN(i * sf, src_w - 1);
  }
}

CBNINL void carbon_win__rebuild_ytable(usz dst_h, usz src_h) {
  carbon_win__ytable = (usz *) carbon_memory_realloc(carbon_win__ytable, dst_h * sizeof(usz));
  const f32 sf = (f32) src_h / dst_h;
  for (usz j = 0; j < dst_h; ++j) {
    carbon_win__ytable[j] = CARBON_MIN(j * sf, src_h - 1);
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

CBNINL void *carbon_win__thread_fn(void *arg) {
  const CBN_DrawCanvas *dc = (const CBN_DrawCanvas *)arg;
  while (atomic_load(&carbon_win__thread_running)) {
    pthread_mutex_lock(&carbon_win__thread_mut);
    while (!atomic_load(&carbon_win__thread_ready) && atomic_load(&carbon_win__thread_running)) {
      pthread_cond_wait(&carbon_win__thread_cond, &carbon_win__thread_mut);
    }
    pthread_mutex_unlock(&carbon_win__thread_mut);
    if (atomic_exchange(&carbon_win__thread_ready, false)) {
      carbon_win__resize_buf(dc);
      carbon_win__upscale_buf(dc);
    }
  }
  return 0;
}

void carbon_win_open(const CBN_DrawCanvas *dc, const char *title) {
  const usz w = carbon_drawcanvas_width(dc), h = carbon_drawcanvas_height(dc);
  carbon_win__handle = RGFW_createWindow(title, RGFW_RECT(0, 0, w, h), RGFW_windowCenter);
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
  atomic_store(&carbon_win__thread_running, true);
  CBN_ASSERT(0 == pthread_create(&carbon_win__thread_id, 0, carbon_win__thread_fn, (void *)dc));
}

void carbon_win_close(void) {
  pthread_mutex_lock(&carbon_win__thread_mut);
  atomic_store(&carbon_win__thread_running, false);
  pthread_cond_signal(&carbon_win__thread_cond);
  pthread_mutex_unlock(&carbon_win__thread_mut);
  pthread_join(carbon_win__thread_id, 0);
  carbon_memory_free(carbon_win__xtable);
  carbon_memory_free(carbon_win__ytable);
  if (carbon_win__icon.data) carbon_fs_destroy_img(&carbon_win__icon);
  RGFW_window_close(carbon_win__handle);
  CBN_INFO("Window closed successfully");
}

void carbon_win_set_max_fps(u32 fps) {
  carbon_win__max_fps = fps;
  CBN_INFO("Window max FPS set to %$", $(carbon_win__max_fps));
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
  CARBON_UNUSED(dc);
  pthread_mutex_lock(&carbon_win__thread_mut);
  atomic_store(&carbon_win__thread_ready, true);
  pthread_cond_signal(&carbon_win__thread_cond);
  pthread_mutex_unlock(&carbon_win__thread_mut);
  RGFW_window_swapBuffers(carbon_win__handle);
  carbon_win__curr_fps = RGFW_window_checkFPS(carbon_win__handle, carbon_win__max_fps);
}

bool carbon_win_shouldclose(void) {
  for (CBN_KeyCode i = 0; i < CARBON_KEY_CODE_Count; ++i) carbon_win__prev_keys[i] = carbon_win__keys[i];
  for (CBN_MouseButton i = 0; i < CARBON_MOUSE_BUTTON_Count; ++i) carbon_win__prev_mouse_buttons[i] = carbon_win__mouse_buttons[i];
  while (RGFW_window_checkEvent(carbon_win__handle)) {
    RGFW_eventType e = carbon_win__handle->event.type;
    if (e == RGFW_quit) return true;
    if (!carbon_win__cursor_visible) {
      if (e == RGFW_mouseEnter) RGFW_window_showMouse(carbon_win__handle, false);
      if (e == RGFW_mouseLeave) RGFW_window_showMouse(carbon_win__handle, true);
    }
  }
  return false;
}

void carbon_win_exit(void) {
  carbon_win__handle->event.type = RGFW_quit;
}

bool carbon_win_get_key_down(const CBN_KeyCode key) {
  return carbon_win__keys[key] && !carbon_win__prev_keys[key];
}

bool carbon_win_get_key(const CBN_KeyCode key) {
  return carbon_win__keys[key];
}

bool carbon_win_get_key_up(const CBN_KeyCode key) {
  return !carbon_win__keys[key] && carbon_win__prev_keys[key];
}

bool carbon_win_get_mouse_button_down(const CBN_MouseButton btn) {
  return carbon_win__mouse_buttons[btn] && !carbon_win__prev_mouse_buttons[btn];
}

bool carbon_win_get_mouse_button(const CBN_MouseButton btn) {
  return carbon_win__mouse_buttons[btn];
}

bool carbon_win_get_mouse_button_up(const CBN_MouseButton btn) {
  return !carbon_win__mouse_buttons[btn] && carbon_win__prev_mouse_buttons[btn];
}

CBN_Vec2 carbon_win_get_mouse_position(void) {
  RGFW_point p = RGFW_window_getMousePoint(carbon_win__handle);
  return carbon_math_vec2(p.x, p.y);
}

void carbon_win_set_mouse_visibility(bool visible) {
  carbon_win__cursor_visible = visible;
}
