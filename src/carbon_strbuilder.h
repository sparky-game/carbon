/*
**  $$========================$$
**  ||       StrBuilder       ||
**  $$========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a mutable string of characters.
 */
typedef struct CBN_StrBuilder {
  char *items;
  usz size;
  usz capacity;
#ifdef __cplusplus
  /**
   * @brief carbon_fs_read_entire_file
   * @param file The filename to read the data from.
   * @return The StrBuilder object where the read data will be stored.
   */
  static Opt<CBN_StrBuilder> make(const char *file);
  /**
   * @brief carbon_strbuilder_free
   */
  void Free(void);
  /**
   * @brief carbon_strbuilder_add_null
   */
  void Push(void);
  /**
   * @brief carbon_strbuilder_add_cstr
   * @param s The string to append.
   */
  void Push(const char *s);
  /**
   * @brief carbon_strbuilder_add_strview
   * @param sv The StrView to append.
   */
  void Push(const CBN_StrView &sv);
  /**
   * @brief StrView::make(CBN_StrBuilder const&)
   * @return The StrView representing the current StrBuilder object.
   */
  CBN_StrView ToString(void) const;
  // Overloaded Operators
  CBN_StrBuilder &operator<<(const char *s);
  CBN_StrBuilder &operator<<(const CBN_StrView &sv);
#endif
} CBN_StrBuilder;

/**
 * @brief Appends a copy of the StrView to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 * @param sv The StrView to append.
 */
CARBON_API void carbon_strbuilder_add_strview(CBN_StrBuilder *sb, CBN_StrView sv);

/**
 * @brief Appends a copy of the string to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 * @param s The string to append.
 */
CARBON_API void carbon_strbuilder_add_cstr(CBN_StrBuilder *sb, const char *s);

/**
 * @brief Appends an empty string (null terminator) to the end of the StrBuilder's buffer.
 * @param sb The StrBuilder object.
 */
CARBON_API void carbon_strbuilder_add_null(CBN_StrBuilder *sb);

/**
 * @brief Frees (deallocates) and cleans up the CBN_StrBuilder object.
 * @param sb The StrBuilder object.
 */
CARBON_API void carbon_strbuilder_free(CBN_StrBuilder *sb);

// Local Variables:
// mode: c++
// End:
