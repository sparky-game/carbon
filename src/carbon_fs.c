// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define STBI_MALLOC  CARBON_MALLOC
#define STBI_REALLOC CARBON_REALLOC
#define STBI_FREE    CARBON_FREE
#define STBI_ASSERT  CARBON_ASSERT
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image/stb_image.h"

#define STBIW_MALLOC  CARBON_MALLOC
#define STBIW_REALLOC CARBON_REALLOC
#define STBIW_FREE    CARBON_FREE
#define STBIW_ASSERT  CARBON_ASSERT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb_image_write/stb_image_write.h"

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
    // NOTE: is this warning useful or is it kinda lame?
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
  if (!_NSGetExecutablePath(dir, (u32 *) &size)) {
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

CBN_PatternMatchedFiles carbon_fs_pattern_match(const char *pattern) {
  static usz i = 0;
  CBN_PatternMatchedFiles out;
  memset(&out, 0, sizeof(CBN_PatternMatchedFiles));
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
    return out;
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
  out.count = counts[i];
  ++i;
  if (i >= CARBON_FS_PATMAT_MAX_STRUCTS) i = 0;
  out.files = results[i];
  return out;
#else
  static glob_t xs[CARBON_FS_PATMAT_MAX_STRUCTS];
  glob_t *x = &xs[i];
  memset(x, 0, sizeof(glob_t));
  switch (glob(pattern, GLOB_TILDE, 0, x)) {
  case GLOB_NOSPACE:
    CARBON_ERROR("out of memory");
    return out;
  case GLOB_ABORTED:
    CARBON_ERROR("read error");
    return out;
  case GLOB_NOMATCH:
    CARBON_ERROR("no found matches");
    return out;
  }
  ++i;
  if (i >= CARBON_FS_PATMAT_MAX_STRUCTS) i = 0;
  out.count = x->gl_pathc;
  out.files = x->gl_pathv;
  return out;
#endif
}

u32 carbon_fs_get_file_size(const char *file) {
  FILE *fd = fopen(file, "rb");
  if (!fd) {
    CARBON_ERROR("unable to open file (`%s`)", file);
    return 0;
  }
  if (-1 == fseek(fd, 0, SEEK_END)) {
    CARBON_ERROR("unable to set file's pointer to EOF (`%s`)", file);
    return 0;
  }
  i32 size = ftell(fd);
  if (size == -1) {
    CARBON_ERROR("unable to get value of file's pointer (`%s`)", file);
    return 0;
  }
  fclose(fd);
  return size;
}

u8 carbon_fs_read_entire_file(CBN_StrBuilder *sb, const char *file) {
  u32 n = carbon_fs_get_file_size(file);
  FILE *fd = fopen(file, "rb");
  if (!fd) {
    CARBON_ERROR("unable to open file (`%s`)", file);
    return false;
  }
  usz count = sb->size + n;
  if (count > sb->capacity) {
    char *prev_p = sb->items;
    sb->items = (char *) CARBON_REALLOC(sb->items, count);
    if (!sb->items && sb->size > 0) {
      CARBON_ERROR("failed to reallocate memory (%zuB)", count);
      CARBON_FREE(prev_p);
      return false;
    }
    sb->capacity = count;
  }
  if (1 != fread(sb->items + sb->size, n, 1, fd)) {
    CARBON_ERROR("failed to read 1 item of %uB (`%s`)", n, file);
    return false;
  }
  if (ferror(fd)) {
    CARBON_ERROR("unable to read file's contents (`%s`)", file);
    return false;
  }
  sb->size = count;
  fclose(fd);
  return true;
}

CBN_List carbon_fs_read_img_from_file(const char *file) {
  usz width = 0, height = 0, chs = 0;
  u8 *pixels = carbon_fs_read_img_from_file_linearly(file, &width, &height, &chs);
  CBN_List img = carbon_fs_img_tensorize(pixels, width, height, chs);
  CARBON_FREE(pixels);
  return img;
}

u8 *carbon_fs_read_img_from_file_linearly(const char *file, usz *out_width, usz *out_height, usz *out_chs) {
  return stbi_load(file, (i32 *) out_width, (i32 *) out_height, (i32 *) out_chs, 0);
}

CBN_List carbon_fs_img_tensorize(u8 *pixels, usz width, usz height, usz chs) {
  CBN_List img = carbon_list_create(sizeof(CBN_Matrix));
  for (usz c = 0; c < chs; ++c) {
    u8 *ptr = pixels + c;
    CBN_Matrix ch = carbon_math_mat_create(height, width);
    for (usz i = 0; i < ch.rows; ++i) {
      for (usz j = 0; j < ch.cols; ++j) {
        CARBON_MAT_AT(ch, i, j) = *ptr;
        ptr += chs;
      }
    }
    carbon_list_push(&img, &ch);
  }
  return img;
}

u8 *carbon_fs_img_linearize(CBN_List *img) {
  CBN_Matrix first_ch = carbon_list_at(CBN_Matrix, *img, 0);
  usz n_pixels = first_ch.rows * first_ch.cols;
  u8 *pixels = (u8 *) CARBON_MALLOC(img->size * n_pixels * sizeof(u8));
  usz i = 0, j = 0;
  for (usz k = 0; k < n_pixels; ++k) {
    carbon_list_foreach(CBN_Matrix, *img) {
      pixels[(k * img->size) + it.i] = (u8) CARBON_MAT_AT(it.var, i, j);
    }
    ++j;
    if (j == first_ch.cols) j = 0, ++i;
  }
  return pixels;
}

u8 carbon_fs_write_img_to_file(CBN_List *img, CBN_FileFormat fmt, const char *file) {
  CBN_Matrix first_ch = carbon_list_at(CBN_Matrix, *img, 0);
  u8 *pixels = carbon_fs_img_linearize(img);
  u8 result = carbon_fs_write_img_to_file_linearly(pixels, fmt, first_ch.cols, first_ch.rows, img->size, file);
  CARBON_FREE(pixels);
  return result;
}

u8 carbon_fs_write_img_to_file_linearly(u8 *pixels, CBN_FileFormat fmt, usz width, usz height, usz chs, const char *file) {
  i32 result = 0;
  switch (fmt) {
  case CBN_FILE_FORMAT_PNG:
    result = stbi_write_png(file, width, height, chs, pixels, width * chs);
    break;
  case CBN_FILE_FORMAT_BMP:
    result = stbi_write_bmp(file, width, height, chs, pixels);
    break;
  case CBN_FILE_FORMAT_TGA:
    result = stbi_write_tga(file, width, height, chs, pixels);
    break;
  case CBN_FILE_FORMAT_JPG:
    // TODO: customize the quality value [1, 100] with a macro (maybe `CARBON_FS_WRITE_IMG_JPG_QUALITY`)
    result = stbi_write_jpg(file, width, height, chs, pixels, 90);
    break;
  default:
    CARBON_ASSERT(0 && "unreachable");
  }
  if (!result) CARBON_ERROR("unable to write pixels to file (`%s`)", file);
  // NOTE: maybe just `return result;` is fine?
  return result ? true : false;
}

void carbon_fs_destroy_img(CBN_List *img) {
  carbon_list_foreach(CBN_Matrix, *img) {
    carbon_math_mat_destroy(&it.var);
  }
  carbon_list_destroy(img);
}
