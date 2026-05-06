/*
  $$=====================$$
  ||       Sorting       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Comparison function type used by all sorting procedures.
 * @return Negative if `a < b`, zero if `a == b`, positive if `a > b`.
 */
typedef i32 (*CBN_Sort_CmpFunc)(const void *, const void *);
CBNDEF_TAKA(cbn::sort, CmpFunc, CBN_Sort_CmpFunc);

/**
 * @brief Sorting function type common to all sorting procedures.
 */
typedef void (*CBN_Sort_Func)(void *, usz, usz, CBN_Sort_CmpFunc);
CBNDEF_TAKA(cbn::sort, Func, CBN_Sort_Func);

/**
 * @brief Sorts the elements in the range in-place using the Insertion sort algorithm.
 * @param p Pointer to the first element of the range to sort.
 * @param n Number of elements in the range.
 * @param w Size in bytes of each element.
 * @param f_cmp Comparison function used to order elements.
 */
CBNDEF void carbon_sort_insertion(void *p, usz n, usz w, CBN_Sort_CmpFunc f_cmp);
static_assert(typeeq(CBN_Sort_Func, typeof(&carbon_sort_insertion)), "Must adhere to the API");
CBNDEF_AKA(cbn::sort, Insertion, carbon_sort_insertion);
