// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define STB_SPRINTF_IMPLEMENTATION
#include "../thirdparty/stb/stb_sprintf.h"

#define CARBON_STRING__FMT_MAX_LEN     1024
#define CARBON_STRING__FMT_MAX_BUFFERS 64

usz carbon_string_len(const char *s) {
  usz n = 0;
  while (*s++) ++n;
  return n;
}

i32 carbon_string_cmp(const char *s1, const char *s2) {
  for (; *s1 == *s2 && *s1; ++s1, ++s2);
  return *(u8 *)s1 - *(u8 *)s2;
}

i32 carbon_string_cmp_n(const char *s1, const char *s2, usz size) {
  const u8 *l = (u8 *) s1, *r = (u8 *) s2;
  if (!size--) return 0;
  for (; *l && *r && size && *l == *r; ++l, ++r, --size);
  return *l - *r;
}

char *carbon_string_dup(const char *s) {
  usz len = carbon_string_len(s) + 1;
  char *data = carbon_memory_alloc(len);
  return carbon_memory_copy(data, s, len);
}

void carbon_string_vsfmt(char *buf, usz n, const char *s, va_list args) {
  i32 bytes = stbsp_vsnprintf(buf, n, s, args);
  if (bytes >= (i32)n && n >= 4) {
    char *trunc = buf + n - 4;
    stbsp_snprintf(trunc, 4, "...");
  }
}

void carbon_string_sfmt(char *buf, usz n, const char *s, ...) {
  va_list args;
  va_start(args, s);
  carbon_string_vsfmt(buf, n, s, args);
  va_end(args);
}

char *carbon_string_vfmt(const char *s, va_list args) {
  static usz i = 0;
  static char xs[CARBON_STRING__FMT_MAX_BUFFERS][CARBON_STRING__FMT_MAX_LEN];
  char *x = xs[i++];
  if (i >= CARBON_STRING__FMT_MAX_BUFFERS) i = 0;
  carbon_string_vsfmt(x, CARBON_STRING__FMT_MAX_LEN, s, args);
  return x;
}

char *carbon_string_fmt(const char *s, ...) {
  va_list args;
  va_start(args, s);
  char *x = carbon_string_vfmt(s, args);
  va_end(args);
  return x;
}

bool carbon_string_has_char(const char *s, char c) {
  const char *c_ptr = carbon_string_get_char(s, c);
  if (!c_ptr) return false;
  return true;
}

char *carbon_string_get_char(const char *s, char c) {
  while (*s && *s != c) ++s;
  if (!c || *s == c) return (char *)s;
  return 0;
}

char *carbon_string_get_substr(const char *s, const char *sub) {
  const char *p = s;
  usz len = carbon_string_len(sub);
  if (!len) return (char *)s;
  for (; (p = carbon_string_get_char(p, *sub)) != 0; ++p) {
    if (!carbon_string_cmp_n(p, sub, len)) return (char *)p;
  }
  return 0;
}

void carbon_string_strip_substr(char *s, const char *sub) {
  usz len = carbon_string_len(sub);
  if (!len) return;
  char *p;
  while ((p = carbon_string_get_substr(s, sub))) {
    carbon_memory_copy(p, p + len, carbon_string_len(p + len) + 1);
  }
}

bool carbon_string_starts_with_substr(const char *s, const char *sub) {
  return carbon_string_cmp_n(s, sub, carbon_string_len(sub)) ? false : true;
}

bool carbon_string_ends_with_substr(const char *s, const char *sub) {
  usz s_len = carbon_string_len(s), sub_len = carbon_string_len(sub);
  return (s_len >= sub_len) && (!carbon_string_cmp(s + (s_len - sub_len), sub));
}

bool carbon_string_is_digit(char c) {
  return '0' <= c && c <= '9';
}

bool carbon_string_is_number(const char *s) {
  usz len = carbon_string_len(s);
  for (usz i = 0; i < len; ++i) {
    if (!carbon_string_is_digit(s[i])) return false;
  }
  return true;
}

i32 carbon_string_to_int(const char *s) {
  char *end;
  i32 ret = strtol(s, &end, 10);
  if (end == s) CBN_ERROR("unable to convert string to int (no digits found)");
  else if (*end != 0) CBN_ERROR("unable to convert string to int (invalid char `%c`)", *end);
  return ret;
}

f32 carbon_string_to_float(const char *s) {
  char *end;
  f32 ret = strtof(s, &end);
  if (end == s) CBN_ERROR("unable to convert string to float (no digits found)");
  else if (*end != 0) CBN_ERROR("unable to convert string to float (invalid char `%c`)", *end);
  return ret;
}

char carbon_string_to_upper(char c) {
  return 'a' <= c && c <= 'z' ? c - ('a' - 'A') : c;
}

usz carbon_string_lev_dist(const char *s1, const char *s2) {
  usz n = carbon_string_len(s1);
  usz m = carbon_string_len(s2);
  CBN_Mat dp = carbon_math_mat_create(n + 1, m + 1);
  for (usz i = 0; i <= n; ++i) CARBON_MAT_AT(dp, i, 0) = i;
  for (usz j = 0; j <= m; ++j) CARBON_MAT_AT(dp, 0, j) = j;
  for (usz i = 1; i <= n; ++i) {
    for (usz j = 1; j <= m; ++j) {
      if (s1[i - 1] == s2[j - 1]) CARBON_MAT_AT(dp, i, j) = CARBON_MAT_AT(dp, i - 1, j - 1);
      else CARBON_MAT_AT(dp, i, j) = 1 + carbon_math_min_3(CARBON_MAT_AT(dp, i - 1, j),
                                                           CARBON_MAT_AT(dp, i, j - 1),
                                                           CARBON_MAT_AT(dp, i - 1, j - 1));
    }
  }
  usz dist = CARBON_MAT_AT(dp, n, m);
  carbon_math_mat_destroy(&dp);
  return dist;
}
