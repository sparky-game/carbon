// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Font : CBN_Font_t {
  static CBN_Font New(const char *file, usz size) {
    return carbon_font_create_from_file(file, size);
  }
};

#endif
