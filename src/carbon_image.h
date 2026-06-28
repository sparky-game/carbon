/*
  $$===================$$
  ||       Image       ||
  $$===================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Types of image file formats.
 */
typedef enum {
  CBN_IMAGE_FORMAT_PNG,
  CBN_IMAGE_FORMAT_BMP,
  CBN_IMAGE_FORMAT_TGA,
  CBN_IMAGE_FORMAT_JPG,
} CBN_Image_Format;

/**
 * @brief Represents a drawable image object as a linear buffer of 32-bit RGBA pixels.
 */
CBNDEF_T(CBN_Image) {
  u32 *pixels;
  usz width;
  usz height;
  bool owned;
};
CBNDEF_TAKA(cbn, Image, CBN_Image);

// Backwards compatibility with Sprite
typedef CBN_Image CBN_Sprite;
CBNDEF_TAKA(cbn, Sprite, CBN_Sprite);

/**
 * @brief Reads an image from disk.
 * @param file The filename to read the image from.
 * @return The image object.
 */
CBNDEF CBN_Image carbon_image_read_from_file(const char *file);

/**
 * @brief ...
 * @param dc ...
 * @return The image object.
 */
CBNDEF CBN_Image carbon_image_view_from_canvas(const CBN_DrawCanvas *dc);

/**
 * @brief Destroy an image object.
 * @param img The image object.
 */
CBNDEF void carbon_image_destroy(CBN_Image *img);

/**
 * @brief Writes an image to disk.
 * @param img The image object.
 * @param fmt The format of the image file to write to.
 * @param file The filename to write the image to.
 * @return A boolean value representing the success of the operation.
 */
CBNDEF bool carbon_image_write_to_file(const CBN_Image *img, CBN_Image_Format fmt, const char *file);

//////////////////////////////////////////////////////////////////////

/* CBNDEF CBN_List carbon_image_tensorize(u8 *pixels, usz width, usz height, usz chs); */

/* CBNDEF u8 *carbon_image_linearize(CBN_List *img); */

/* CBNDEF CBN_List carbon_image_read_from_file_as_tensor(const char *file); */

/* CBNDEF bool carbon_image_write_tensor_to_file(CBN_List *img, CBN_Image_Format fmt, const char *file); */

/* CBNDEF void carbon_image_destroy_tensor(CBN_List *img); */
