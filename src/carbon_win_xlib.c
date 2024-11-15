// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#if defined(__linux__) || defined(__FreeBSD__)

#include <X11/Xlib.h>
#include <X11/Xutil.h>

static Display *carbon_win__display;
static i32 carbon_win__screen;
static Visual *carbon_win__visual;
static i32 carbon_win__depth;
static XPixmapFormatValues *carbon_win__pixmap_fmts;
static i32 carbon_win__pixmap_fmt_count;
static i32 carbon_win__converter_depth;
static i32 carbon_win__screen_width;
static i32 carbon_win__screen_height;
static Window carbon_win__root_window;
static XSetWindowAttributes carbon_win__window_attrs;
static i32 carbon_win__x_pos;
static i32 carbon_win__y_pos;
static Window carbon_win__window;
static XSizeHints carbon_win__window_szhints;
static GC carbon_win__window_gc;
static XImage *carbon_win__ximg;
static i32 carbon_win__out_pitch;
static i32 carbon_win__viewport_width;
static i32 carbon_win__viewport_height;

void carbon_win_open(u16 width, u16 height, const char *title) {
  carbon_win__display = XOpenDisplay(0);
  if (!carbon_win__display) {
    CARBON_ERROR("unable to connect to X server");
    return;
  }
  carbon_win__screen = DefaultScreen(carbon_win__display);
  carbon_win__visual = DefaultVisual(carbon_win__display, carbon_win__screen);
  carbon_win__depth = DefaultDepth(carbon_win__display, carbon_win__screen);
  carbon_win__pixmap_fmts = XListPixmapFormats(carbon_win__display, &carbon_win__pixmap_fmt_count);
  for (i32 i = 0; i < carbon_win__pixmap_fmt_count; ++i) {
    if (carbon_win__depth == carbon_win__pixmap_fmts[i].depth) {
      carbon_win__converter_depth = carbon_win__pixmap_fmts[i].bits_per_pixel;
    }
  }
  XFree(carbon_win__pixmap_fmts);
  // TODO: perform color depth conversions
  if (32 != carbon_win__converter_depth) {
    CARBON_ERROR("no 32bpp display available");
    XCloseDisplay(carbon_win__display);
    return;
  }
  carbon_win__out_pitch = 4;
  carbon_win__screen_width = DisplayWidth(carbon_win__display, carbon_win__screen);
  carbon_win__screen_height = DisplayHeight(carbon_win__display, carbon_win__screen);
  carbon_win__root_window = DefaultRootWindow(carbon_win__display);
  carbon_win__window_attrs.border_pixel = BlackPixel(carbon_win__display, carbon_win__screen);
  carbon_win__window_attrs.background_pixel = BlackPixel(carbon_win__display, carbon_win__screen);
  carbon_win__window_attrs.backing_store = NotUseful;
  carbon_win__x_pos = (carbon_win__screen_width - width) / 2;
  carbon_win__y_pos = (carbon_win__screen_height - height) / 2;
  carbon_win__window = XCreateWindow(carbon_win__display,
                                     carbon_win__root_window,
                                     carbon_win__x_pos,
                                     carbon_win__y_pos,
                                     width,
                                     height,
                                     0,
                                     carbon_win__depth,
                                     InputOutput,
                                     carbon_win__visual,
                                     CWBackPixel | CWBorderPixel | CWBackingStore,
                                     &carbon_win__window_attrs);
  XStoreName(carbon_win__display, carbon_win__window, title);
  // XSelectInput(carbon_win__display, carbon_win__window, KeyPressMask | KeyReleaseMask);
  carbon_win__window_szhints.flags = PPosition | PMinSize | PMaxSize;
  carbon_win__window_szhints.x = 0;
  carbon_win__window_szhints.y = 0;
  carbon_win__window_szhints.min_width = width;
  carbon_win__window_szhints.max_width = width;
  carbon_win__window_szhints.min_height = height;
  carbon_win__window_szhints.max_height = height;
  XSetWMNormalHints(carbon_win__display, carbon_win__window, &carbon_win__window_szhints);
  XClearWindow(carbon_win__display, carbon_win__window);
  XMapRaised(carbon_win__display, carbon_win__window);
  XFlush(carbon_win__display);
  carbon_win__window_gc = DefaultGC(carbon_win__display, carbon_win__screen);
  carbon_win__ximg = XCreateImage(carbon_win__display,
                                  CopyFromParent,
                                  carbon_win__depth,
                                  ZPixmap,
                                  0,
                                  0,
                                  width,
                                  height,
                                  32,
                                  width * carbon_win__out_pitch);
  carbon_win__viewport_width = width;
  carbon_win__viewport_height = height;
}

void carbon_win_close(void) {
  carbon_win__ximg->data = 0;
  XDestroyImage(carbon_win__ximg);
  XDestroyWindow(carbon_win__display, carbon_win__window);
  XCloseDisplay(carbon_win__display);
}

#endif  // defined(__linux__) || defined(__FreeBSD__)
