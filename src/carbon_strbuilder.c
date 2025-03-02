// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

void carbon_strbuilder_add_buf(CBN_StrBuilder *sb, const char *data, usz size) {
  if (sb->size + size > sb->capacity) {
    if (!sb->capacity) sb->capacity = 256;
    while (sb->size + size > sb->capacity) sb->capacity *= 2;
    char *prev_p = sb->items;
    usz sz = sb->capacity * sizeof(char);
    sb->items = (char *) CARBON_REALLOC(sb->items, sz);
    if (!sb->items && sb->size > 0) {
      carbon_log_error("failed to reallocate memory (%zuB)", sz);
      CARBON_FREE(prev_p);
      return;
    }
  }
  carbon_memory_copy(sb->items + sb->size, data, size * sizeof(char));
  sb->size += size;
}

void carbon_strbuilder_add_cstr(CBN_StrBuilder *sb, const char *s) {
  carbon_strbuilder_add_buf(sb, s, carbon_string_len(s));
}

void carbon_strbuilder_add_null(CBN_StrBuilder *sb) {
  carbon_strbuilder_add_buf(sb, "", 1);
}

void carbon_strbuilder_free(CBN_StrBuilder *sb) {
  if (!sb) {
    carbon_log_warn("`sb` is not a valid pointer, skipping free");
    return;
  }
  CARBON_FREE(sb->items);
  memset(sb, 0, sizeof(CBN_StrBuilder));
}
