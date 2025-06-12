// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifdef __cplusplus

#include "../carbon.h.in"
#include "carbon_deps.h"
#include "carbon_defs.h"
#include "carbon_types.h"
#include "carbon_math.h"
#include "carbon_strview.h"
#include "carbon_strbuilder.h"
#include "carbon_list.h"
#include "carbon_drawcanvas.h"
#include "carbon_fs.h"

CBN_PatternMatchedFiles::iterator CBN_PatternMatchedFiles::begin(void) {
  return const_cast<iterator>(static_cast<const CBN_PatternMatchedFiles &>(*this).begin());
}

CBN_PatternMatchedFiles::const_iterator CBN_PatternMatchedFiles::begin(void) const {
  return files;
}

CBN_PatternMatchedFiles::iterator CBN_PatternMatchedFiles::end(void) {
  return const_cast<iterator>(static_cast<const CBN_PatternMatchedFiles &>(*this).end());
}

CBN_PatternMatchedFiles::const_iterator CBN_PatternMatchedFiles::end(void) const {
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

#endif  // __cplusplus
