// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#include <sys/stat.h>

u8 carbon_fs_rename(const char *old, const char *new) {
  CARBON_INFO(CARBON_COLOR_YELLOW "[*] Renaming file %s -> %s" CARBON_COLOR_RESET "\n", old, new);
  if (-1 == rename(old, new)) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_fs_rename :: unable to rename %s -> %s" CARBON_COLOR_RESET "\n", old, new);
    return 0;
  }
  return 1;
}

i32 carbon_fs_mtime(const char *file) {
  struct stat sb = {0};
  if (-1 == stat(file, &sb)) {
    CARBON_ERROR("[ERROR]: " CARBON_COLOR_RED "carbon_fs_mtime :: unable to stat file `%s`" CARBON_COLOR_RESET "\n", file);
    return 0;
  }
  return sb.st_mtime;
}
