// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define CARBON_DRAWCANVAS__AA_RES 2

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

CARBON_INLINE void carbon_drawcanvas__alpha_blending(u32 *c1, u32 c2) {
  u32 r1 = (*c1 >> 24) & 0xff;
  u32 g1 = (*c1 >> 16) & 0xff;
  u32 b1 = (*c1 >> 8)  & 0xff;
  u32 a1 = (*c1 >> 0)  & 0xff;
  u32 r2 = (c2  >> 24) & 0xff;
  u32 g2 = (c2  >> 16) & 0xff;
  u32 b2 = (c2  >> 8)  & 0xff;
  u32 a2 = (c2  >> 0)  & 0xff;
  u32 r = (r1 * (255 - a2) + r2*a2) / 255;
  r = CARBON_CLAMP(r, 0, 255);
  u32 g = (g1 * (255 - a2) + g2*a2) / 255;
  g = CARBON_CLAMP(g, 0, 255);
  u32 b = (b1 * (255 - a2) + b2*a2) / 255;
  b = CARBON_CLAMP(b, 0, 255);
  u32 a = (a1 * (255 - a2) + a2*255) / 255;
  a = CARBON_CLAMP(a, 0, 255);
  *c1 = ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | ((a & 0xff) << 0);
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

CARBON_INLINE u8 carbon_drawcanvas__rect_normalize(const CBN_DrawCanvas dc, const CBN_Rect r, i32 *x1, i32 *x2, i32 *y1, i32 *y2) {
  if (!r.w || !r.h) return false;
  i32 ox1 = r.x;
  i32 oy1 = r.y;
  i32 ox2 = ox1 + CARBON_SIGN(r.w) * (carbon_math_abs(r.w) - 1);
  if (ox1 > ox2) CARBON_SWAP(i32, ox1, ox2);
  i32 oy2 = oy1 + CARBON_SIGN(r.h) * (carbon_math_abs(r.h) - 1);
  if (oy1 > oy2) CARBON_SWAP(i32, oy1, oy2);
  if (ox1 >= (i32) dc.width || ox2 < 0 || oy1 >= (i32) dc.height || oy2 < 0) return false;
  *x1 = ox1;
  if (*x1 < 0) *x1 = 0;
  *x2 = ox2;
  if (*x2 >= (i32) dc.width) *x2 = dc.width - 1;
  *y1 = oy1;
  if (*y1 < 0) *y1 = 0;
  *y2 = oy2;
  if (*y2 >= (i32) dc.height) *y2 = dc.height - 1;
  return true;
}

void carbon_drawcanvas_rect(CBN_DrawCanvas dc, CBN_Rect r, u32 color) {
  i32 x1, x2, y1, y2;
  if (!carbon_drawcanvas__rect_normalize(dc, r, &x1, &x2, &y1, &y2)) return;
  for (i32 i = x1; i <= x2; ++i) {
    for (i32 j = y1; j <= y2; ++j) {
      carbon_drawcanvas__alpha_blending(&CARBON_DRAWCANVAS_AT(dc, i, j), color);
    }
  }
}

void carbon_drawcanvas_circle(CBN_DrawCanvas dc, CBN_Vec2 center, usz radius, u32 color) {
  i32 x1, x2, y1, y2;
  i32 r = radius + CARBON_SIGN(radius);
  if (!carbon_drawcanvas__rect_normalize(dc, CARBON_RECT(center.x - r, center.y - r, 2*r, 2*r), &x1, &x2, &y1, &y2)) return;
  for (i32 j = y1; j <= y2; ++j) {
    for (i32 i = x1; i <= x2; ++i) {
      i64 n = 0;
      for (i64 sx = 0; sx < CARBON_DRAWCANVAS__AA_RES; ++sx) {
        for (i64 sy = 0; sy < CARBON_DRAWCANVAS__AA_RES; ++sy) {
          i64 res = CARBON_DRAWCANVAS__AA_RES + 1;
          i64 dx = (i * res * 2) + 2 + (sx * 2) - (res * center.x * 2) - res;
          i64 dy = (j * res * 2) + 2 + (sy * 2) - (res * center.y * 2) - res;
          if (dx*dx + dy*dy <= (i64) (res*res * radius*radius * 4)) ++n;
        }
      }
      u32 aa_alpha = ((color >> 0) & 0xff) * (n / CARBON_DRAWCANVAS__AA_RES / CARBON_DRAWCANVAS__AA_RES);
      u32 aa_color = (color & 0xffffff00) | (aa_alpha << 0);
      carbon_drawcanvas__alpha_blending(&CARBON_DRAWCANVAS_AT(dc, i, j), aa_color);
    }
  }
}
