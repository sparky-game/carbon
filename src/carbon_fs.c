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
  if (!access(file, F_OK)) return 1;
  return 0;
#endif
}

u8 carbon_fs_rename(const char *old, const char *new) {
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Renaming file %s -> %s", old, new);
  if (-1 == rename(old, new)) {
    CARBON_ERROR("carbon_fs_rename :: unable to rename %s -> %s", old, new);
    return 0;
  }
  return 1;
}

i32 carbon_fs_mtime(const char *file) {
  struct stat sb = {0};
  if (-1 == stat(file, &sb)) {
    CARBON_ERROR("carbon_fs_mtime :: unable to stat file `%s`", file);
    return 0;
  }
  return sb.st_mtime;
}
