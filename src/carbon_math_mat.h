/*
  $$========================$$
  ||       Math (Mat)       ||
  $$========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef struct {
  f32 *items;
  usz rows;
  usz cols;
} CBN_Mat;

typedef struct {
  f32 *items;
  usz cols;
} CBN_Row;

// TODO: somehow perform bounds checking `CBN_ASSERT(0 <= (i) && (i) < (m).rows && 0 <= (j) && (j) < (m).cols && "Matrix index out of bounds")`
#define CARBON_MAT_AT(m, i, j) (m).items[(i) * (m).cols + (j)]

// TODO: somehow perform bounds checking `CBN_ASSERT(0 <= (i) && (i) < (r).cols && "Row index out of bounds")`
#define CARBON_ROW_AT(r, i) (r).items[(i)]

#define CARBON_MAT_PRINT(m) carbon_math_mat_print(m, #m)
#define CARBON_ROW_PRINT(r) carbon_math_row_print(r, #r)

CBNDEF CBN_Mat carbon_math_mat_create(usz rows, usz cols);
CBNDEF void carbon_math_mat_destroy(CBN_Mat *m);
CBNDEF void carbon_math_mat_fill(CBN_Mat m, f32 x);
CBNDEF void carbon_math_mat_rand(CBN_Mat m, f32 min, f32 max);
CBNDEF CBN_Row carbon_math_mat_row(CBN_Mat m, usz row);
CBNDEF void carbon_math_mat_copy(CBN_Mat dst, CBN_Mat src);
CBNDEF void carbon_math_mat_add(CBN_Mat dst, CBN_Mat m);
CBNDEF void carbon_math_mat_dot(CBN_Mat dst, CBN_Mat a, CBN_Mat b);
CBNDEF void carbon_math_mat_map(CBN_Mat m, f32 (*f)(f32));
CBNDEF void carbon_math_mat_print(CBN_Mat m, const char *name);
CBNDEF CBN_Row carbon_math_row_create(usz cols);
CBNDEF void carbon_math_row_destroy(CBN_Row *r);
CBNDEF CBN_Mat carbon_math_row_to_mat(CBN_Row r);
CBNDEF CBN_Row carbon_math_row_slice(CBN_Row r, usz i, usz cols);
CBNDEF void carbon_math_row_fill(CBN_Row r, f32 x);
CBNDEF void carbon_math_row_rand(CBN_Row r, f32 min, f32 max);
CBNDEF void carbon_math_row_copy(CBN_Row dst, CBN_Row src);
CBNDEF void carbon_math_row_print(CBN_Row r, const char *name);
