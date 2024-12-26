// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define CARBON_STRVIEW_TO_CSTR_MAX_LEN     1024
#define CARBON_STRVIEW_TO_CSTR_MAX_BUFFERS 4

CBN_StrView carbon_strview_from_buf(const char *data, usz size) {
  return (CBN_StrView) {
    .data = data,
    .size = size
  };
}

CBN_StrView carbon_strview_from_cstr(const char *s) {
  return carbon_strview_from_buf(s, strlen(s));
}

char *carbon_strview_to_cstr(CBN_StrView sv) {
  static usz i = 0;
  static char xs[CARBON_STRVIEW_TO_CSTR_MAX_BUFFERS][CARBON_STRVIEW_TO_CSTR_MAX_LEN];
  char *x = xs[i];
  memset(x, 0, CARBON_STRVIEW_TO_CSTR_MAX_LEN);
  memcpy(x, sv.data, sv.size);
  x[sv.size] = 0;
  ++i;
  if (i >= CARBON_STRVIEW_TO_CSTR_MAX_BUFFERS) i = 0;
  return x;
}

CBN_StrView carbon_strview_from_strbuilder(CBN_StrBuilder *sb) {
  return carbon_strview_from_buf(sb->items, sb->size);
}

CBN_StrView carbon_strview_trim_left(CBN_StrView sv) {
  usz i = 0;
  while (i < sv.size && isspace(sv.data[i])) ++i;
  return carbon_strview_from_buf(sv.data + i, sv.size - i);
}

CBN_StrView carbon_strview_trim_right(CBN_StrView sv) {
  usz i = 0;
  while (i < sv.size && isspace(sv.data[sv.size - 1 - i])) ++i;
  return carbon_strview_from_buf(sv.data, sv.size - i);
}

CBN_StrView carbon_strview_trim_both(CBN_StrView sv) {
  return carbon_strview_trim_right(carbon_strview_trim_left(sv));
}

CBN_StrView carbon_strview_chop(CBN_StrView *sv, char c) {
  usz i = 0;
  while (i < sv->size && sv->data[i] != c) ++i;
  CBN_StrView new_sv = carbon_strview_from_buf(sv->data, i);
  if (i < sv->size) {
    sv->size -= i + 1;
    sv->data += i + 1;
  }
  else {
    sv->size -= i;
    sv->data += i;
  }
  return new_sv;
}

CBN_StrView carbon_strview_chop_by_space(CBN_StrView *sv) {
  usz i = 0;
  while (i < sv->size && !isspace(sv->data[i])) ++i;
  CBN_StrView new_sv = carbon_strview_from_buf(sv->data, i);
  if (i < sv->size) {
    sv->size -= i + 1;
    sv->data += i + 1;
  }
  else {
    sv->size -= i;
    sv->data += i;
  }
  return new_sv;
}

u8 carbon_strview_are_equal(CBN_StrView x, CBN_StrView y) {
  if (x.size != y.size) return false;
  return !memcmp(x.data, y.data, x.size);
}

u8 carbon_strview_starts_with(CBN_StrView sv, CBN_StrView sub) {
  if (sub.size > sv.size) return false;
  return memcmp(sv.data, sub.data, sub.size) ? false : true;
}

u8 carbon_strview_ends_with(CBN_StrView sv, CBN_StrView sub) {
  if (sub.size > sv.size) return false;
  return memcmp(sv.data + (sv.size - sub.size), sub.data, sub.size) ? false : true;
}
