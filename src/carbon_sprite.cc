// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

const CBN_Sprite CBN_Sprite::make(const CBN_Image &img) {
  return carbon_sprite_from_img(&img);
}

void CBN_Sprite::Free(void) {
  carbon_sprite_destroy(this);
}
