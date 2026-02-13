/*
  $$========================$$
  ||       StrBuilder       ||
  $$========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a mutable string of characters.
 */
CBNDEF_T(CBN_StrBuilder) {
  char *items;
  usz size;
  usz capacity;
};
CBNDEF_AKA(cbn::str, Builder, CBN_StrBuilder);

/**
 * @brief Appends a copy of the StrView to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 * @param sv The StrView to append.
 */
CBNDEF void carbon_strbuilder_add_strview(CBN_StrBuilder *sb, CBN_StrView sv);

/**
 * @brief Appends a copy of the string to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 * @param s The string to append.
 */
CBNDEF void carbon_strbuilder_add_cstr(CBN_StrBuilder *sb, const char *s);

/**
 * @brief Appends an empty string (null terminator) to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 */
CBNDEF void carbon_strbuilder_add_null(CBN_StrBuilder *sb);

/**
 * @brief Frees (deallocates) and cleans up the CBN_StrBuilder object.
 * @param sb The StrBuilder object.
 */
CBNDEF void carbon_strbuilder_free(CBN_StrBuilder *sb);
