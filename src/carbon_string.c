// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define ONES ((usz) - 1 / 255)
#define HIGHS (ONES * (255 / 2 + 1))
#define HASZERO(x) ((x) - ONES & ~(x) & HIGHS)

#define CARBON_STRING_FMT_MAX_LEN     1024
#define CARBON_STRING_FMT_MAX_BUFFERS 4

i32 carbon_string_cmp(const char *s1, const char *s2) {
  for (; *s1 == *s2 && *s1; ++s1, ++s2);
  return *(u8 *)s1 - *(u8 *)s2;
}

char *carbon_string_dup(const char *s) {
  usz len = strlen(s);
  char *data = CARBON_MALLOC(len + 1);
  if (!data) {
    CARBON_ERROR("carbon_string_dup :: failed to allocate memory (%zuB)", len + 1);
    return false;
  }
  return memcpy(data, s, len + 1);
}

char *carbon_string_fmt(const char *s, ...) {
  static i32 i = 0;
  static char xs[CARBON_STRING_FMT_MAX_BUFFERS][CARBON_STRING_FMT_MAX_LEN] = {0};
  char *x = xs[i];
  memset(x, 0, CARBON_STRING_FMT_MAX_LEN);
  va_list args;
  va_start(args, s);
  i32 bytes = vsnprintf(x, CARBON_STRING_FMT_MAX_LEN, s, args);
  va_end(args);
  if (bytes >= CARBON_STRING_FMT_MAX_LEN) {
    char *trunc_x = xs[i] + CARBON_STRING_FMT_MAX_LEN - 4;
    sprintf(trunc_x, "...");
  }
  ++i;
  if (i >= CARBON_STRING_FMT_MAX_BUFFERS) i = 0;
  return x;
}

void carbon_string_strip_substr(char *s, const char *sub) {
  usz len = strlen(sub);
  if (!len) return;
  char *p;
  while ((p = strstr(s, sub))) {
    memmove(p, p + len, strlen(p + len) + 1);
  }
}
