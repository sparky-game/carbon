/*
  $$=====================$$
  ||       StrView       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a view into a string of characters, whilst not owning the actual memory.
 */
CBNDEF_T(CBN_StrView) {
  const char *data;
  usz size;
};
CBNDEF_AKA(cbn::str, View, CBN_StrView);

// Forward declaration
#ifdef __cplusplus
struct CBN_StrBuilder;
#else
typedef struct CBN_StrBuilder_t CBN_StrBuilder;
#endif

/**
 * @brief Creates a StrView from a char buffer pointer and its size.
 * @param data The pointer to the char buffer.
 * @param size The size of the char buffer.
 * @return The StrView representing the provided char buffer.
 */
CBNDEF CBN_StrView carbon_strview_from_buf(const char *data, usz size);

/**
 * @brief Creates a StrView from a null-terminated C-style string.
 * @param s The pointer to the standard C char array.
 * @return The StrView representing the provided C-style string.
 */
CBNDEF CBN_StrView carbon_strview_from_cstr(const char *s);

/**
 * @brief Creates a StrView from a StrBuilder object holding some data.
 * @param sb The StrBuilder holding the data.
 * @return The StrView representing the provided StrBuilder object.
 */
CBNDEF CBN_StrView carbon_strview_from_strbuilder(const CBN_StrBuilder *sb);

/**
 * @brief Returns a non-modifiable standard C char array version of the string.
 * @param sv The StrView holding the data.
 * @return A pointer to a null-terminated char array with same data.
 */
CBNDEF char *carbon_strview_to_cstr(CBN_StrView sv);

/**
 * @brief Removes all the leading whitespace characters from the current StrView.
 * @param sv The StrView holding the data.
 * @return The StrView that remains after all whitespace characters are removed from the start.
 */
CBNDEF CBN_StrView carbon_strview_trim_left(CBN_StrView sv);

/**
 * @brief Removes all the trailing whitespace characters from the current StrView.
 * @param sv The StrView holding the data.
 * @return The StrView that remains after all whitespace characters are removed from the end.
 */
CBNDEF CBN_StrView carbon_strview_trim_right(CBN_StrView sv);

/**
 * @brief Removes all leading and trailing whitespace characters from the current StrView.
 * @param sv The StrView holding the data.
 * @return The StrView that remains after all whitespace characters are removed from the start and the end.
 */
CBNDEF CBN_StrView carbon_strview_trim_both(CBN_StrView sv);

/**
 * @brief Returns the leading portion of the current StrView just before the first occurrence of a delimiter.
 *
 * The StrView is taken as a pointer, because it's modified to point to the data after the first occurrence of the delimiter.
 *
 * @param sv The StrView holding the data.
 * @param c The char delimiter to search for.
 * @return The StrView containing the data from the beginning up to the first occurrence of the delimiter.
 */
CBNDEF CBN_StrView carbon_strview_chop(CBN_StrView *sv, char c);

/**
 * @brief Returns the leading portion of the current StrView just before the first occurrence of a whitespace character.
 *
 * The StrView is taken as a pointer, because it's modified to point to the data after the first occurrence of the whitespace character.
 *
 * @param sv The StrView holding the data.
 * @return The StrView containing the data from the beginning up to the first occurrence of the whitespace character.
 */
CBNDEF CBN_StrView carbon_strview_chop_by_space(CBN_StrView *sv);

/**
 * @brief Compares two StrViews and returns whether they're equal.
 * @param x The first StrView.
 * @param y The second StrView.
 * @return A boolean value representing the equality between them.
 */
CBNDEF bool carbon_strview_are_equal(CBN_StrView x, CBN_StrView y);

/**
 * @brief Checks whether a StrView is contained in another StrView.
 * @param sv The main StrView.
 * @param sub The sub StrView to search for.
 * @return A boolean value representing whether sub ∈ sv.
 */
CBNDEF bool carbon_strview_contains(CBN_StrView sv, CBN_StrView sub);

/**
 * @brief Checks whether a StrView starts with a particular StrView.
 * @param sv The main StrView.
 * @param sub The sub StrView to search for.
 * @return A boolean value representing whether `sv` starts with `sub`.
 */
CBNDEF bool carbon_strview_starts_with(CBN_StrView sv, CBN_StrView sub);

/**
 * @brief Checks whether a StrView ends with a particular StrView.
 * @param sv The main StrView.
 * @param sub The sub StrView to search for.
 * @return A boolean value representing whether `sv` ends with `sub`.
 */
CBNDEF bool carbon_strview_ends_with(CBN_StrView sv, CBN_StrView sub);

#ifdef __cplusplus
struct CBN_StrView : CBN_StrView_t {
  static CBN_StrView make(const char *data, usz size) {
    return carbon_strview_from_buf(data, size);
  }

  static CBN_StrView make(const char *s) {
    return carbon_strview_from_cstr(s);
  }

  
  static CBN_StrView make(const CBN_StrBuilder &sb) {
    return carbon_strview_from_strbuilder(&sb);
  }

  const char *c_str(void) const {
    return carbon_strview_to_cstr(*this);
  }

  CBN_StrView Trim(void) const {
    return carbon_strview_trim_both(*this);
  }

  CBN_StrView TrimLeft(void) const {
    return carbon_strview_trim_left(*this);
  }

  CBN_StrView TrimRight(void) const {
    return carbon_strview_trim_right(*this);
  }

  CBN_StrView Chop(void) {
    return carbon_strview_chop_by_space(this);
  }

  CBN_StrView Chop(char c) {
    return carbon_strview_chop(this, c);
  }

  bool Contains(const CBN_StrView &sv) const {
    return carbon_strview_contains(*this, sv);
  }

  bool StartsWith(const CBN_StrView &sv) const {
    return carbon_strview_starts_with(*this, sv);
  }

  bool EndsWith(const CBN_StrView &sv) const {
    return carbon_strview_ends_with(*this, sv);
  }

  usz LevDist(const CBN_StrView &sv) const {
    return carbon_string_lev_dist(c_str(), sv.c_str());
  }
  
  bool operator==(const CBN_StrView &sv) const {
    return carbon_strview_are_equal(*this, sv);
  }
  
  bool operator==(const char *s) const {
    return *this == make(s);
  }
  
  friend bool operator==(const char *s, const CBN_StrView &sv) {
    return sv == s;
  }
  
  bool operator!=(const CBN_StrView &sv) const {
    return !(*this == sv);
  }
  
  bool operator!=(const char *s) const {
    return !(*this == s);
  }
  
  friend bool operator!=(const char *s, const CBN_StrView &sv) {
    return !(s == sv);
  }
};
#endif
