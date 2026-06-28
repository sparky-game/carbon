// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Image : CBN_Image_t {
  using Format = CBN_Image_Format;

  explicit CBN_Image(const char *file) : CBN_Image_t{carbon_image_read_from_file(file)} {}
  explicit CBN_Image(const CBN_DrawCanvas &dc) : CBN_Image_t{carbon_image_view_from_canvas(&dc)} {}

  ~CBN_Image(void) { carbon_image_destroy(this); }

  bool Save(Format fmt, const char *file) const {
    return carbon_image_write_to_file(this, fmt, file);
  }
};

#endif
