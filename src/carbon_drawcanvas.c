// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_DRAWCANVAS__AA_RES                2
#define CARBON_DRAWCANVAS__BOX_OUTLINE_COLOR     0x000000ff
#define CARBON_DRAWCANVAS__BOX_TOPLEFT_COLOR     0xffffffff
#define CARBON_DRAWCANVAS__BOX_BOTTOMRIGHT_COLOR 0x555555ff
#define CARBON_DRAWCANVAS__BOX_INSIDE_COLOR      0xc6c6c6ff

#include "carbon_drawcanvas_font.inl"

CBN_DrawCanvas carbon_drawcanvas_create(usz width, usz height) {
  return (CBN_DrawCanvas) {
    .pixels = (u32 *) carbon_memory_alloc(width * height * sizeof(u32)),
    .width = width,
    .height = height
  };
}

void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc) {
  if (!dc) {
    CBN_WARN("`dc` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(dc->pixels);
  carbon_memory_set(dc, 0, sizeof(*dc));
}

void carbon_drawcanvas_fill(CBN_DrawCanvas dc, u32 color) {
  for (usz i = 0; i < dc.width * dc.height; ++i) {
    dc.pixels[i] = color;
  }
}

CARBON_INLINE void carbon_drawcanvas__alpha_blending(u32 *c1, u32 c2) {
  u32 a2 = c2 & 0xff;
  if (a2 == 0) return;
  if (a2 == 255) {
    *c1 = c2;
    return;
  }
  u32 r1 = (*c1 >> 24) & 0xff;
  u32 g1 = (*c1 >> 16) & 0xff;
  u32 b1 = (*c1 >> 8)  & 0xff;
  u32 a1 = (*c1 >> 0)  & 0xff;
  u32 r2 = (c2  >> 24) & 0xff;
  u32 g2 = (c2  >> 16) & 0xff;
  u32 b2 = (c2  >> 8)  & 0xff;
  u32 r = (r1 * (255 - a2) + r2 * a2) / 255;
  u32 g = (g1 * (255 - a2) + g2 * a2) / 255;
  u32 b = (b1 * (255 - a2) + b2 * a2) / 255;
  u32 a = a2 + (a1 * (255 - a2)) / 255;
  *c1 = (r << 24) | (g << 16) | (b << 8) | a;
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
  *det = (v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y);
  *u1  = (v2.y - v3.y) * (x - v3.x) + (v3.x - v2.x) * (y - v3.y);
  *u2  = (v3.y - v1.y) * (x - v3.x) + (v1.x - v3.x) * (y - v3.y);
  i32 u3 = *det - *u1 - *u2;
  i8 det_sign = CARBON_SIGN(*det);
  return (CARBON_SIGN(*u1) == det_sign || !*u1) && (CARBON_SIGN(*u2) == det_sign || !*u2) && (CARBON_SIGN(u3) == det_sign || !u3);
}

void carbon_drawcanvas_triangle(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) {
  usz lx, hx, ly, hy;
  if (!carbon_drawcanvas__triangle_norm(dc, v1, v2, v3, &lx, &hx, &ly, &hy)) return;
  for (usz j = ly; j <= hy; ++j) {
    for (usz i = lx; i <= hx; ++i) {
      i32 u1, u2, det;
      if (!carbon_drawcanvas__triangle_barycentric(v1, v2, v3, i, j, &u1, &u2, &det)) continue;
      carbon_drawcanvas_at(dc, i, j) = color;
    }
  }
}

CARBON_INLINE u8 carbon_drawcanvas__rect_normalize(const CBN_DrawCanvas dc, const CBN_Rect r, i32 *x1, i32 *x2, i32 *y1, i32 *y2) {
  if (!r.w || !r.h) return false;
  i32 ox1 = r.x;
  i32 oy1 = r.y;
  i32 ox2 = ox1 + CARBON_SIGN(r.w) * (carbon_math_abs(r.w) - 1);
  if (ox1 > ox2) CARBON_SWAP(ox1, ox2);
  i32 oy2 = oy1 + CARBON_SIGN(r.h) * (carbon_math_abs(r.h) - 1);
  if (oy1 > oy2) CARBON_SWAP(oy1, oy2);
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
  u32 *p_dc = dc.pixels + (y1 * dc.width + x1);
  usz row_offset = x2 - x1 + 1;
  for (i32 j = y1; j <= y2; ++j) {
    for (i32 i = x1; i <= x2; ++i) {
      carbon_drawcanvas__alpha_blending(p_dc, color);
      ++p_dc;
    }
    p_dc += dc.width - row_offset;
  }
}

void carbon_drawcanvas_circle(CBN_DrawCanvas dc, CBN_Vec2 center, usz radius, u32 color) {
  i32 x1, x2, y1, y2;
  if (!carbon_drawcanvas__rect_normalize(dc, CARBON_RECT(center.x - radius, center.y - radius, 2*radius, 2*radius), &x1, &x2, &y1, &y2)) return;
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
      u32 aa_color = (color & 0xffffff00) | aa_alpha;
      carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, i, j), aa_color);
    }
  }
}

