// SPDX-License-Identifier: AGPL-3.0-only
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
    carbon_log_error("failed to allocate memory (%zuB)", width * height * sizeof(u32));
    memset(&dc, 0, sizeof(CBN_DrawCanvas));
  }
  return dc;
}

void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc) {
  if (!dc) {
    carbon_log_warn("`dc` is not a valid pointer, skipping destruction");
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

CARBON_INLINE u8 carbon_drawcanvas__triangle_norm(const CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, usz *lx, usz *hx, usz *ly, usz *hy) {
  *lx = *hx = v1.x;
  *ly = *hy = v1.y;
  if (*lx > v2.x) *lx = v2.x;
  if (*hx < v2.x) *hx = v2.x;
  if (*ly > v2.y) *ly = v2.y;
  if (*hy < v2.y) *hy = v2.y;
  if (*lx > v3.x) *lx = v3.x;
  if (*hx < v3.x) *hx = v3.x;
  if (*ly > v3.y) *ly = v3.y;
  if (*hy < v3.y) *hy = v3.y;
  *lx = CARBON_CLAMP(*lx, 0, dc.width - 1);
  *hx = CARBON_CLAMP(*hx, 0, dc.width - 1);
  *ly = CARBON_CLAMP(*ly, 0, dc.height - 1);
  *hy = CARBON_CLAMP(*hy, 0, dc.height - 1);
  if (*lx > *hx || *ly > *hy) return false;
  return true;
}

CARBON_INLINE u8 carbon_drawcanvas__triangle_barycentric(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, usz x, usz y, i32 *u1, i32 *u2, i32 *det) {
  *det = ((v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y));
  *u1  = ((v2.y - v3.y) * (x - v3.x)    + (v3.x - v2.x) * (y - v3.y));
  *u2  = ((v3.y - v1.y) * (x - v3.x)    + (v1.x - v3.x) * (y - v3.y));
  i32 u3 = *det - *u1 - *u2;
  return ((CARBON_SIGN(*u1) == CARBON_SIGN(*det) || *u1 == 0) &&
          (CARBON_SIGN(*u2) == CARBON_SIGN(*det) || *u2 == 0) &&
          (CARBON_SIGN(u3)  == CARBON_SIGN(*det) || u3  == 0));
}

void carbon_drawcanvas_triangle(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) {
  usz lx, hx, ly, hy;
  if (!carbon_drawcanvas__triangle_norm(dc, v1, v2, v3, &lx, &hx, &ly, &hy)) return;
  for (usz j = ly; j <= hy; ++j) {
    for (usz i = lx; i <= hx; ++i) {
      i32 u1, u2, det;
      if (!carbon_drawcanvas__triangle_barycentric(v1, v2, v3, i, j, &u1, &u2, &det)) continue;
      CARBON_DRAWCANVAS_AT(dc, i, j) = color;
    }
  }
}
