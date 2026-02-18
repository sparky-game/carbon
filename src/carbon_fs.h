/*
  $$========================$$
  ||       Filesystem       ||
  $$========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifdef _WIN32
#define CARBON_FS_PATH_MAX_LEN 256
#else
#define CARBON_FS_PATH_MAX_LEN 4096
#endif

#define carbon_fs_pattern_match_foreach(pmf) for (struct { usz i; char *f; } it = {0, (pmf).files[0]}; it.i < (pmf).count; ++it.i, it.i < (pmf).count ? it.f = (pmf).files[it.i] : it.f)

typedef enum {
  CBN_FILE_FORMAT_PNG,
  CBN_FILE_FORMAT_BMP,
  CBN_FILE_FORMAT_TGA,
  CBN_FILE_FORMAT_JPG,
} CBN_FileFormat;

CBNDEF_T(CBN_PatternMatchedFiles) {
  char **files;
  usz count;
};

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
 * @brief ...
 * @param file ...
 * @return ...
 */
CBNDEF bool carbon_fs_exists(const char *file);
CBNDEF_FN(cbn::fs, Exists, carbon_fs_exists);

/**
 * @brief ...
 * @param file ...
 * @return ...
 */
CBNDEF bool carbon_fs_is_regular_file(const char *file);
CBNDEF_FN(cbn::fs, IsFile, carbon_fs_is_regular_file);

/**
 * @brief ...
 * @param file ...
 * @return ...
 */
CBNDEF bool carbon_fs_is_directory(const char *file);
CBNDEF_FN(cbn::fs, IsDir, carbon_fs_is_directory);

/**
 * @brief ...
 * @param oldie ...
 * @param newie ...
 * @return ...
 */
CBNDEF bool carbon_fs_rename(const char *oldie, const char *newie);
CBNDEF_FN(cbn::fs, Rename, carbon_fs_rename);
CBNDEF_FN(cbn::fs, mv, Rename);

/**
 * @brief ...
 * @param file ...
 * @return ...
 */
CBNDEF i32 carbon_fs_mtime(const char *file);
CBNDEF_FN(cbn::fs, LastMod, carbon_fs_mtime);

// CBNDEF void carbon_fs_copy(const char *from, const char *to, bool recursive);

/**
 * @brief ...
 * @param file ...
 * @return ...
 */
CBNDEF bool carbon_fs_remove(const char *file);
CBNDEF_FN(cbn::fs, Remove, carbon_fs_remove);
CBNDEF_FN(cbn::fs, rm, Remove);

// CBNDEF bool carbon_fs_remove_all(const char *file);

/**
 * @brief ...
 * @param path ...
 * @return ...
 */
CBNDEF bool carbon_fs_change_directory(const char *path);
CBNDEF_FN(cbn::fs, ChangeDir, carbon_fs_change_directory);
CBNDEF_FN(cbn::fs, cd, ChangeDir);

/**
 * @brief ...
 * @param path ...
 * @return ...
 */
CBNDEF bool carbon_fs_create_directory(const char *path);
CBNDEF_FN(cbn::fs, CreateDir, carbon_fs_create_directory);
CBNDEF_FN(cbn::fs, mkdir, CreateDir);

/**
 * @brief ...
 * @param path ...
 * @return ...
 */
CBNDEF bool carbon_fs_create_directories(const char *path);
CBNDEF_FN(cbn::fs, CreateDirs, carbon_fs_create_directories);
CBNDEF_FN(cbn::fs, mkdir_p, CreateDirs);

/**
 * @brief ...
 * @return ...
 */
CBNDEF char *carbon_fs_get_curr_directory(void);
CBNDEF_FN(cbn::fs, GetCurrDir, carbon_fs_get_curr_directory);
CBNDEF_FN(cbn::fs, cwd, GetCurrDir);

/**
 * @brief ...
 * @return ...
 */
CBNDEF char *carbon_fs_get_bin_directory(void);
CBNDEF_FN(cbn::fs, GetBinDir, carbon_fs_get_bin_directory);

/**
 * @brief ...
 * @param path ...
 * @return ...
 */
CBNDEF char *carbon_fs_get_directory(const char *path);
CBNDEF_FN(cbn::fs, GetDir, carbon_fs_get_directory);
CBNDEF_FN(cbn::fs, dirname, GetDir);

CBNDEF CBN_PatternMatchedFiles carbon_fs_pattern_match(const char *pattern);

/**
 * @brief ...
 * @param path ...
 * @return ...
 */
CBNDEF u32 carbon_fs_get_file_size(const char *file);
CBNDEF_FN(cbn::fs, Size, carbon_fs_get_file_size);

/**
 * @brief Reads all the data from the given file into a StrBuilder object.
 * @param sb The StrBuilder object where the read data will be stored.
 * @param file The filename to read the data from.
 * @return A boolean value representing the success of the operation.
 */
CBNDEF bool carbon_fs_read_entire_file(CBN_StrBuilder *sb, const char *file);
CBNDEF_FN(cbn::fs, ReadFile, carbon_fs_read_entire_file);

/**
 */
CBNDEF bool carbon_fs_write_entire_file(const CBN_StrBuilder *sb, const char *file);
CBNDEF_FN(cbn::fs, WriteFile, carbon_fs_write_entire_file);

/**
 * @brief Reads an image from disk.
 * @param file The filename to read the image from.
 * @return The image object.
 */
CBNDEF CBN_Image carbon_fs_read_img_from_file(const char *file);

CBNDEF CBN_List carbon_fs_read_img_from_file_as_tensor(const char *file);

/**
 * @brief Reads an image from disk as a linear buffer of 8-bit per channel RGBA bytes.
 * @param file The filename to read the image from.
 * @param out_width The width of the read image (output argument pointer).
 * @param out_height The height of the read image (output argument pointer).
 * @param out_chs The number of channels of the read image (output argument pointer).
 * @return The pointer to the newly allocated linear buffer of 8-bit per channel RGBA bytes.
 */
CBNDEF u8 *carbon_fs_read_img_from_file_linearly(const char *file, usz *out_width, usz *out_height, usz *out_chs);

CBNDEF CBN_List carbon_fs_img_tensorize(u8 *pixels, usz width, usz height, usz chs);

CBNDEF u8 *carbon_fs_img_linearize(CBN_List *img);

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
CBNDEF u8 *carbon_fs_img_32bit_to_8bit(const u32 *pixels, const usz width, const usz height);

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
CBNDEF u32 *carbon_fs_img_8bit_to_32bit(const u8 *bytes, const usz width, const usz height);

/**
 * @brief Writes an image to disk.
 * @param img The image object.
 * @param fmt The format of the image file to write to.
 * @param file The filename to write the image to.
 * @return A boolean value representing the success of the operation.
 */
CBNDEF u8 carbon_fs_write_img_to_file(const CBN_Image *img, CBN_FileFormat fmt, const char *file);

CBNDEF u8 carbon_fs_write_tensor_img_to_file(CBN_List *img, CBN_FileFormat fmt, const char *file);

CBNDEF u8 carbon_fs_write_img_to_file_linearly(u8 *pixels, CBN_FileFormat fmt, usz width, usz height, usz chs, const char *file);

/**
 * @brief Destroy an image object.
 * @param img The image object.
 */
CBNDEF void carbon_fs_destroy_img(CBN_Image *img);

CBNDEF void carbon_fs_destroy_tensor_img(CBN_List *img);
