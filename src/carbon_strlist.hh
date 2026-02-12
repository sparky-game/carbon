/*
  $$=====================$$
  ||       StrList       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a CBN_List containter specifically for strings.
 */
CBNDEF_T(CBN_StrList) {
  char **items;
  usz size;
  usz capacity;
  u8 unique;
};
CBNDEF_AKA(cbn::str, List, CBN_StrList);

#define carbon_strlist_at(sl, i) (CBN_ASSERT(0 <= (i32) (i) && (i) < (sl).size && "StrList index out of bounds"), ((sl).items)[(i)])

/**
 * @brief Loops through the provided StrList with a custom-defined iterator.
 *
 * The iterator consists of the following elements:
 *   - `i`  :: The index of the actual element being evaluated.
 *   - `sv` :: The actual element being evaluated, as a StrView object.
 * @param name The name to give to the iterator (optional; default = `it`).
 * @param sl The StrList container.
 */
#define carbon_strlist_foreach__named(name, sl) for (struct { usz i; CBN_StrView sv; } name = {0, carbon_strview_from_cstr(carbon_strlist_at(sl, 0))}; name.i < (sl).size; ++name.i, name.i < (sl).size ? name.sv = carbon_strview_from_cstr(carbon_strlist_at(sl, name.i)) : name.sv)
#define carbon_strlist_foreach__default(sl) carbon_strlist_foreach__named(it, sl)
#define carbon_strlist_foreach__dispatcher(_1, _2, NAME, ...) NAME
#define carbon_strlist_foreach(...) carbon_strlist_foreach__dispatcher(__VA_ARGS__, carbon_strlist_foreach__named, carbon_strlist_foreach__default)(__VA_ARGS__)

/**
 * @brief Create a new StrList container, ready to hold strings.
 * @param unique A boolean to configure whether we want the StrList to hold repeated strings or not.
 * @return The new StrList, configured to your liking.
 */
CBNDEF CBN_StrList carbon_strlist_create(u8 unique);

/**
 * @brief Splits a C-style string into a StrList, based on a delimiter.
 * @param s The C-style string to split.
 * @param delim The delimiter as a C-style string.
 * @return The StrList containing the resulting strings.
 */
CBNDEF CBN_StrList carbon_strlist_from_splitted_cstr(const char *s, const char *delim);

/**
 * @brief Destroy a StrList container.
 * @param sl The StrList container.
 */
CBNDEF void carbon_strlist_destroy(CBN_StrList *sl);

/**
 * @brief Appends a copy of the string to the end of the container.
 * @param sl The StrList container.
 * @param s The pointer to the standard C char array.
 */
CBNDEF void carbon_strlist_push(CBN_StrList *sl, const char *s);

CBNDEF void carbon_strlist_pop(CBN_StrList *sl, const char *s);
CBNDEF u8 carbon_strlist_contains(CBN_StrList *sl, const char *s);

#ifdef __cplusplus
struct CBN_StrList : CBN_StrList_t {
  using iterator = char **;

  static CBN_StrList make(void) {
    return carbon_strlist_create(false);
  }

  static CBN_StrList make(const char *s, const char *delim) {
    return carbon_strlist_from_splitted_cstr(s, delim);
  }

  static CBN_StrList make_unique(void) {
    return carbon_strlist_create(true);
  }

  void Free(void) {
    carbon_strlist_destroy(this);
  }

  void Push(const char *s) {
    carbon_strlist_push(this, s);
  }

  iterator begin(void) const {
    return items;
  }

  iterator end(void) const {
    return items + size;
  }
};
#endif
