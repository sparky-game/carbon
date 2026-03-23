/*
  $$==================$$
  ||       Span       ||
  $$==================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents ...
 */
CBNDEF_T(CBN_Span) {
  u8 *data;
  usz size;
};
CBNDEF_TAKA(cbn, Span, CBN_Span);

// Forward declaration
#ifdef __cplusplus
struct CBN_StrBuilder;
#else
typedef struct CBN_List_t CBN_StrBuilder;
#endif

/**
 * @brief Creates a Span from a StrBuilder object holding some data.
 * @param sb The StrBuilder holding the data.
 * @return The Span representing the provided StrBuilder object.
 */
CBNDEF CBN_Span carbon_span_from_strbuilder(const CBN_StrBuilder *sb);
