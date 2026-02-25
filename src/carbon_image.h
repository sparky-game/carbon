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
 * @brief Represents the needed metadata for an image object definition.
 */
typedef struct {
  usz width;
  usz height;
  usz channels;
} CBN_Image_Metadata;

/**
 * @brief Represents an image object as a linear buffer of 8-bit per channel RGBA bytes.
 */
CBNDEF_T(CBN_Image) {
  u8 *data;
  CBN_Image_Metadata metadata;
};
CBNDEF_AKA(cbn, Image, CBN_Image);

/**
 */
CBNDEF CBN_Image carbon_image_from_canvas(const CBN_DrawCanvas *dc);

/**
 * @brief Reads an image from disk.
 * @param file The filename to read the image from.
 * @return The image object.
 */
CBNDEF CBN_Image carbon_image_read_from_file(const char *file);

/**
 */
CBNDEF CBN_List carbon_image_read_from_file_as_tensor(const char *file);

/**
 * @brief Reads an image from disk as a linear buffer of 8-bit per channel RGBA bytes.
 * @param file The filename to read the image from.
 * @param out_width The width of the read image (output argument pointer).
 * @param out_height The height of the read image (output argument pointer).
 * @param out_chs The number of channels of the read image (output argument pointer).
 * @return The pointer to the newly allocated linear buffer of 8-bit per channel RGBA bytes.
 */
CBNDEF u8 *carbon_image_read_from_file_linearly(const char *file, usz *out_width, usz *out_height, usz *out_chs);

/**
 */
CBNDEF CBN_List carbon_image_tensorize(u8 *pixels, usz width, usz height, usz chs);

/**
 */
CBNDEF u8 *carbon_image_linearize(CBN_List *img);

/**
 * @brief Converts a linear buffer of 32-bit RGBA pixels to a linear buffer of 8-bit per channel RGBA bytes.
 *
 * This function allocates memory in the heap using `carbon_memory_alloc`; thus, it's important to keep in mind
 * the lifetime of that allocation and free it accordingly using `carbon_memory_free`.
 *
 * @param pixels The pointer to the 32-bit RGBA linear buffer of pixels.
 * @param width The width of the image.
 * @param height The height of the image.
 * @return The pointer to the newly allocated linear buffer of 8-bit per channel RGBA bytes.
 */
CBNDEF u8 *carbon_image_32bit_to_8bit(const u32 *pixels, const usz width, const usz height);

/**
 * @brief Converts a linear buffer of 8-bit per channel RGBA bytes to a linear buffer of 32-bit RGBA pixels.
 *
 * This function allocates memory in the heap using `carbon_memory_alloc`; thus, it's important to keep in mind
 * the lifetime of that allocation and free it accordingly using `carbon_memory_free`.
 *
 * @param bytes The pointer to the 8-bit per channel RGBA linear buffer of bytes.
 * @param width The width of the image.
 * @param height The height of the image.
 * @return The pointer to the newly allocated linear buffer of 32-bit RGBA pixels.
 */
CBNDEF u32 *carbon_image_8bit_to_32bit(const u8 *bytes, const usz width, const usz height);

/**
 * @brief Writes an image to disk.
 * @param img The image object.
 * @param fmt The format of the image file to write to.
 * @param file The filename to write the image to.
 * @return A boolean value representing the success of the operation.
 */
CBNDEF bool carbon_image_write_to_file(const CBN_Image *img, CBN_Image_Format fmt, const char *file);

/**
 */
CBNDEF bool carbon_image_write_tensor_to_file(CBN_List *img, CBN_Image_Format fmt, const char *file);

/**
 */
CBNDEF bool carbon_image_write_to_file_linearly(u8 *pixels, CBN_Image_Format fmt, usz width, usz height, usz chs, const char *file);

/**
 * @brief Destroy an image object.
 * @param img The image object.
 */
CBNDEF void carbon_image_destroy(CBN_Image *img);

/**
 */
CBNDEF void carbon_image_destroy_tensor(CBN_List *img);
