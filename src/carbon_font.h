/*
  $$==================$$
  ||       Font       ||
  $$==================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_FONT_DATA_SIZE   512
#define CARBON_FONT_ASCII_START 32
#define CARBON_FONT_ASCII_COUNT 95

typedef struct {
  u16 x0, y0, x1, y1;
  f32 xoff, yoff;
  f32 xadvance;
} CBN_Font_Chardata;

typedef struct {
  usz size;
  CBN_Font_Chardata cdata[CARBON_FONT_ASCII_COUNT];
} CBN_Font_Metadata;

CBNDEF_T(CBN_Font) {
  CBN_Font_Metadata metadata;
  u8 data[CARBON_FONT_DATA_SIZE*CARBON_FONT_DATA_SIZE];
};
CBNDEF_AKA(cbn, Font, CBN_Font);

/**
 */
CBNDEF CBN_Font carbon_font_load(const char *file, usz size);
