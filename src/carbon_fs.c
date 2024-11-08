// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef _WIN32
#define CARBON_FS_PATH_MAX_LEN 256
#else
#define CARBON_FS_PATH_MAX_LEN 4096
#endif
#define CARBON_FS_PATMAT_MAX_STRUCTS 4

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
  struct stat sb;
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
  struct stat sb;
  if (-1 == stat(file, &sb)) return false;
  return S_ISDIR(sb.st_mode);
#endif
}

u8 carbon_fs_rename(const char *oldie, const char *newie) {
  CARBON_INFO_COLOR(CARBON_COLOR_YELLOW, "[*] Renaming file %s -> %s", oldie, newie);
  if (-1 == rename(oldie, newie)) {
    CARBON_ERROR("unable to rename %s -> %s", oldie, newie);
    return false;
  }
  return true;
}

i32 carbon_fs_mtime(const char *file) {
  struct stat sb;
  if (-1 == stat(file, &sb)) {
    CARBON_ERROR("unable to stat file `%s`", file);
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
    CARBON_ERROR("unable to remove file `%s`", file);
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
    CARBON_ERROR("unable to change to directory `%s`", path);
    return false;
  }
  return true;
}

u8 carbon_fs_create_directory(const char *path) {
  if (!path || !path[0]) {
    CARBON_ERROR("path is invalid");
    return false;
  }
  if (carbon_fs_is_directory(path)) {
    CARBON_WARNING("directory already exists, skipping creation");
    return true;
  }
#ifdef _WIN32
  i8 result = _mkdir(path);
#else
  i8 result = mkdir(path, 0755);
#endif
  if (result == -1) {
    CARBON_ERROR("unable to create directory `%s`", path);
    return false;
  }
  return true;
}

u8 carbon_fs_create_directories(const char *path) {
  if (!path || !path[0]) {
    CARBON_ERROR("path is invalid");
    return false;
  }
  if (carbon_fs_is_directory(path)) {
    CARBON_WARNING("directory already exists, skipping creation");
    return true;
  }
  usz len = strlen(path) + 1;
  char *pathcpy = (char *) CARBON_CALLOC(len, sizeof(char));
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
  static char dir[CARBON_FS_PATH_MAX_LEN];
  memset(dir, 0, CARBON_FS_PATH_MAX_LEN);
#if defined(_WIN32)
  usz len = GetModuleFileNameA(0, dir, MAX_PATH);
  if (len > 0) {
    for (usz i = len;; --i) {
      if (dir[i] == '\\') {
        dir[i + 1] = 0;
        break;
      }
    }
  }
  else {
    dir[0] = '.';
    dir[1] = '\\';
  }
#elif defined(__linux__)
  usz len = readlink("/proc/self/exe", dir, sizeof(dir));
  if (len > 0) {
    for (usz i = len;; --i) {
      if (dir[i] == '/') {
        dir[i + 1] = 0;
        break;
      }
    }
  }
  else {
    dir[0] = '.';
    dir[1] = '/';
  }
#elif defined(__FreeBSD__)
  usz size = sizeof(dir);
  i32 mib[] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
  if (!sysctl(mib, 4, dir, &size, 0, 0)) {
    usz len = strlen(dir);
    for (usz i = len;; --i) {
      if (dir[i] == '/') {
        dir[i + 1] = 0;
        break;
      }
    }
  }
  else {
    dir[0] = '.';
    dir[1] = '/';
  }
#elif defined(__APPLE__)
  usz size = sizeof(dir);
  if (!_NSGetExecutablePath(dir, &size)) {
    usz len = strlen(dir);
    for (usz i = len;; --i) {
      if (dir[i] == '/') {
        dir[i + 1] = 0;
        break;
      }
    }
  }
  else {
    dir[0] = '.';
    dir[1] = '/';
  }
#endif
  return dir;
}

char **carbon_fs_pattern_match(const char *pattern, usz *out_count) {
  static usz i = 0;
#ifdef _WIN32
  static usz counts[CARBON_FS_PATMAT_MAX_STRUCTS];
  static char *results[CARBON_FS_PATMAT_MAX_STRUCTS][MAX_PATH];
  HANDLE h_find;
  WIN32_FIND_DATA find_data;
  for (usz j = 0; j < counts[i]; ++j) free(results[i][j]);
  counts[i] = 0;
  h_find = FindFirstFile(pattern, &find_data);
  if (h_find == INVALID_HANDLE_VALUE) {
    CARBON_ERROR("no found matches");
    *out_count = 0;
    return 0;
  }
  do {
    if (counts[i] < MAX_PATH) {
      results[i][counts[i]] = carbon_string_dup(find_data.cFileName);
      ++counts[i];
    }
    else {
      CARBON_ERROR("too many matches");
      break;
    }
  } while (FindNextFile(h_find, &find_data));
  FindClose(h_find);
  *out_count = counts[i];
  ++i;
  if (i >= CARBON_FS_PATMAT_MAX_STRUCTS) i = 0;
  return results[i];
#else
  static glob_t xs[CARBON_FS_PATMAT_MAX_STRUCTS];
  glob_t *x = &xs[i];
  memset(x, 0, sizeof(glob_t));
  switch (glob(pattern, GLOB_TILDE, 0, x)) {
  case GLOB_NOSPACE:
    CARBON_ERROR("out of memory");
    *out_count = 0;
    return 0;
  case GLOB_ABORTED:
    CARBON_ERROR("read error");
    *out_count = 0;
    return 0;
  case GLOB_NOMATCH:
    CARBON_ERROR("no found matches");
    *out_count = 0;
    return 0;
  }
  ++i;
  if (i >= CARBON_FS_PATMAT_MAX_STRUCTS) i = 0;
  *out_count = x->gl_pathc;
  return x->gl_pathv;
#endif
}
