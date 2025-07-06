// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_STRING_FMT_MAX_LEN     1024
#define CARBON_STRING_FMT_MAX_BUFFERS 64

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
  char *data = (char *) CARBON_MALLOC(len);
  CARBON_ASSERT(data && "failed to allocate memory");
  return (char *) carbon_memory_copy(data, s, len);
}

char *carbon_string_fmt(const char *s, ...) {
  static usz i = 0;
  static char xs[CARBON_STRING_FMT_MAX_BUFFERS][CARBON_STRING_FMT_MAX_LEN];
  char *x = xs[i];
  carbon_memory_set(x, 0, CARBON_STRING_FMT_MAX_LEN);
  va_list args;
  va_start(args, s);
  i32 bytes = vsnprintf(x, CARBON_STRING_FMT_MAX_LEN, s, args);
  va_end(args);
  if (bytes >= CARBON_STRING_FMT_MAX_LEN) {
    char *trunc_x = xs[i] + CARBON_STRING_FMT_MAX_LEN - 4;
    snprintf(trunc_x, 4, "...");
  }
  ++i;
  if (i >= CARBON_STRING_FMT_MAX_BUFFERS) i = 0;
  return x;
}

void carbon_string_strip_substr(char *s, const char *sub) {
  usz len = carbon_string_len(sub);
  if (!len) return;
  char *p;
  while ((p = strstr(s, sub))) memmove(p, p + len, carbon_string_len(p + len) + 1);
}

u8 carbon_string_starts_with_substr(const char *s, const char *sub) {
  return carbon_string_cmp_n(s, sub, carbon_string_len(sub)) ? false : true;
}

u8 carbon_string_ends_with_substr(const char *s, const char *sub) {
  usz s_len = carbon_string_len(s), sub_len = carbon_string_len(sub);
  return (s_len >= sub_len) && (!carbon_string_cmp(s + (s_len - sub_len), sub));
}

u8 carbon_string_is_number(const char *s) {
  usz len = carbon_string_len(s);
  for (usz i = 0; i < len; ++i) {
    if (!isdigit(s[i])) return false;
  }
  return true;
}

u8 carbon_string_has_char(const char *s, char c) {
  const char *c_ptr = strchr(s, c);
  if (!c_ptr) return false;
  return true;
}

usz carbon_string_lev_dist(const char *s1, const char *s2) {
  usz n = carbon_string_len(s1);
  usz m = carbon_string_len(s2);
  CBN_Matrix dp = carbon_math_mat_create(n + 1, m + 1);
  for (usz i = 0; i <= n; ++i) CARBON_MAT_AT(dp, i, 0) = i;
  for (usz j = 0; j <= m; ++j) CARBON_MAT_AT(dp, 0, j) = j;
  for (usz i = 1; i <= n; ++i) {
    for (usz j = 1; j <= m; ++j) {
      if (s1[i - 1] == s2[j - 1]) CARBON_MAT_AT(dp, i, j) = CARBON_MAT_AT(dp, i - 1, j - 1);
      else CARBON_MAT_AT(dp, i, j) = 1 + CARBON_MIN3(CARBON_MAT_AT(dp, i - 1, j),
                                                     CARBON_MAT_AT(dp, i, j - 1),
                                                     CARBON_MAT_AT(dp, i - 1, j - 1));
    }
  }
  usz dist = CARBON_MAT_AT(dp, n, m);
  carbon_math_mat_destroy(&dp);
  return dist;
}
