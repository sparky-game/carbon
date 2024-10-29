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
  if (attrs == INVALID_FILE_ATTRIBUTES) return false;
  return !(attrs & FILE_ATTRIBUTE_DIRECTORY);
#else
  struct stat sb = {0};
  if (-1 == stat(file, &sb)) return false;
  return S_ISREG(sb.st_mode);
#endif
}

u8 carbon_fs_is_directory(const char *file) {
#ifdef _WIN32
  DWORD attrs = GetFileAttributes(file);
  if (attrs == INVALID_FILE_ATTRIBUTES) return false;
  return attrs & FILE_ATTRIBUTE_DIRECTORY;
#else
  struct stat sb = {0};
  if (-1 == stat(file, &sb)) return false;
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

void carbon_fs_copy(const char *from, const char *to, u8 recursive) {
  // TODO: not yet implemented
  CARBON_NOTUSED(from);
  CARBON_NOTUSED(to);
  CARBON_NOTUSED(recursive);
}

u8 carbon_fs_remove(const char *file) {
  if (-1 == remove(file)) {
    CARBON_ERROR("carbon_fs_remove :: unable to remove file `%s`", file);
    return false;
  }
  return true;
}

u8 carbon_fs_remove_all(const char *file) {
  // TODO: not yet implemented
  CARBON_NOTUSED(file);
  return false;
}

u8 carbon_fs_change_directory(const char *path) {
#ifdef _WIN32
  i8 result = _chdir(path);
#else
  i8 result = chdir(path);
#endif
  if (result == -1) {
    CARBON_ERROR("carbon_fs_change_directory :: unable to change to directory `%s`", path);
    return false;
  }
  return true;
}

u8 carbon_fs_create_directory(const char *path) {
  if (!path || !path[0]) {
    CARBON_ERROR("carbon_fs_create_directory :: path is invalid");
    return false;
  }
  if (carbon_fs_is_directory(path)) {
    CARBON_WARNING("carbon_fs_create_directory :: directory already exists, skipping creation");
    return true;
  }
#ifdef _WIN32
  i8 result = _mkdir(path);
#else
  i8 result = mkdir(path, 0755);
#endif
  if (result == -1) {
    CARBON_ERROR("carbon_fs_create_directory :: unable to create directory `%s`", path);
    return false;
  }
  return true;
}

u8 carbon_fs_create_directories(const char *path) {
  if (!path || !path[0]) {
    CARBON_ERROR("carbon_fs_create_directories :: path is invalid");
    return false;
  }
  if (carbon_fs_is_directory(path)) {
    CARBON_WARNING("carbon_fs_create_directories :: directory already exists, skipping creation");
    return true;
  }
  usz len = strlen(path) + 1;
  char *pathcpy = CARBON_CALLOC(len, sizeof(char));
  memcpy(pathcpy, path, len);
  for (usz i = 0; i < len && pathcpy[i]; ++i) {
    if (pathcpy[i] == ':') ++i;
    else if (pathcpy[i] == '\\' || pathcpy[i] == '/') {
      pathcpy[i] = 0;
      carbon_fs_create_directory(pathcpy);
      pathcpy[i] = '/';
    }
  }
  carbon_fs_create_directory(pathcpy);
  CARBON_FREE(pathcpy);
  return true;
}

char *carbon_fs_get_bin_directory(void) {
  // TODO: not yet implemented
  return false;
}

// In Raylib we do: ChangeDirectory(GetApplicationDirectory())
// In Carbon we do: carbon_fs_change_directory(carbon_fs_get_bin_directory())
// --------------------------------------------------------------------------------
// if not -> CARBON_WARNING("unable to change CWD to the binary's directory");
