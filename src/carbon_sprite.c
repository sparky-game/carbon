// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Sprite carbon_sprite_from_img(const CBN_Image *img) {
  return (CBN_Sprite) {
    .pixels = carbon_fs_img_8bit_to_32bit(img->data, img->metadata.width, img->metadata.height),
    .width = img->metadata.width,
    .height = img->metadata.height,
  };
}

void carbon_sprite_destroy(CBN_Sprite *sprite) {
  carbon_memory_free((void *) sprite->pixels);
  carbon_memory_set(sprite, 0, sizeof(*sprite));
}
