// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Sprite : CBN_Sprite_t {
  explicit CBN_Sprite(const CBN_Image &img) : CBN_Sprite_t{carbon_sprite_from_img(&img)} {}

  ~CBN_Sprite(void) { carbon_sprite_destroy(this); }
};

#endif
