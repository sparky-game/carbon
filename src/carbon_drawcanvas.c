// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

CBN_DrawCanvas carbon_drawcanvas_create(usz width, usz height) {
  CBN_DrawCanvas dc = {
    .pixels = (u32 *) CARBON_MALLOC(width * height * sizeof(u32)),
    .width = width,
    .height = height,
    .stride = width
  };
  if (!dc.pixels) {
    CARBON_ERROR("failed to allocate memory (%zuB)", width * height * sizeof(u32));
    memset(&dc, 0, sizeof(CBN_DrawCanvas));
  }
  return dc;
}

void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc) {
  if (!dc) {
    CARBON_WARNING("`dc` is not a valid pointer, skipping destruction");
    return;
  }
  CARBON_FREE(dc->pixels);
  memset(dc, 0, sizeof(CBN_DrawCanvas));
  dc = 0;
}

void carbon_drawcanvas_fill(CBN_DrawCanvas dc, u32 color) {
  for (usz j = 0; j < dc.height; ++j) {
    for (usz i = 0; i < dc.width; ++i) {
      CARBON_DRAWCANVAS_AT(dc, i, j) = color;
    }
  }
}
