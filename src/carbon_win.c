// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_WIN__DLDECL(name)                \
  typedef CARBON_TYPE_OF(&name) name ## _ptr_t; \
  static name ## _ptr_t name ## _ptr;

#if defined(__linux__) || defined(__FreeBSD__)
#define CARBON_WIN__DLOPEN(lib, name)                           \
  lib = dlmopen(LM_ID_NEWLM, name, RTLD_LAZY | RTLD_LOCAL);     \
  CARBON_ASSERT(lib && "Failed to load");
#else
#define CARBON_WIN__DLOPEN(lib, name)           \
  lib = dlopen(name, RTLD_LAZY | RTLD_LOCAL);   \
  CARBON_ASSERT(lib && "Failed to load");
#endif

#define CARBON_WIN__DLSYM(lib, name)                    \
  if (!name ## _ptr) {                                  \
    name ## _ptr = (name ## _ptr_t) dlsym(lib, #name);  \
    CARBON_ASSERT(name ## _ptr && "Failed to load");    \
  }

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

#if defined(__APPLE__)
#include "carbon_win_dl_cocoa.inl"
#include "carbon_win_dl_corevideo.inl"
#include "carbon_win_dl_iokit.inl"
#elif defined(__linux__) || defined(__FreeBSD__)
#include "carbon_win_dl_x11.inl"
#include "carbon_win_dl_xrandr.inl"
#endif

static RGFW_window *carbon_win__handle;
static u32 carbon_win__max_fps;
static CBN_Image carbon_win__icon;
static u8 carbon_win__keys[RGFW_keyLast];
static u8 carbon_win__prev_keys[RGFW_keyLast];

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

CARBON_INLINE void carbon_win__key_callback(RGFW_window *win, u8 key, char keyChar, RGFW_keymod keyMod, RGFW_bool pressed) {
  CARBON_UNUSED(keyChar), CARBON_UNUSED(keyMod);
  if (win != carbon_win__handle) return;
  if (pressed) carbon_win__keys[key] = true;
  else carbon_win__keys[key] = false;
}

void carbon_win_open(u16 width, u16 height, const char *title) {
  carbon_win__dl_open();
  // TODO: let user resize the window.
  carbon_win__handle = RGFW_createWindow(title, RGFW_RECT(0, 0, width, height), RGFW_windowNoResize);
  // TODO: use `RGFW_window_initBuffer` instead, so the buffer isn't tied up to the initial window size.
  RGFW_window_initBufferSize(carbon_win__handle, RGFW_AREA(carbon_win__handle->r.w, carbon_win__handle->r.h));
  carbon_log_info("Opened a %$x%$ window", $(carbon_win__handle->r.w), $(carbon_win__handle->r.h));

  RGFW_setKeyCallback(carbon_win__key_callback);
}

void carbon_win_close(void) {
  if (carbon_win__icon.data) carbon_fs_destroy_img(&carbon_win__icon);
  RGFW_window_close(carbon_win__handle);
  carbon_win__dl_close();
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
  for (u8 i = 0; i < RGFW_keyLast; ++i) carbon_win__prev_keys[i] = carbon_win__keys[i];
  RGFW_window_checkEvent(carbon_win__handle);
  return RGFW_window_shouldClose(carbon_win__handle);
}

u8 carbon_win_get_key_down(const CBN_KeyCode key) {
  return carbon_win__keys[carbon_win__map_keycodes(key)];
}

u8 carbon_win_get_key_up(const CBN_KeyCode key) {
  return !carbon_win__keys[carbon_win__map_keycodes(key)] && carbon_win__prev_keys[carbon_win__map_keycodes(key)];
}
