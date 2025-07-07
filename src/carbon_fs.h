/*
**  $$========================$$
**  ||       Filesystem       ||
**  $$========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define carbon_fs_pattern_match_foreach(pmf) for (struct { usz i; char *f; } it = {0, (pmf).files[0]}; it.i < (pmf).count; ++it.i, it.i < (pmf).count ? it.f = (pmf).files[it.i] : it.f)

typedef enum {
  CBN_FILE_FORMAT_PNG,
  CBN_FILE_FORMAT_BMP,
  CBN_FILE_FORMAT_TGA,
  CBN_FILE_FORMAT_JPG,
} CBN_FileFormat;

typedef struct CBN_PatternMatchedFiles {
  char **files;
  usz count;
#ifdef __cplusplus
  using iterator = char **;
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void) const;
#endif
} CBN_PatternMatchedFiles;

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
typedef struct CBN_Image {
  u8 *data;
  CBN_Image_Metadata metadata;
#ifdef __cplusplus
  /**
   * @brief carbon_fs_read_img_from_file
   * @param file The filename to read the image from.
   * @return The image object.
   */
  static CBN_Image make(const char *file);
  /**
   * @brief carbon_fs_img_32bit_to_8bit
   * @return The image object.
   */
  static CBN_Image make(const CBN_DrawCanvas &dc);
  /**
   * @brief carbon_fs_destroy_img
   */
  void Free(void);
  /**
   * @brief carbon_fs_write_img_to_file
   * @param fmt The format of the image file to write to.
   * @param file The filename to write the image to.
   * @return A boolean value representing the success of the operation.
   */
  u8 Save(CBN_FileFormat fmt, const char *file) const;
#endif
} CBN_Image;

CARBON_API u8 carbon_fs_exists(const char *file);
CARBON_API u8 carbon_fs_is_regular_file(const char *file);
CARBON_API u8 carbon_fs_is_directory(const char *file);
CARBON_API u8 carbon_fs_rename(const char *oldie, const char *newie);
CARBON_API i32 carbon_fs_mtime(const char *file);
CARBON_API void carbon_fs_copy(const char *from, const char *to, u8 recursive);
CARBON_API u8 carbon_fs_remove(const char *file);
CARBON_API u8 carbon_fs_remove_all(const char *file);
CARBON_API u8 carbon_fs_change_directory(const char *path);
CARBON_API u8 carbon_fs_create_directory(const char *path);
CARBON_API u8 carbon_fs_create_directories(const char *path);
CARBON_API char *carbon_fs_get_curr_directory(void);
CARBON_API char *carbon_fs_get_bin_directory(void);
CARBON_API CBN_PatternMatchedFiles carbon_fs_pattern_match(const char *pattern);
CARBON_API u32 carbon_fs_get_file_size(const char *file);

/**
 * @brief Reads all the text data from the given file into a StrBuilder object.
 * @param sb The StrBuilder object where the read data will be stored.
 * @param file The filename to read the data from.
 * @return A boolean value representing the success of the operation.
 */
CARBON_API u8 carbon_fs_read_entire_file(CBN_StrBuilder *sb, const char *file);

/**
 * @brief Reads an image from disk.
 * @param file The filename to read the image from.
 * @return The image object.
 */
CARBON_API CBN_Image carbon_fs_read_img_from_file(const char *file);

CARBON_API CBN_List carbon_fs_read_img_from_file_as_tensor(const char *file);

/**
 * @brief Reads an image from disk as a linear buffer of 8-bit per channel RGBA bytes.
 * @param file The filename to read the image from.
 * @param out_width The width of the read image (output argument pointer).
 * @param out_height The height of the read image (output argument pointer).
 * @param out_chs The number of channels of the read image (output argument pointer).
 * @return The pointer to the newly allocated linear buffer of 8-bit per channel RGBA bytes.
 */
CARBON_API u8 *carbon_fs_read_img_from_file_linearly(const char *file, usz *out_width, usz *out_height, usz *out_chs);

CARBON_API CBN_List carbon_fs_img_tensorize(u8 *pixels, usz width, usz height, usz chs);

CARBON_API u8 *carbon_fs_img_linearize(CBN_List *img);

/**
 * @brief Converts a linear buffer of 32-bit RGBA pixels to a linear buffer of 8-bit per channel RGBA bytes.
 *
 * This function allocates memory in the heap using `CARBON_MALLOC`; thus, it's important to keep in mind the
 * lifetime of that allocation and free it accordingly using `CARBON_FREE`.
 *
 * @param pixels The pointer to the 32-bit RGBA linear buffer of pixels.
 * @param width The width of the image.
 * @param height The height of the image.
 * @return The pointer to the newly allocated linear buffer of 8-bit per channel RGBA bytes.
 */
CARBON_API u8 *carbon_fs_img_32bit_to_8bit(const u32 *pixels, const usz width, const usz height);

/**
 * @brief Writes an image to disk.
 * @param img The image object.
 * @param fmt The format of the image file to write to.
 * @param file The filename to write the image to.
 * @return A boolean value representing the success of the operation.
 */
CARBON_API u8 carbon_fs_write_img_to_file(const CBN_Image *img, CBN_FileFormat fmt, const char *file);

CARBON_API u8 carbon_fs_write_tensor_img_to_file(CBN_List *img, CBN_FileFormat fmt, const char *file);

CARBON_API u8 carbon_fs_write_img_to_file_linearly(u8 *pixels, CBN_FileFormat fmt, usz width, usz height, usz chs, const char *file);

/**
 * @brief Destroy an image object.
 * @param img The image object.
 */
CARBON_API void carbon_fs_destroy_img(CBN_Image *img);

CARBON_API void carbon_fs_destroy_tensor_img(CBN_List *img);

// Local Variables:
// mode: c++
// End:
