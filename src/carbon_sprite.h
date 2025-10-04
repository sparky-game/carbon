/*
**  $$====================$$
**  ||       Sprite       ||
**  $$====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef struct CBN_Image CBN_Image;  // Forward declaration

/**
 * @brief Represents a drawable image as a linear buffer of 32-bit RGBA pixels.
 */
typedef struct CBN_Sprite {
  const u32 *pixels;
  usz width;
  usz height;
#ifdef __cplusplus
  /**
   * @see carbon_sprite_from_img
   */
  static const CBN_Sprite make(const CBN_Image &img);
  /**
   * @see carbon_sprite_destroy
   */
  void Free(void);
#endif
} CBN_Sprite;

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

// Local Variables:
// mode: c++
// End:
