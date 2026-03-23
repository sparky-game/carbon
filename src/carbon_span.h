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
CBNDEF_TT(CBN_List);

/**
 */
CBNDEF CBN_Span carbon_span_from_buf(void *data, usz size);

/**
 * @brief Creates a Span from a List object holding some data.
 * @param l The List holding the data.
 * @return The Span representing the provided List object.
 */
CBNDEF CBN_Span carbon_span_from_list(const CBN_List *l);
