// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define stbtt_uint8       u8
#define stbtt_int8        i8
#define stbtt_uint16      u16
#define stbtt_int16       i16
#define stbtt_uint32      u32
#define stbtt_int32       i32
#define STBTT_ifloor(x)   ((int) carbon_math_floor(x))
#define STBTT_iceil(x)    ((int) carbon_math_ceil(x))
#define STBTT_sqrt(x)     carbon_math_sqrt(x)
#define STBTT_pow(x,y)    carbon_math_pow(x,y)
#define STBTT_fmod(x,y)   carbon_math_fmod(x,y)
#define STBTT_cos(x)      carbon_math_cos(x)
#define STBTT_acos(x)     carbon_math_acos(x)
#define STBTT_fabs(x)     carbon_math_abs(x)
#define STBTT_malloc(x,u) ((void)(u),carbon_memory_alloc(x))
#define STBTT_free(x,u)   ((void)(u),carbon_memory_free(x))
#define STBTT_assert(x)   CBN_ASSERT(x)
#define STBTT_strlen(x)   carbon_string_len(x)
#define STBTT_memcpy      carbon_memory_copy
#define STBTT_memset      carbon_memory_set
#define STB_TRUETYPE_IMPLEMENTATION
#include "../thirdparty/stb/stb_truetype.h"

#define CARBON_FONT__SDF_PADDING 2
#define CARBON_FONT__SDF_ONEDGE  180
#define CARBON_FONT__SDF_PXSCALE 32

CBN_Font carbon_font_create_from_file(const char *file, usz size) {
  CBN_Font f = {0};
  CBN_List data = carbon_list_create(sizeof(u8));
  CBN_ASSERT(carbon_fs_read_entire_file(&data, file));
  stbtt_fontinfo fi;
  CBN_ASSERT(stbtt_InitFont(&fi, data.items, 0));
  f32 scale = stbtt_ScaleForPixelHeight(&fi, size);
  i32 pen_x = 0, pen_y = 0, row_h = 0;
  for (i32 c = CARBON_FONT_ASCII_START; c < CARBON_FONT_ASCII_START + CARBON_FONT_ASCII_COUNT; ++c) {
    usz i = c - CARBON_FONT_ASCII_START;
    i32 advance, lsb;
    stbtt_GetCodepointHMetrics(&fi, c, &advance, &lsb);
    f.metadata.cdata[i].xadvance = advance * scale;
    i32 w, h, xoff, yoff;
    u8 *sdf = stbtt_GetCodepointSDF(&fi, scale, c,
                                    CARBON_FONT__SDF_PADDING,
                                    CARBON_FONT__SDF_ONEDGE,
                                    CARBON_FONT__SDF_PXSCALE,
                                    &w, &h, &xoff, &yoff);
    if (!sdf) continue;
    if (pen_x + w > CARBON_FONT_DATA_SIZE) {
      pen_x = 0;
      pen_y += row_h;
      row_h = 0;
    }
    CBN_ASSERT(pen_y + h <= CARBON_FONT_DATA_SIZE && "atlas overflow");
    for (i32 row = 0; row < h; ++row) {
      carbon_memory_copy(f.data + (pen_y + row)*CARBON_FONT_DATA_SIZE + pen_x, sdf + row*w, w);
    }
    f.metadata.cdata[i].x0 = pen_x;
    f.metadata.cdata[i].x1 = pen_x + w;
    f.metadata.cdata[i].y0 = pen_y;
    f.metadata.cdata[i].y1 = pen_y + h;
    f.metadata.cdata[i].xoff = xoff;
    f.metadata.cdata[i].yoff = yoff;
    pen_x += w;
    if (h > row_h) row_h = h;
    stbtt_FreeSDF(sdf, 0);
  }
  carbon_list_destroy(&data);
  f.metadata.size = size;
  f.metadata.sdf_onedge = CARBON_FONT__SDF_ONEDGE;
  f.metadata.sdf_pxscale = CARBON_FONT__SDF_PXSCALE;
  f.metadata.sdf_padding = CARBON_FONT__SDF_PADDING;
  f.metadata.yoff_up = f.metadata.cdata['A' - CARBON_FONT_ASCII_START].yoff;
  for (char c = 'B'; c <= 'Z'; ++c) {
    usz i = c - CARBON_FONT_ASCII_START;
    f.metadata.yoff_up = carbon_math_min(f.metadata.yoff_up, f.metadata.cdata[i].yoff);
  }
  f.metadata.yoff_down = f.metadata.cdata[0].yoff + (f.metadata.cdata[0].y1 - f.metadata.cdata[0].y0);
  for (usz i = 1; i < CARBON_FONT_ASCII_COUNT; ++i) {
    f32 down = f.metadata.cdata[i].yoff + (f.metadata.cdata[i].y1 - f.metadata.cdata[i].y0);
    f.metadata.yoff_down = carbon_math_max(f.metadata.yoff_down, down);
  }
  return f;
}

f32 carbon_font_get_text_width(const CBN_Font *f, const char *txt, usz size) {
  const f32 sf = (0 < size && size <= f->metadata.size) ? (f32)size/(f32)f->metadata.size : 1;
  f32 width = 0;
  for (; *txt; ++txt) {
    usz idx = *txt - CARBON_FONT_ASCII_START;
    width += sf * f->metadata.cdata[idx].xadvance;
  }
  return width;
}

f32 carbon_font_get_text_height(const CBN_Font *f, usz size) {
  const f32 sf = (0 < size && size <= f->metadata.size) ? (f32)size/(f32)f->metadata.size : 1;
  return sf * (f->metadata.yoff_down - f->metadata.yoff_up);
}
