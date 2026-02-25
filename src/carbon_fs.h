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

CBNDEF_T(CBN_PatternMatchedFiles) {
  char **files;
  usz count;
};

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
