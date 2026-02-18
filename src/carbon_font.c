#define STB_TRUETYPE_IMPLEMENTATION
#include "../thirdparty/stb_truetype/stb_truetype.h"

CBN_Font carbon_font_load(const char *file, usz size) {
  CBN_Font f = {0};
  CBN_StrBuilder sb = {0};
  CBN_ASSERT(carbon_fs_read_entire_file(&sb, file));
  i32 n = stbtt_BakeFontBitmap((u8 *)sb.items,
                               0,
                               size,
                               f.data,
                               CARBON_FONT_DATA_SIZE,
                               CARBON_FONT_DATA_SIZE,
                               CARBON_FONT_ASCII_START,
                               CARBON_FONT_ASCII_COUNT,
                               (stbtt_bakedchar *)f.metadata.cdata);
  CBN_ASSERT(n > 0);
  carbon_strbuilder_free(&sb);
  f.metadata.size = size;
  return f;
}
