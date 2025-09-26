/*
**  $$====================$$
**  ||       String       ||
**  $$====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

CARBON_API usz carbon_string_len(const char *s);
CARBON_API i32 carbon_string_cmp(const char *s1, const char *s2);
CARBON_API i32 carbon_string_cmp_n(const char *s1, const char *s2, usz size);
CARBON_API char *carbon_string_dup(const char *s);
CARBON_API char *carbon_string_fmt(const char *s, ...);
CARBON_API void carbon_string_strip_substr(char *s, const char *sub);
CARBON_API bool carbon_string_starts_with_substr(const char *s, const char *sub);
CARBON_API bool carbon_string_ends_with_substr(const char *s, const char *sub);
CARBON_API bool carbon_string_is_number(const char *s);
CARBON_API bool carbon_string_has_char(const char *s, char c);

/**
 * @brief Gets the Levenshtein distance between two strings.
 * @param s1 The first string.
 * @param s2 The second string.
 * @return The Levenshtein distance between s1 and s2.
 */
CARBON_API usz carbon_string_lev_dist(const char *s1, const char *s2);
