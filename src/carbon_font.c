// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

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
