/*
**  $$=====================$$
**  ||       StrList       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a CBN_List containter specifically for strings.
 */
typedef struct CBN_StrList {
  char **items;
  usz size;
  usz capacity;
  u8 unique;
#ifdef __cplusplus
  using iterator = const char **;
  using const_iterator = const char * const *;
  /**
   * @brief carbon_strlist_create(unique = false)
   * @return The new StrList, configured to hold repeated strings.
   */
  static CBN_StrList make(void);
  /**
   * @brief carbon_strlist_from_splitted_cstr
   * @param s The C-style string to split.
   * @param delim The delimiter as a C-style string.
   * @return The StrList containing the resulting strings.
   */
  static const CBN_StrList make(const char *s, const char *delim);
  /**
   * @brief carbon_strlist_create(unique = true)
   * @return The new StrList, configured to hold only unique strings.
   */
  static CBN_StrList make_unique(void);
  /**
   * @brief carbon_strlist_destroy
   */
  void Free(void);
  /**
   * @brief carbon_strlist_push
   * @param s The C-style string to append.
   */
  void Push(const char *s);
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void);
  /**
   * @brief Returns a constant iterator to the beginning.
   * @return The const_iterator.
   */
  const_iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void);
  /**
   * @brief Returns a constant iterator to the end.
   * @return The const_iterator.
   */
  const_iterator end(void) const;
#endif
} CBN_StrList;

#define carbon_strlist_at(sl, i) (CARBON_ASSERT(0 <= (i32) (i) && (i) < (sl).size && "StrList index out of bounds"), ((sl).items)[(i)])
#define carbon_strlist_foreach(sl) for (struct { usz i; CBN_StrView sv; } it = {0, carbon_strview_from_cstr(carbon_strlist_at(sl, 0))}; it.i < (sl).size; ++it.i, it.i < (sl).size ? it.sv = carbon_strview_from_cstr(carbon_strlist_at(sl, it.i)) : it.sv)

/**
 * @brief Create a new StrList container, ready to hold strings.
 * @param unique A boolean to configure whether we want the StrList to hold repeated strings or not.
 * @return The new StrList, configured to your liking.
 */
CARBON_API CBN_StrList carbon_strlist_create(u8 unique);

/**
 * @brief Splits a C-style string into a StrList, based on a delimiter.
 * @param s The C-style string to split.
 * @param delim The delimiter as a C-style string.
 * @return The StrList containing the resulting strings.
 */
CARBON_API CBN_StrList carbon_strlist_from_splitted_cstr(const char *s, const char *delim);

/**
 * @brief Destroy a StrList container.
 * @sl The StrList container.
 */
CARBON_API void carbon_strlist_destroy(CBN_StrList *sl);

/**
 * @brief Appends a copy of the string to the end of the container.
 * @param sl The StrList container.
 * @param s The pointer to the standard C char array.
 */
CARBON_API void carbon_strlist_push(CBN_StrList *sl, const char *s);

CARBON_API void carbon_strlist_pop(CBN_StrList *sl, const char *s);
CARBON_API u8 carbon_strlist_contains(CBN_StrList *sl, const char *s);

// Local Variables:
// mode: c++
// End:
