// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

u8 carbon_fs_exists(const char *file) {
#ifdef _WIN32
  DWORD attrs = GetFileAttributes(file);
  return attrs != INVALID_FILE_ATTRIBUTES;
#else
  if (!access(file, F_OK)) return true;
  return false;
#endif
}

u8 carbon_fs_is_regular_file(const char *file) {
#ifdef _WIN32
  DWORD attrs = GetFileAttributes(file);
  if (attrs == INVALID_FILE_ATTRIBUTES) {
    CARBON_ERROR("carbon_fs_is_regular_file :: unable to get attrs of file `%s`", file);
    return false;
  }
  return !(attrs & FILE_ATTRIBUTE_DIRECTORY);
#else
  struct stat sb = {0};
  if (-1 == stat(file, &sb)) {
    CARBON_ERROR("carbon_fs_is_regular_file :: unable to stat file `%s`", file);
    return false;
  }
  return S_ISREG(sb.st_mode);
#endif
}

u8 carbon_fs_is_directory(const char *file) {
#ifdef _WIN32
  DWORD attrs = GetFileAttributes(file);
  if (attrs == INVALID_FILE_ATTRIBUTES) {
    CARBON_ERROR("carbon_fs_is_directory :: unable to get attrs of file `%s`", file);
    return false;
  }
  return attrs & FILE_ATTRIBUTE_DIRECTORY;
#else
  struct stat sb = {0};
  if (-1 == stat(file, &sb)) {
    CARBON_ERROR("carbon_fs_is_directory :: unable to stat file `%s`", file);
    return false;
  }
  return S_ISDIR(sb.st_mode);
#endif
}

u8 carbon_fs_rename(const char *old, const char *new) {
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Renaming file %s -> %s", old, new);
  if (-1 == rename(old, new)) {
    CARBON_ERROR("carbon_fs_rename :: unable to rename %s -> %s", old, new);
    return false;
  }
  return true;
}

i32 carbon_fs_mtime(const char *file) {
  struct stat sb = {0};
  if (-1 == stat(file, &sb)) {
    CARBON_ERROR("carbon_fs_mtime :: unable to stat file `%s`", file);
    return false;
  }
  return sb.st_mtime;
}

u8 carbon_fs_remove(const char *file) {
  if (-1 == remove(file)) {
    CARBON_ERROR("carbon_fs_remove :: unable to remove file `%s`", file);
    return false;
  }
  return true;
}
