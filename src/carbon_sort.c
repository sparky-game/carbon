// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

void carbon_sort_insertion(void *p, usz n, usz w, CBN_Sort_CmpFunc f_cmp) {
  u8 *a = (u8 *) p;
  u8 t[w];
  for (usz i = 1; i < n; ++i) {
    carbon_memory_copy(t, a + w*i, w);
    usz j;
    for (j = i; j > 0 && f_cmp(a + w*(j-1), t) > 0; --j) {
      carbon_memory_copy(a + w*j, a + w*(j-1), w);
    }
    carbon_memory_copy(a + w*j, t, w);
  }
}
