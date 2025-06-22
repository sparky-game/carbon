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

void carbon_win_open(u16 width, u16 height, const char *title) {
  carbon_win__dl_open();
  // TODO: let user resize the window.
  carbon_win__handle = RGFW_createWindow(title, RGFW_RECT(0, 0, width, height), RGFW_windowNoResize);
  // TODO: use `RGFW_window_initBuffer` instead, so the buffer isn't tied up to the initial window size.
  RGFW_window_initBufferSize(carbon_win__handle, RGFW_AREA(carbon_win__handle->r.w, carbon_win__handle->r.h));
  carbon_log_info("Opened a %$x%$ window", $(carbon_win__handle->r.w), $(carbon_win__handle->r.h));
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
  RGFW_window_checkEvent(carbon_win__handle);
  return RGFW_window_shouldClose(carbon_win__handle);
}
