// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define STB_TRUETYPE_IMPLEMENTATION
#include "../thirdparty/stb_truetype/stb_truetype.h"

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
  return f;
}
