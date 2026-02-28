// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_FS__PATMAT_MAX_STRUCTS 4

bool carbon_fs_exists(const char *file) {
#ifdef _WIN32
  DWORD attrs = GetFileAttributes(file);
  return attrs != INVALID_FILE_ATTRIBUTES;
#else
  if (!access(file, F_OK)) return true;
  return false;
#endif
}

bool carbon_fs_is_regular_file(const char *file) {
  if (!carbon_fs_exists(file)) return false;
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

bool carbon_fs_is_directory(const char *file) {
  if (!carbon_fs_exists(file)) return false;
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

bool carbon_fs_rename(const char *oldie, const char *newie) {
#ifdef _WIN32
  if (carbon_fs_exists(newie) && !carbon_fs_remove(newie)) return false;
#endif
  if (-1 == rename(oldie, newie)) {
    CBN_ERROR("unable to rename `%s` -> `%s` (reason: `%s`)", oldie, newie, strerror(errno));
    return false;
  }
  return true;
}

i32 carbon_fs_mtime(const char *file) {
  struct stat sb;
  if (-1 == stat(file, &sb)) {
    CBN_ERROR("unable to stat file `%s`", file);
    return false;
  }
  return sb.st_mtime;
}

/* void carbon_fs_copy(const char *from, const char *to, bool recursive) { */
/*   // ... */
/* } */

bool carbon_fs_remove(const char *file) {
  if (-1 == remove(file)) {
    CBN_ERROR("unable to remove file `%s` (reason: `%s`)", file, strerror(errno));
    return false;
  }
  return true;
}

/* bool carbon_fs_remove_all(const char *file) { */
/*   // ... */
/* } */

bool carbon_fs_change_directory(const char *path) {
#ifdef _WIN32
  i8 result = _chdir(path);
#else
  i8 result = chdir(path);
#endif
  if (result == -1) {
    CBN_ERROR("unable to change to directory `%s`", path);
    return false;
  }
  return true;
}

bool carbon_fs_create_directory(const char *path) {
  if (!path || !path[0]) {
    CBN_ERROR("path is invalid");
    return false;
  }
  if (carbon_fs_is_directory(path)) return true;
#ifdef _WIN32
  i8 result = _mkdir(path);
#else
  i8 result = mkdir(path, 0755);
#endif
  if (result == -1) {
    CBN_ERROR("unable to create directory `%s`", path);
    return false;
  }
  return true;
}

bool carbon_fs_create_directories(const char *path) {
  if (!path || !path[0]) {
    CBN_ERROR("path is invalid");
    return false;
  }
  if (carbon_fs_is_directory(path)) return true;
  usz len = carbon_string_len(path) + 1;
  char *pathcpy = carbon_string_dup(path);
  for (usz i = 0; i < len && pathcpy[i]; ++i) {
    if (pathcpy[i] == ':') ++i;
    else if (pathcpy[i] == '\\' || pathcpy[i] == '/') {
      pathcpy[i] = 0;
      carbon_fs_create_directory(pathcpy);
      pathcpy[i] = '/';
    }
  }
  carbon_fs_create_directory(pathcpy);
  carbon_memory_free(pathcpy);
  return true;
}

char *carbon_fs_get_curr_directory(void) {
  static char dir[CARBON_FS_PATH_MAX_LEN];
  carbon_memory_set(dir, 0, CARBON_FS_PATH_MAX_LEN);
  char sep = '/';
#ifdef _WIN32
  char *path = _getcwd(dir, CARBON_FS_PATH_MAX_LEN - 1);
  sep = '\\';
#else
  char *path = getcwd(dir, CARBON_FS_PATH_MAX_LEN - 1);
#endif
  if (!path) {
    CBN_ERROR("unable to get the current directory");
    return 0;
  }
  usz len = carbon_string_len(path);
  if (len && len < CARBON_FS_PATH_MAX_LEN - 1 && path[len - 1] != sep) {
    path[len] = sep;
    path[len + 1] = 0;
  }
  return path;
}

char *carbon_fs_get_bin_directory(void) {
  static char dir[CARBON_FS_PATH_MAX_LEN];
  carbon_memory_set(dir, 0, CARBON_FS_PATH_MAX_LEN);
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
  extern isz readlink(const char *, char *, usz);
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
    usz len = carbon_string_len(dir);
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
  if (!_NSGetExecutablePath(dir, (u32 *) &size)) {
    usz len = carbon_string_len(dir);
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

char *carbon_fs_get_directory(const char *path) {
  static char dir[CARBON_FS_PATH_MAX_LEN];
  if (!path || !*path) return ".";
  usz i = carbon_string_len(path) - 1;
  for (; path[i] == '/'; i--) if (!i) return "/";
  for (; path[i] != '/'; i--) if (!i) return ".";
  for (; path[i] == '/'; i--) if (!i) return "/";
  carbon_memory_set(dir, 0, CARBON_FS_PATH_MAX_LEN);
  carbon_memory_copy(dir, path, i + 1);
  return dir;
}

CBN_PatternMatchedFiles carbon_fs_pattern_match(const char *pattern) {
  static usz i = 0;
  CBN_PatternMatchedFiles out;
  carbon_memory_set(&out, 0, sizeof(out));
#ifdef _WIN32
  static usz counts[CARBON_FS__PATMAT_MAX_STRUCTS];
  static char *results[CARBON_FS__PATMAT_MAX_STRUCTS][MAX_PATH];
  HANDLE h_find;
  WIN32_FIND_DATA find_data;
  for (usz j = 0; j < counts[i]; ++j) free(results[i][j]);
  counts[i] = 0;
  h_find = FindFirstFile(pattern, &find_data);
  if (h_find == INVALID_HANDLE_VALUE) {
    CBN_ERROR("no found matches");
    return out;
  }
  do {
    if (counts[i] < MAX_PATH) {
      results[i][counts[i]] = carbon_string_dup(find_data.cFileName);
      ++counts[i];
    }
    else {
      CBN_ERROR("too many matches");
      break;
    }
  } while (FindNextFile(h_find, &find_data));
  FindClose(h_find);
  out.count = counts[i];
  out.files = results[i];
  ++i;
  if (i >= CARBON_FS__PATMAT_MAX_STRUCTS) i = 0;
  return out;
#else
  static glob_t xs[CARBON_FS__PATMAT_MAX_STRUCTS];
  glob_t *x = &xs[i];
  carbon_memory_set(x, 0, sizeof(*x));
  switch (glob(pattern, GLOB_TILDE, 0, x)) {
  case GLOB_NOSPACE:
    CBN_ERROR("out of memory");
    return out;
  case GLOB_ABORTED:
    CBN_ERROR("read error");
    return out;
  case GLOB_NOMATCH:
    CBN_ERROR("no found matches");
    return out;
  }
  ++i;
  if (i >= CARBON_FS__PATMAT_MAX_STRUCTS) i = 0;
  out.count = x->gl_pathc;
  out.files = x->gl_pathv;
  return out;
#endif
}

u32 carbon_fs_get_file_size(const char *file) {
  if (!carbon_fs_is_regular_file(file)) {
    CBN_ERROR("file (`%s`) needs to be regular", file);
    return 0;
  }
  FILE *fd = fopen(file, "rb");
  if (!fd) {
    CBN_ERROR("unable to open file (`%s`)", file);
    return 0;
  }
  if (-1 == fseek(fd, 0, SEEK_END)) {
    CBN_ERROR("unable to set file's pointer to EOF (`%s`)", file);
    return 0;
  }
  i32 size = ftell(fd);
  if (size == -1) {
    CBN_ERROR("unable to get value of file's pointer (`%s`)", file);
    return 0;
  }
  fclose(fd);
  return size;
}

bool carbon_fs_read_entire_file(CBN_StrBuilder *sb, const char *file) {
  u32 n = carbon_fs_get_file_size(file);
  FILE *fd = fopen(file, "rb");
  if (!fd) {
    CBN_ERROR("unable to open file (`%s`)", file);
    return false;
  }
  usz count = sb->size + n;
  if (count > sb->capacity) {
    sb->items = (char *) carbon_memory_realloc(sb->items, count);
    sb->capacity = count;
  }
  if (1 != fread(sb->items + sb->size, n, 1, fd)) {
    CBN_ERROR("failed to read 1 item of %uB (`%s`)", n, file);
    return false;
  }
  if (ferror(fd)) {
    CBN_ERROR("unable to read file's contents (`%s`)", file);
    return false;
  }
  sb->size = count;
  fclose(fd);
  return true;
}

bool carbon_fs_write_entire_file(const CBN_StrBuilder *sb, const char *file) {
  FILE *fd = fopen(file, "wb");
  if (!fd) {
    CBN_ERROR("unable to open file (`%s`)", file);
    return false;
  }
  if (1 != fwrite(sb->items, sb->size, 1, fd)) {
    CBN_ERROR("failed to write 1 item of %zuB (`%s`)", sb->size, file);
    return false;
  }
  fclose(fd);
  return true;
}
