// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

void carbon_strbuilder_add_strview(CBN_StrBuilder *sb, CBN_StrView sv) {
  if (sb->size + sv.size > sb->capacity) {
    if (!sb->capacity) sb->capacity = 256;
    while (sb->size + sv.size > sb->capacity) sb->capacity *= 2;
    sb->items = (char *) CARBON_REALLOC(sb->items, sb->capacity * sizeof(char));
    CARBON_ASSERT(sb->items && "failed to reallocate memory");
  }
  carbon_memory_copy(sb->items + sb->size, sv.data, sv.size * sizeof(char));
  sb->size += sv.size;
}

void carbon_strbuilder_add_cstr(CBN_StrBuilder *sb, const char *s) {
  carbon_strbuilder_add_strview(sb, carbon_strview_from_cstr(s));
}

void carbon_strbuilder_add_null(CBN_StrBuilder *sb) {
  carbon_strbuilder_add_strview(sb, carbon_strview_from_buf("", 1));
}

void carbon_strbuilder_free(CBN_StrBuilder *sb) {
  if (!sb) {
    carbon_log_warn("`sb` is not a valid pointer, skipping free");
    return;
  }
  CARBON_FREE(sb->items);
  carbon_memory_set(sb, 0, sizeof(*sb));
}
