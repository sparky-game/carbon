/*
  $$====================$$
  ||       Sprite       ||
  $$====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a drawable image as a linear buffer of 32-bit RGBA pixels.
 */
CBNDEF_T(CBN_Sprite) {
  u32 *pixels;
  usz width;
  usz height;
};
CBNDEF_AKA(cbn, Sprite, CBN_Sprite);

/**
 * @brief Creates a Sprite from an Image object.
 * @param img The Image object.
 * @return The newly allocated Sprite object.
 */
CBNDEF CBN_Sprite carbon_sprite_from_img(const CBN_Image *img);

/**
 * @brief Destroys a Sprite object.
 * @param sprite The Sprite object.
 */
CBNDEF void carbon_sprite_destroy(CBN_Sprite *sprite);
