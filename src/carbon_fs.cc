// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_PatternMatchedFiles::iterator CBN_PatternMatchedFiles::begin(void) const {
  return files;
}

CBN_PatternMatchedFiles::iterator CBN_PatternMatchedFiles::end(void) const {
  return files + count;
}

CBN_Image CBN_Image::make(const char *file) {
  return carbon_fs_read_img_from_file(file);
}

CBN_Image CBN_Image::make(const CBN_DrawCanvas &dc) {
  return (CBN_Image) {
    .data = carbon_fs_img_32bit_to_8bit(dc.pixels, dc.width, dc.height),
    .metadata = (CBN_Image_Metadata) {
      .width = dc.width,
      .height = dc.height,
      .channels = 4
    }
  };
}

void CBN_Image::Free(void) {
  carbon_fs_destroy_img(this);
}

u8 CBN_Image::Save(CBN_FileFormat fmt, const char *file) const {
  return carbon_fs_write_img_to_file(this, fmt, file);
}
