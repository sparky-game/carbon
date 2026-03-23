// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

void carbon_strbuilder_add_strview(CBN_StrBuilder *sb, CBN_StrView sv) {
  carbon_list_push_range(sb, sv);
}

void carbon_strbuilder_add_cstr(CBN_StrBuilder *sb, const char *s) {
  carbon_strbuilder_add_strview(sb, carbon_strview_from_cstr(s));
}

void carbon_strbuilder_add_null(CBN_StrBuilder *sb) {
  carbon_strbuilder_add_strview(sb, carbon_strview_from_buf("", 1));
}

void carbon_strbuilder_free(CBN_StrBuilder *sb) {
  carbon_list_destroy(sb);
}
