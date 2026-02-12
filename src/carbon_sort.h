/*
  $$=====================$$
  ||       Sorting       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief ...
 *
 * ...
 *
 * @return ...
 */
typedef i32 (*CBN_Sort_CmpFunc)(const void *, const void *);
CBNDEF_AKA(cbn::sort, CmpFunc, CBN_Sort_CmpFunc);

/**
 * @brief ...
 * @param p ...
 * @param n ...
 * @param w ...
 * @param f_cmp ...
 */
void carbon_sort_insertion(void *p, usz n, usz w, CBN_Sort_CmpFunc f_cmp);
CBNDEF_FN(cbn::sort, Insertion, carbon_sort_insertion);
