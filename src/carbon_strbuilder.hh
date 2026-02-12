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

#ifdef __cplusplus
struct CBN_StrBuilder : CBN_StrBuilder_t {
  static cbn::Opt<CBN_StrBuilder> FromFile(const char *file);

  void Free(void) {
    carbon_strbuilder_free(this);
  }

  void Push(void) {
    carbon_strbuilder_add_null(this);
  }

  void Push(const char *s) {
    carbon_strbuilder_add_cstr(this, s);
  }

  void Push(const CBN_StrView &sv) {
    carbon_strbuilder_add_strview(this, sv);
  }

  CBN_StrView ToString(void) const {
    return CBN_StrView::make(*this);
  }

  CBN_StrBuilder &operator<<(const char *s) {
    Push(s);
    return *this;
  }
  
  CBN_StrBuilder &operator<<(const CBN_StrView &sv) {
    Push(sv);
    return *this;
  }
};
#endif
