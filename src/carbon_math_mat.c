// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Mat carbon_math_mat_create(usz rows, usz cols) {
  return (CBN_Mat) {
    .items = (f32 *) carbon_memory_alloc(rows * cols * sizeof(f32)),
    .rows = rows,
    .cols = cols
  };
}

void carbon_math_mat_destroy(CBN_Mat *m) {
  if (!m) {
    CBN_WARN("`m` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(m->items);
  carbon_memory_set(m, 0, sizeof(*m));
}

void carbon_math_mat_fill(CBN_Mat m, f32 x) {
  for (usz i = 0; i < m.rows; ++i) {
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_MAT_AT(m, i, j) = x;
    }
  }
}

void carbon_math_mat_rand(CBN_Mat m, f32 min, f32 max) {
  for (usz i = 0; i < m.rows; ++i) {
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_MAT_AT(m, i, j) = carbon_math_randf() * (max - min) + min;
    }
  }
}

CBN_Row carbon_math_mat_row(CBN_Mat m, usz row) {
  return (CBN_Row) {
    .items = &CARBON_MAT_AT(m, row, 0),
    .cols = m.cols
  };
}

void carbon_math_mat_copy(CBN_Mat dst, CBN_Mat src) {
  CBN_ASSERT(dst.rows == src.rows);
  CBN_ASSERT(dst.cols == src.cols);
  for (usz i = 0; i < dst.rows; ++i) {
    for (usz j = 0; j < dst.cols; ++j) {
      CARBON_MAT_AT(dst, i, j) = CARBON_MAT_AT(src, i, j);
    }
  }
}

void carbon_math_mat_add(CBN_Mat dst, CBN_Mat m) {
  CBN_ASSERT(dst.rows == m.rows);
  CBN_ASSERT(dst.cols == m.cols);
  for (usz i = 0; i < dst.rows; ++i) {
    for (usz j = 0; j < dst.cols; ++j) {
      CARBON_MAT_AT(dst, i, j) += CARBON_MAT_AT(m, i, j);
    }
  }
}

void carbon_math_mat_dot(CBN_Mat dst, CBN_Mat a, CBN_Mat b) {
  CBN_ASSERT(a.cols == b.rows);
  CBN_ASSERT(dst.rows == a.rows);
  CBN_ASSERT(dst.cols == b.cols);
  carbon_math_mat_fill(dst, 0);
  usz n = a.cols;
  for (usz i = 0; i < dst.rows; ++i) {
    for (usz k = 0; k < n; ++k) {
      for (usz j = 0; j < dst.cols; ++j) {
        CARBON_MAT_AT(dst, i, j) += CARBON_MAT_AT(a, i, k) * CARBON_MAT_AT(b, k, j);
      }
    }
  }
}

void carbon_math_mat_map(CBN_Mat m, f32 (*f)(f32)) {
  for (usz i = 0; i < m.rows; ++i) {
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_MAT_AT(m, i, j) = f(CARBON_MAT_AT(m, i, j));
    }
  }
}

void carbon_math_mat_print(CBN_Mat m, const char *name) {
  carbon_println("%s = [", name);
  for (usz i = 0; i < m.rows; ++i) {
    carbon_print("  ");
    for (usz j = 0; j < m.cols; ++j) {
      carbon_print("%f ", CARBON_MAT_AT(m, i, j));
    }
    carbon_println("");
  }
  carbon_println("];");
}

CBN_Row carbon_math_row_create(usz cols) {
  return carbon_math_mat_row(carbon_math_mat_create(1, cols), 0);
}

void carbon_math_row_destroy(CBN_Row *r) {
  if (!r) {
    CBN_WARN("`r` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(r->items);
  carbon_memory_set(r, 0, sizeof(*r));
}

CBN_Mat carbon_math_row_to_mat(CBN_Row r) {
  return (CBN_Mat) {
    .items = r.items,
    .rows = 1,
    .cols = r.cols
  };
}

CBN_Row carbon_math_row_slice(CBN_Row r, usz i, usz cols) {
  CBN_ASSERT(i < r.cols);
  CBN_ASSERT(i + cols <= r.cols);
  return (CBN_Row) {
    .items = &CARBON_ROW_AT(r, i),
    .cols = cols
  };
}

void carbon_math_row_fill(CBN_Row r, f32 x) {
  carbon_math_mat_fill(carbon_math_row_to_mat(r), x);
}

void carbon_math_row_rand(CBN_Row r, f32 min, f32 max) {
  carbon_math_mat_rand(carbon_math_row_to_mat(r), min, max);
}

void carbon_math_row_copy(CBN_Row dst, CBN_Row src) {
  carbon_math_mat_copy(carbon_math_row_to_mat(dst), carbon_math_row_to_mat(src));
}

void carbon_math_row_print(CBN_Row r, const char *name) {
  carbon_math_mat_print(carbon_math_row_to_mat(r), name);
}
