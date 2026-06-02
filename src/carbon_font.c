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

CBN_Font carbon_font_create_from_file(const char *file, usz size) {
  CBN_Font f = {0};
  CBN_List data = carbon_list_create(sizeof(u8));
  CBN_ASSERT(carbon_fs_read_entire_file(&data, file));
  i32 n = stbtt_BakeFontBitmap((u8 *)data.items,
                               0,
                               size,
                               f.data,
                               CARBON_FONT_DATA_SIZE,
                               CARBON_FONT_DATA_SIZE,
                               CARBON_FONT_ASCII_START,
                               CARBON_FONT_ASCII_COUNT,
                               (stbtt_bakedchar *)f.metadata.cdata);
  CBN_ASSERT(n > 0);
  carbon_list_destroy(&data);
  f.metadata.size = size;
  f.metadata.yoff = f.metadata.cdata['A' - CARBON_FONT_ASCII_START].yoff;
  for (char c = 'B'; c <= 'Z'; ++c) {
    usz i = c - CARBON_FONT_ASCII_START;
    f.metadata.yoff = carbon_math_min(f.metadata.yoff, f.metadata.cdata[i].yoff);
  }
  return f;
}
