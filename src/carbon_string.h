/*
  $$====================$$
  ||       String       ||
  $$====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Gets the number of characters in a C-style string.
 * @param s The C-style string.
 * @return The length of the provided C-style string.
 */
CBNDEF usz carbon_string_len(const char *s);
CBNDEF_FN(cbn::str, Length, carbon_string_len);
CBNDEF_FN(cbn::str, len, Length);

/**
 * @brief Performs a lexicographical comparison between two C-style strings.
 * @param s1 The first C-style string.
 * @param s2 The second C-style string.
 * @return A 32-bit integer that indicates the lexical relationship between them:
 *   - If s1 < s2, then <0.
 *   - If s1 ≡ s2, then 0.
 *   - If s1 > s2, then >0.
 */
CBNDEF i32 carbon_string_cmp(const char *s1, const char *s2);
CBNDEF_FN(cbn::str, Compare, carbon_string_cmp);
CBNDEF_FN(cbn::str, cmp, Compare);

/**
 * @brief Performs a lexicographical comparison (at most `size` chars) between two C-style strings.
 * @param s1 The first C-style string.
 * @param s2 The second C-style string.
 * @param size Maximum number of characters to compare.
 * @return A 32-bit integer that indicates the lexical relationship between them:
 *   - If s1 < s2, then <0.
 *   - If s1 ≡ s2, then 0.
 *   - If s1 > s2, then >0.
 */
CBNDEF i32 carbon_string_cmp_n(const char *s1, const char *s2, usz size);
CBNDEF_FN(cbn::str, CompareCount, carbon_string_cmp_n);
CBNDEF_FN(cbn::str, ncmp, CompareCount);

/**
 * @brief Creates a copy of a C-style string.
 *
 * It allocates sufficient memory for a copy of `s`, performs the copy, and returns a pointer to it.
 * The returned pointer must be free'd to avoid a memory leak.
 *
 * @param s The C-style string.
 * @return The pointer to the new C-style string, which is a duplicate of `s`.
 */
CBNDEF char *carbon_string_dup(const char *s);
CBNDEF_FN(cbn::str, Duplicate, carbon_string_dup);
CBNDEF_FN(cbn::str, dup, Duplicate);

/**
 */
CBNDEF char *carbon_string_fmt(const char *s, ...);
CBNDEF_FN(cbn::str, Format, carbon_string_fmt);
CBNDEF_FN(cbn::str, fmt, Format);

CBNDEF void carbon_string_strip_substr(char *s, const char *sub);

/**
 */
CBNDEF bool carbon_string_starts_with_substr(const char *s, const char *sub);
CBNDEF_FN(cbn::str, StartsWith, carbon_string_starts_with_substr);

/**
 */
CBNDEF bool carbon_string_ends_with_substr(const char *s, const char *sub);
CBNDEF_FN(cbn::str, EndsWith, carbon_string_ends_with_substr);

/**
 * @brief Checks whether a character is categorized as a decimal digit.
 * @param c The character.
 * @return A boolean value representing whether c ∈ [0..9].
 */
CBNDEF bool carbon_string_is_digit(char c);
CBNDEF_FN(cbn::str, IsDigit, carbon_string_is_digit);

/**
 * @brief Checks whether a C-style string represents a number.
 * @param s The C-style string.
 * @return A boolean value representing whether it does or it does not.
 */
CBNDEF bool carbon_string_is_number(const char *s);
CBNDEF_FN(cbn::str, IsNumber, carbon_string_is_number);

/**
 * @brief Converts the specified C-style string to an equivalent 32-bit integer.
 * @param s The C-style string.
 * @return The equivalent 32-bit integer (0 if unable to convert).
 */
CBNDEF i32 carbon_string_to_number(const char *s);
CBNDEF_FN(cbn::str, ToNumber, carbon_string_to_number);

/**
 * @brief Checks whether a C-style string contains a specific character.
 * @param s The C-style string.
 * @param c The character.
 * @return A boolean value representing whether c ∈ s.
 */
CBNDEF bool carbon_string_has_char(const char *s, char c);

/**
 * @brief Finds the first occurrence of a specific character in a C-style string.
 * @param s The C-style string.
 * @param c The character.
 * @return The pointer to the found character in `s`.
 */
CBNDEF char *carbon_string_get_char(const char *s, char c);

/**
 * @brief Gets the Levenshtein distance between two strings.
 * @param s1 The first string.
 * @param s2 The second string.
 * @return The Levenshtein distance between s1 and s2.
 */
CBNDEF usz carbon_string_lev_dist(const char *s1, const char *s2);
CBNDEF_FN(cbn::str, LevDist, carbon_string_lev_dist);