void carbon_drawcanvas_sprite(CBN_DrawCanvas dc, const CBN_Sprite *sprite, CBN_Vec2 position) {
  if (!carbon_math_rect_contains_point(CARBON_RECT(0, 0, dc.width, dc.height), position)) return;
  usz sp_w = CARBON_MIN(sprite->width, dc.width - position.x);
  usz sp_h = CARBON_MIN(sprite->height, dc.height - position.y);
  u32 *p_dc = dc.pixels + (usz)(position.y * dc.width + position.x);
  const u32 *p_sp = sprite->pixels;
  for (usz j = 0; j < sp_h; ++j) {
    for (usz i = 0; i < sp_w; ++i) {
      carbon_drawcanvas__alpha_blending(p_dc, *p_sp);
      ++p_dc, ++p_sp;
    }
    p_dc += dc.width - sp_w;
  }
}

void carbon_drawcanvas_box(CBN_DrawCanvas dc, CBN_Rect r) {
#define PX(i, j, c) carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, (usz)(i), (usz)(j)), (c));
#define OUTLINE(i, j)     PX(i, j, CARBON_DRAWCANVAS__BOX_OUTLINE_COLOR)
#define TOPLEFT(i, j)     PX(i, j, CARBON_DRAWCANVAS__BOX_TOPLEFT_COLOR)
#define BOTTOMRIGHT(i, j) PX(i, j, CARBON_DRAWCANVAS__BOX_BOTTOMRIGHT_COLOR)
#define INSIDE(i, j)      PX(i, j, CARBON_DRAWCANVAS__BOX_INSIDE_COLOR)
  // Header
  for (usz j = r.y; j <= r.y + 1; ++j) {
    for (usz i = r.x + 4; i <= r.w - 6; ++i) {
      OUTLINE(i, j);
    }
  }
  for (usz j = r.y + 2; j <= r.y + 3; ++j) {
    OUTLINE(r.x + 2, j);
    OUTLINE(r.x + 3, j);
    for (usz i = r.x + 4; i <= r.w - 6; ++i) {
      TOPLEFT(i, j);
    }
    OUTLINE(r.w - 5, j);
    OUTLINE(r.w - 4, j);
  }
  for (usz j = r.y + 4; j <= r.y + 5; ++j) {
    OUTLINE(r.x + 0, j);
    OUTLINE(r.x + 1, j);
    for (usz i = r.x + 2; i <= r.w - 6; ++i) {
      TOPLEFT(i, j);
    }
    INSIDE(r.w - 5, j);
    INSIDE(r.w - 4, j);
    OUTLINE(r.w - 3, j);
    OUTLINE(r.w - 2, j);
  }
  for (usz j = r.y + 6; j <= r.y + 7; ++j) {
    OUTLINE(r.x + 0, j);
    OUTLINE(r.x + 1, j);
    for (usz i = r.x + 2; i <= r.x + 7; ++i) {
      TOPLEFT(i, j);
    }
    for (usz i = r.x + 8; i <= r.w - 6; ++i) {
      INSIDE(i, j);
    }
    for (usz i = r.w - 5; i <= r.w - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(r.w - 1, j);
    OUTLINE(r.w - 0, j);
  }
  // Content
  for (usz j = r.y + 8; j <= r.h - 8; ++j) {
    OUTLINE(r.x + 0, j);
    OUTLINE(r.x + 1, j);
    for (usz i = r.x + 2; i <= r.x + 5; ++i) {
      TOPLEFT(i, j);
    }
    for (usz i = r.x + 6; i <= r.w - 6; ++i) {
      INSIDE(i, j);
    }
    for (usz i = r.w - 5; i <= r.w - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(r.w - 1, j);
    OUTLINE(r.w - 0, j);
  }
  // Footer
  for (usz j = r.h - 7; j <= r.h - 6; ++j) {
    OUTLINE(r.x + 0, j);
    OUTLINE(r.x + 1, j);
    for (usz i = r.x + 2; i <= r.x + 5; ++i) {
      TOPLEFT(i, j);
    }
    for (usz i = r.x + 6; i <= r.w - 8; ++i) {
      INSIDE(i, j);
    }
    for (usz i = r.w - 7; i <= r.w - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(r.w - 1, j);
    OUTLINE(r.w - 0, j);
  }
  for (usz j = r.h - 5; j <= r.h - 4; ++j) {
    OUTLINE(r.x + 2, j);
    OUTLINE(r.x + 3, j);
    INSIDE(r.x + 4, j);
    INSIDE(r.x + 5, j);
    for (usz i = r.x + 6; i <= r.w - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(r.w - 1, j);
    OUTLINE(r.w - 0, j);
  }
  for (usz j = r.h - 3; j <= r.h - 2; ++j) {
    OUTLINE(r.x + 4, j);
    OUTLINE(r.x + 5, j);
    for (usz i = r.x + 6; i <= r.w - 4; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(r.w - 3, j);
    OUTLINE(r.w - 2, j);
  }
  for (usz j = r.h - 1; j <= r.h; ++j) {
    for (usz i = r.x + 6; i <= r.w - 4; ++i) {
      OUTLINE(i, j);
    }
  }
#undef PX
#undef OUTLINE
#undef TOPLEFT
#undef BOTTOMRIGHT
#undef INSIDE
}

void carbon_drawcanvas_text(CBN_DrawCanvas dc, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  static const char *glyphs = &carbon_drawcanvas__font[0][0][0];
  for (usz i = 0; *txt; ++i, ++txt) {
    i32 gx = position.x + (i * CARBON_DRAWCANVAS__FONT_WIDTH * size);
    i32 gy = position.y;
    const char *glyph = &glyphs[(*txt) * CARBON_DRAWCANVAS__FONT_WIDTH * CARBON_DRAWCANVAS__FONT_HEIGHT];
    for (usz dy = 0; dy < CARBON_DRAWCANVAS__FONT_HEIGHT; ++dy) {
      for (usz dx = 0; dx < CARBON_DRAWCANVAS__FONT_WIDTH; ++dx) {
        i32 px = gx + dx*size;
        i32 py = gy + dy*size;
        if (0 <= px && px < (i32) dc.width && 0 <= py && py < (i32) dc.height && glyph[dy*CARBON_DRAWCANVAS__FONT_WIDTH + dx]) {
          carbon_drawcanvas_rect(dc, CARBON_RECT_SQUARE(px, py, size), color);
        }
      }
    }
  }
}

void carbon_drawcanvas_text_with_shadow(CBN_DrawCanvas dc, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  carbon_drawcanvas_text(dc, txt, CARBON_VEC2(position.x + 1 * size, position.y + 1 * size), size, 0x33333380);
  carbon_drawcanvas_text(dc, txt, position, size, color);
}

usz carbon_drawcanvas_get_text_width(const char *txt, usz size) {
  return CARBON_DRAWCANVAS__FONT_WIDTH * size * carbon_string_len(txt);
}

usz carbon_drawcanvas_get_text_height(usz size) {
  return CARBON_DRAWCANVAS__FONT_HEIGHT * size;
}

u32 carbon_drawcanvas_hsv_to_rgb(f32 h, f32 s, f32 v) {
  f32 k = carbon_math_fmod(5 + h/60, 6);
  k = CARBON_CLAMP(CARBON_MIN(4 - k, k), 0, 1);
  u32 r = (v - v*s*k) * 255;
  k = carbon_math_fmod(3 + h/60, 6);
  k = CARBON_CLAMP(CARBON_MIN(4 - k, k), 0, 1);
  u32 g = (v - v*s*k) * 255;
  k = carbon_math_fmod(1 + h/60, 6);
  k = CARBON_CLAMP(CARBON_MIN(4 - k, k), 0, 1);
  u32 b = (v - v*s*k) * 255;
  return ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | ((255 & 0xff) << 0);
}
