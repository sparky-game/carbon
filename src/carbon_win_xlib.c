// SPDX-License-Identifier: AGPL-3.0-only
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
    carbon_log_error("unable to connect to X server");
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
    carbon_log_error("no 32bpp display available");
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
  XSelectInput(carbon_win__display, carbon_win__window, KeyPressMask | KeyReleaseMask);
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
                                  0, 0,
                                  width, height,
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

void carbon_win_update(CBN_DrawCanvas dc) {
  carbon_win__ximg->data = (char *) dc.pixels;
  XPutImage(carbon_win__display,
            carbon_win__window,
            carbon_win__window_gc,
            carbon_win__ximg,
            0, 0, 0, 0,
            carbon_win__viewport_width,
            carbon_win__viewport_height);
  XFlush(carbon_win__display);
}

u8 carbon_win_shouldclose(void) {
  if (XPending(carbon_win__display)) {
    XEvent event;
    XNextEvent(carbon_win__display, &event);
    if (event.type == KeyPress || event.type == KeyRelease) {
      KeySym ks = XLookupKeysym(&event.xkey, 0);
      if (event.type == KeyPress) {
        // TODO: callback to keypress user-defined func
        // NOTE: 0xff is FUNCTION_KEY and 0x1b is ESCAPE_KEY
        if ((ks >> 8) == 0xff && (ks & 0xff) == 0x1b) return true;
      }
      else {
        // TODO: callback to keyrelease user-defined func
      }
    }
  }
  return false;
}

#endif  // defined(__linux__) || defined(__FreeBSD__)
