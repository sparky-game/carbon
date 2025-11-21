// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_WIN__DLDECL(name)                \
  typedef typeof(&name) name ## _ptr_t;         \
  static name ## _ptr_t name ## _ptr;

#if defined(__linux__) || defined(__FreeBSD__)
#define CARBON_WIN__DLOPEN(lib, name)                           \
  lib = dlmopen(LM_ID_NEWLM, name, RTLD_LAZY | RTLD_LOCAL);     \
  CBN_ASSERT(lib && "Failed to load");
#else
#define CARBON_WIN__DLOPEN(lib, name)           \
  lib = dlopen(name, RTLD_LAZY | RTLD_LOCAL);   \
  CBN_ASSERT(lib && "Failed to load");
#endif

#define CARBON_WIN__DLSYM(lib, name)                    \
  if (!name ## _ptr) {                                  \
    name ## _ptr = (name ## _ptr_t) dlsym(lib, #name);  \
    CBN_ASSERT(name ## _ptr && "Failed to load");       \
  }

// NOTE: we ignore here these warnings because RGFW do strange things
// like shadowing variables, leaving labels unused, using deprecated functions, and such.
CARBON_COMPILER_DIAG_BEGIN;
CARBON_COMPILER_DIAG_IGNORE("-Wshadow");
CARBON_COMPILER_DIAG_IGNORE("-Wunused-label");
CARBON_COMPILER_DIAG_IGNORE("-Wdeprecated-declarations");
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

#if defined(__APPLE__)
#include "carbon_win_dl_cocoa.inl"
#include "carbon_win_dl_corevideo.inl"
#include "carbon_win_dl_iokit.inl"
#elif defined(__linux__) || defined(__FreeBSD__)
#include "carbon_win_dl_x11.inl"
#include "carbon_win_dl_xrandr.inl"
#endif

static RGFW_window *carbon_win__handle;
static usz *carbon_win__xtable;
static usz *carbon_win__ytable;
static u32 carbon_win__max_fps;
static u32 carbon_win__curr_fps;
static u32 carbon_win__fps;
static CBN_Chrono carbon_win__fps_timer;
static CBN_Image carbon_win__icon;
static bool carbon_win__keys[RGFW_keyLast];
static bool carbon_win__prev_keys[RGFW_keyLast];
static bool carbon_win__mouse_buttons[RGFW_mouseFinal];
static bool carbon_win__prev_mouse_buttons[RGFW_mouseFinal];

CARBON_INLINE void carbon_win__dl_open(void) {
#if defined(__APPLE__)
  CARBON_WIN__DLOPEN(carbon_win__dl_Cocoa, "/System/Library/Frameworks/Cocoa.framework/Cocoa");
  CARBON_WIN__DLOPEN(carbon_win__dl_CoreVideo, "/System/Library/Frameworks/CoreVideo.framework/CoreVideo");
  CARBON_WIN__DLOPEN(carbon_win__dl_IOKit, "/System/Library/Frameworks/IOKit.framework/IOKit");
#elif defined(__linux__) || defined(__FreeBSD__)
  CARBON_WIN__DLOPEN(carbon_win__dl_X11, "libX11.so");
  CARBON_WIN__DLOPEN(carbon_win__dl_Xrandr, "libXrandr.so");
#endif
}

CARBON_INLINE void carbon_win__dl_close(void) {
#if defined(__APPLE__)
  dlclose(carbon_win__dl_Cocoa);
  dlclose(carbon_win__dl_CoreVideo);
  dlclose(carbon_win__dl_IOKit);
#elif defined(__linux__) || defined(__FreeBSD__)
  dlclose(carbon_win__dl_X11);
  dlclose(carbon_win__dl_Xrandr);
#endif
}

CARBON_INLINE RGFW_key carbon_win__map_keycodes(const CBN_KeyCode key) {
  switch (key) {
  case CARBON_KEY_CODE_A:            return RGFW_a;
  case CARBON_KEY_CODE_B:            return RGFW_b;
  case CARBON_KEY_CODE_C:            return RGFW_c;
  case CARBON_KEY_CODE_D:            return RGFW_d;
  case CARBON_KEY_CODE_E:            return RGFW_e;
  case CARBON_KEY_CODE_F:            return RGFW_f;
  case CARBON_KEY_CODE_G:            return RGFW_g;
  case CARBON_KEY_CODE_H:            return RGFW_h;
  case CARBON_KEY_CODE_I:            return RGFW_i;
  case CARBON_KEY_CODE_J:            return RGFW_j;
  case CARBON_KEY_CODE_K:            return RGFW_k;
  case CARBON_KEY_CODE_L:            return RGFW_l;
  case CARBON_KEY_CODE_M:            return RGFW_m;
  case CARBON_KEY_CODE_N:            return RGFW_n;
  case CARBON_KEY_CODE_O:            return RGFW_o;
  case CARBON_KEY_CODE_P:            return RGFW_p;
  case CARBON_KEY_CODE_Q:            return RGFW_q;
  case CARBON_KEY_CODE_R:            return RGFW_r;
  case CARBON_KEY_CODE_S:            return RGFW_s;
  case CARBON_KEY_CODE_T:            return RGFW_t;
  case CARBON_KEY_CODE_U:            return RGFW_u;
  case CARBON_KEY_CODE_V:            return RGFW_v;
  case CARBON_KEY_CODE_W:            return RGFW_w;
  case CARBON_KEY_CODE_X:            return RGFW_x;
  case CARBON_KEY_CODE_Y:            return RGFW_y;
  case CARBON_KEY_CODE_Z:            return RGFW_z;
  case CARBON_KEY_CODE_Zero:         return RGFW_0;
  case CARBON_KEY_CODE_One:          return RGFW_1;
  case CARBON_KEY_CODE_Two:          return RGFW_2;
  case CARBON_KEY_CODE_Three:        return RGFW_3;
  case CARBON_KEY_CODE_Four:         return RGFW_4;
  case CARBON_KEY_CODE_Five:         return RGFW_5;
  case CARBON_KEY_CODE_Six:          return RGFW_6;
  case CARBON_KEY_CODE_Seven:        return RGFW_7;
  case CARBON_KEY_CODE_Eight:        return RGFW_8;
  case CARBON_KEY_CODE_Nine:         return RGFW_9;
  case CARBON_KEY_CODE_BackQuote:    return RGFW_backtick;
  case CARBON_KEY_CODE_F1:           return RGFW_F1;
  case CARBON_KEY_CODE_F2:           return RGFW_F2;
  case CARBON_KEY_CODE_F3:           return RGFW_F3;
  case CARBON_KEY_CODE_F4:           return RGFW_F4;
  case CARBON_KEY_CODE_F5:           return RGFW_F5;
  case CARBON_KEY_CODE_F6:           return RGFW_F6;
  case CARBON_KEY_CODE_F7:           return RGFW_F7;
  case CARBON_KEY_CODE_F8:           return RGFW_F8;
  case CARBON_KEY_CODE_F9:           return RGFW_F9;
  case CARBON_KEY_CODE_F10:          return RGFW_F10;
  case CARBON_KEY_CODE_F11:          return RGFW_F11;
  case CARBON_KEY_CODE_F12:          return RGFW_F12;
  case CARBON_KEY_CODE_Escape:       return RGFW_escape;
  case CARBON_KEY_CODE_Tab:          return RGFW_tab;
  case CARBON_KEY_CODE_CapsLock:     return RGFW_capsLock;
  case CARBON_KEY_CODE_LeftShift:    return RGFW_shiftL;
  case CARBON_KEY_CODE_LeftControl:  return RGFW_controlL;
  case CARBON_KEY_CODE_LeftMeta:     return RGFW_superL;
  case CARBON_KEY_CODE_LeftAlt:      return RGFW_altL;
  case CARBON_KEY_CODE_Space:        return RGFW_space;
  case CARBON_KEY_CODE_RightAlt:     return RGFW_altR;
  case CARBON_KEY_CODE_RightMeta:    return RGFW_superR;
  case CARBON_KEY_CODE_RightControl: return RGFW_controlR;
  case CARBON_KEY_CODE_RightShift:   return RGFW_shiftR;
  case CARBON_KEY_CODE_Return:       return RGFW_return;
  case CARBON_KEY_CODE_UpArrow:      return RGFW_up;
  case CARBON_KEY_CODE_DownArrow:    return RGFW_down;
  case CARBON_KEY_CODE_LeftArrow:    return RGFW_left;
  case CARBON_KEY_CODE_RightArrow:   return RGFW_right;
  default:                           return RGFW_keyNULL;
  }
}

CARBON_INLINE RGFW_mouseButton carbon_win__map_mouse_buttons(const CBN_MouseButton btn) {
  switch (btn) {
  case CARBON_MOUSE_BUTTON_Left:       return RGFW_mouseLeft;
  case CARBON_MOUSE_BUTTON_Right:      return RGFW_mouseRight;
  case CARBON_MOUSE_BUTTON_Middle:     return RGFW_mouseMiddle;
  case CARBON_MOUSE_BUTTON_ScrollUp:   return RGFW_mouseScrollUp;
  case CARBON_MOUSE_BUTTON_ScrollDown: return RGFW_mouseScrollDown;
  default:
    CARBON_UNREACHABLE;
    return RGFW_mouseFinal;
  }
}

CARBON_INLINE void carbon_win__rebuild_xtable(usz dst_w, usz src_w) {
  carbon_win__xtable = (usz *) carbon_memory_realloc(carbon_win__xtable, dst_w * sizeof(usz));
  const f32 sf = (f32) src_w / dst_w;
  for (usz i = 0; i < dst_w; ++i) {
    carbon_win__xtable[i] = CARBON_MIN(i * sf, src_w - 1);
  }
}

CARBON_INLINE void carbon_win__rebuild_ytable(usz dst_h, usz src_h) {
  carbon_win__ytable = (usz *) carbon_memory_realloc(carbon_win__ytable, dst_h * sizeof(usz));
  const f32 sf = (f32) src_h / dst_h;
  for (usz j = 0; j < dst_h; ++j) {
    carbon_win__ytable[j] = CARBON_MIN(j * sf, src_h - 1);
  }
}

CARBON_INLINE void carbon_win__resize_callback(RGFW_window *win, RGFW_rect r) {
  // TODO: investigate the height offset thing (-28) in other systems
  CBN_DEBUG("r = (%d, %d, %d, %d - 28)", r.x, r.y, r.w, r.h);
  win->r.h -= 28;
}

CARBON_INLINE void carbon_win__key_callback(RGFW_window *win, u8 key, char keyChar, RGFW_keymod keyMod, RGFW_bool pressed) {
  CARBON_UNUSED(keyChar), CARBON_UNUSED(keyMod);
  if (win != carbon_win__handle) return;
  carbon_win__keys[key] = pressed ? true : false;
}

CARBON_INLINE void carbon_win__mouse_button_callback(RGFW_window* win, RGFW_mouseButton button, f64 scroll, RGFW_bool pressed) {
  CARBON_UNUSED(scroll);
  if (win != carbon_win__handle) return;
  carbon_win__mouse_buttons[button] = pressed ? true : false;
}

void carbon_win_open(const CBN_DrawCanvas dc, const char *title) {
  carbon_win__dl_open();
  const usz w = dc.width, h = dc.height;
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
}

void carbon_win_close(void) {
  carbon_memory_free(carbon_win__xtable);
  carbon_memory_free(carbon_win__ytable);
  if (carbon_win__icon.data) carbon_fs_destroy_img(&carbon_win__icon);
  RGFW_window_close(carbon_win__handle);
  carbon_win__dl_close();
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

CARBON_INLINE void carbon_win__resize_buf(const CBN_DrawCanvas dc) {
  RGFW_window *w = carbon_win__handle;
  if (w->bufferSize.w == (u32) w->r.w && w->bufferSize.h == (u32) w->r.h) return;
  w->bufferSize.w = w->r.w;
  w->bufferSize.h = w->r.h;
  CBN_DEBUG("buffer = %zux%zu", w->bufferSize.w, w->bufferSize.h);
  const usz sz = w->bufferSize.w * w->bufferSize.h * 4;
  w->buffer = (u8 *) carbon_memory_realloc(w->buffer, sz);
  carbon_memory_set(w->buffer, 0, sz);
  carbon_win__rebuild_xtable(w->bufferSize.w, dc.width);
  carbon_win__rebuild_ytable(w->bufferSize.h, dc.height);
}

CARBON_INLINE void carbon_win__upscale_buf(const CBN_DrawCanvas dc) {
  // Nearest-neighbor interpolation algorithm
  const u32 * restrict src = dc.pixels;
  const usz src_w = dc.width;
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

void carbon_win_update(const CBN_DrawCanvas dc) {
  carbon_win__resize_buf(dc);
  carbon_win__upscale_buf(dc);
  RGFW_window_swapBuffers(carbon_win__handle);
  carbon_win__curr_fps = RGFW_window_checkFPS(carbon_win__handle, carbon_win__max_fps);
}

bool carbon_win_shouldclose(void) {
  for (u8 i = 0; i < RGFW_keyLast; ++i)    carbon_win__prev_keys[i]          = carbon_win__keys[i];
  for (u8 i = 0; i < RGFW_mouseFinal; ++i) carbon_win__prev_mouse_buttons[i] = carbon_win__mouse_buttons[i];
  RGFW_window_checkEvent(carbon_win__handle);
  return carbon_win__handle->event.type == RGFW_quit;
}

void carbon_win_exit(void) {
  carbon_win__handle->event.type = RGFW_quit;
}

bool carbon_win_get_key_down(const CBN_KeyCode key) {
  return carbon_win__keys[carbon_win__map_keycodes(key)] && !carbon_win__prev_keys[carbon_win__map_keycodes(key)];
}

bool carbon_win_get_key(const CBN_KeyCode key) {
  return carbon_win__keys[carbon_win__map_keycodes(key)];
}

bool carbon_win_get_key_up(const CBN_KeyCode key) {
  return !carbon_win__keys[carbon_win__map_keycodes(key)] && carbon_win__prev_keys[carbon_win__map_keycodes(key)];
}

bool carbon_win_get_mouse_button_down(const CBN_MouseButton btn) {
  return carbon_win__mouse_buttons[carbon_win__map_mouse_buttons(btn)] && !carbon_win__prev_mouse_buttons[carbon_win__map_mouse_buttons(btn)];
}

bool carbon_win_get_mouse_button(const CBN_MouseButton btn) {
  return carbon_win__mouse_buttons[carbon_win__map_mouse_buttons(btn)];
}

bool carbon_win_get_mouse_button_up(const CBN_MouseButton btn) {
  return !carbon_win__mouse_buttons[carbon_win__map_mouse_buttons(btn)] && carbon_win__prev_mouse_buttons[carbon_win__map_mouse_buttons(btn)];
}

CBN_Vec2 carbon_win_get_mouse_position(void) {
  RGFW_point p = RGFW_window_getMousePoint(carbon_win__handle);
  return CARBON_VEC2(p.x, p.y);
}
