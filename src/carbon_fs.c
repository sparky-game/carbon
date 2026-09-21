// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

bool carbon_fs_exists(const char *file) {
#ifdef CARBON_TARGET_OS_WINDOWS
  DWORD attrs = GetFileAttributes(file);
  return attrs != INVALID_FILE_ATTRIBUTES;
#else
  if (!access(file, F_OK)) return true;
  return false;
#endif
}

bool carbon_fs_is_regular_file(const char *file) {
  if (!carbon_fs_exists(file)) return false;
#ifdef CARBON_TARGET_OS_WINDOWS
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
#ifdef CARBON_TARGET_OS_WINDOWS
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
#ifdef CARBON_TARGET_OS_WINDOWS
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
#ifdef CARBON_TARGET_OS_WINDOWS
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
#ifdef CARBON_TARGET_OS_WINDOWS
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
#ifdef CARBON_TARGET_OS_WINDOWS
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
#if defined(CARBON_TARGET_OS_WINDOWS)
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
#elif defined(CARBON_TARGET_OS_LINUX)
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
#elif defined(CARBON_TARGET_OS_FREEBSD)
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
#elif defined(CARBON_TARGET_OS_MACOS)
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

bool carbon_fs_read_entire_file(CBN_List *l, const char *file) {
  u32 n = carbon_fs_get_file_size(file);
  FILE *fd = fopen(file, "rb");
  if (!fd) {
    CBN_ERROR("unable to open file (`%s`)", file);
    return false;
  }
  usz count = l->size + n;
  if (count > l->capacity) {
    l->items = carbon_memory_realloc(l->items, count);
    l->capacity = count;
  }
  if (1 != fread(l->items + l->size, n, 1, fd)) {
    CBN_ERROR("failed to read 1 item of %uB (`%s`)", n, file);
    fclose(fd);
    return false;
  }
  if (ferror(fd)) {
    CBN_ERROR("unable to read file's contents (`%s`)", file);
    fclose(fd);
    return false;
  }
  l->size = count;
  fclose(fd);
  return true;
}

bool carbon_fs_write_entire_file(const CBN_List *l, const char *file) {
  FILE *fd = fopen(file, "wb");
  if (!fd) {
    CBN_ERROR("unable to open file (`%s`)", file);
    return false;
  }
  if (1 != fwrite(l->items, l->size, 1, fd)) {
    CBN_ERROR("failed to write 1 item of %zuB (`%s`)", l->size, file);
    return false;
  }
  fclose(fd);
  return true;
}

bool carbon_fs_glob(const char *pattern, CBN_FS_WalkFunc f, void *arg) {
#ifdef CARBON_TARGET_OS_WINDOWS
  usz pat_len = carbon_string_len(pattern);
  usz dir_len = 0;
  for (usz i = pat_len; i > 0; --i) {
    if (pattern[i - 1] == '\\' || pattern[i - 1] == '/') {
      dir_len = i;
      break;
    }
  }
  if (dir_len >= CARBON_FS_PATH_MAX_LEN) {
    CBN_ERROR("pattern too long (`%s`)", pattern);
    return false;
  }
  char buf[CARBON_FS_PATH_MAX_LEN] = {0};
  carbon_memory_copy(buf, pattern, dir_len);
  WIN32_FIND_DATA fd;
  HANDLE h = FindFirstFile(pattern, &fd);
  if (h == INVALID_HANDLE_VALUE) return true;
  do {
    if (!carbon_string_cmp(fd.cFileName, ".") || !carbon_string_cmp(fd.cFileName, "..")) continue;
    usz len = carbon_string_len(fd.cFileName);
    if (dir_len + len + 1 > CARBON_FS_PATH_MAX_LEN) {
      CBN_WARN("skipping path, too long (`%s%s`)", buf, fd.cFileName);
      continue;
    }
    carbon_memory_copy(buf + dir_len, fd.cFileName, len + 1);
    const CBN_FS_WalkEntry e = {
      .path = buf,
      .name = buf + dir_len,
      .is_dir = carbon_fs_is_directory(buf),
      .depth = 0,
      .arg = arg
    };
    if (f(&e) == CARBON_FS_WALK_STOP) break;
  } while (FindNextFile(h, &fd));
#else
  glob_t g = {0};
  switch (glob(pattern, GLOB_TILDE, 0, &g)) {
  case 0: break;
  case GLOB_NOMATCH:
    globfree(&g);
    return true;
  case GLOB_NOSPACE:
    CBN_ERROR("out of memory");
    globfree(&g);
    return false;
  case GLOB_ABORTED:
    CBN_ERROR("read error");
    globfree(&g);
    return false;
  }
  for (usz i = 0; i < g.gl_pathc; ++i) {
    const char *path = g.gl_pathv[i];
    const char *name = path;
    for (const char *p = path; *p; ++p) {
      if (*p == '/') name = p + 1;
    }
    const CBN_FS_WalkEntry e = {
      .path = path,
      .name = name,
      .is_dir = carbon_fs_is_directory(g.gl_pathv[i]),
      .depth = 0,
      .arg = arg
    };
    if (f(&e) == CARBON_FS_WALK_STOP) break;
  }
  globfree(&g);
  return true;
#endif
}
