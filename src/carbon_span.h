/*
  $$==================$$
  ||       Span       ||
  $$==================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a contiguous sequence of elements. (?)
 */
CBNDEF_TT(CBN_Span) {
  u8 *data;
  usz size;
};
CBNDEF_TTAKA(cbn, Span, CBN_Span);

// Forward declaration
CBNDEF_TT(CBN_List);

/**
 * @brief Gets the element in the provided Span at a specific index.
 * @param T The underlying type you want the Span to represent.
 * @param l The Span.
 * @param i The index to access.
 */
#define carbon_span_at_raw(T, s, i) ((T *) (s).data)[(i)]
#define carbon_span_at(T, s, i) (CBN_ASSERT((i) < (s).size && "Span index out of bounds"), carbon_span_at_raw(T, s, i))

/**
 * @brief Creates a Span from a data buffer pointer and its size.
 * @param data The pointer to the data buffer.
 * @param size The size of the data buffer.
 * @return The Span representing the provided data buffer.
 */
CBNDEF CBN_Span carbon_span_from_buf(void *data, usz size);

/**
 * @brief Creates a Span from a List object holding some data.
 * @param l The List holding the data.
 * @return The Span representing the provided List object.
 */
CBNDEF CBN_Span carbon_span_from_list(const CBN_List *l);

/**
 */
CBNDEF CBN_Span carbon_span_subspan(CBN_Span s, usz offset, usz count);
