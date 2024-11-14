// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

// TODO: replace with own implementation
CARBON_API f32 expf(f32);

static u64 cbn_math_rand_seed;

void carbon_math_srand(u64 seed) {
  cbn_math_rand_seed = seed - 1;
}

int carbon_math_rand(void) {
  cbn_math_rand_seed = CARBON_PCG_RAND_MAGIC * cbn_math_rand_seed + 1;
  return cbn_math_rand_seed >> 33;
}

f32 carbon_math_randf(void) {
  return (f32) carbon_math_rand() / (f32) CARBON_RAND_MAX;
}

f32 carbon_math_abs(f32 x) {
  union { f32 f; u32 i; } u = {x};
  u.i &= CARBON_I32_MAX;
  return u.f;
}

f32 carbon_math_round(f32 x) {
  return x >= 0 ? (i32) (x + 0.5) : (i32) (x - 0.5);
}

f32 carbon_math_sqrt(f32 x) {
  f32 s = x;
  for (usz i = 0; i < 1e3 && carbon_math_abs(s*s - x) > 1e-6; ++i) {
    s -= (s*s - x) / (2*s);
  }
  return s;
}

f32 carbon_math_exp(f32 x) {
  // e^x = 1 / e^(-x)
  u8 negative = x < 0;
  if (negative) x = -x;
  // e^x = (e^(x/2))^2
  while (x > 1) {
    x /= 2;
    negative = false;
  }
  // Precomputed values
  f32 result = 1;
  if (x >= 0.5f) {
    result *= CARBON_E_0_5;
    x -= 0.5f;
  }
  if (x >= 0.25f) {
    result *= CARBON_E_0_25;
    x -= 0.25f;
  }
  if (x >= 0.125f) {
    result *= CARBON_E_0_125;
    x -= 0.125f;
  }
  // Taylor series
  f32 sum = 1;
  f32 term = 1;
  for (usz i = 0; i <= 10; ++i) {
    term *= x / i;
    sum += term;
    if (term < 1e-7) break;
  }
  result *= sum;
  return negative ? 1 / result : result;
}

f32 carbon_math_sigmoid(f32 x) {
  // return 1 / (1 + carbon_math_exp(-x));
  return 1 / (1 + expf(-x));
}

f32 carbon_math_tanh(f32 x) {
  // f32 ex = carbon_math_exp(x);
  f32 ex = expf(x);
  // f32 enx = carbon_math_exp(-x);
  f32 enx = expf(-x);
  return (ex - enx) / (ex + enx);
}

f32 carbon_math_smoothstep(f32 a, f32 b, f32 t) {
  CARBON_ASSERT(a < b);
  f32 x = CARBON_CLAMP((t - a) / (b - a), 0, 1);
  return x * x * (3 - 2 * x);
}

i8 carbon_math_cmp(f32 x, f32 y) {
  f32 eps = 1e-6;
  if ((x - eps) < y)      return -1;
  else if ((x + eps) > y) return  1;
  else return 0;
}

f32 carbon_math_sin(f32 x) {
  return carbon_math_cos(x - CARBON_PI_2);
}

f32 carbon_math_cos(f32 x) {
  if (x < 0) x = -x;
  if (0 <= carbon_math_cmp(x, CARBON_2PI)) {
    do { x -= CARBON_2PI; } while (0 <= carbon_math_cmp(x, CARBON_2PI));
  }
  if ((0 <= carbon_math_cmp(x, CARBON_PI)) && (-1 == carbon_math_cmp(x, CARBON_2PI))) {
    x -= CARBON_PI;
    return ((-1)*(1-(x*x/2)*(1-(x*x/12)*(1-(x*x/30)*(1-(x*x/56)*(1-(x*x/90)*(1-(x*x/132)*(1-(x*x/182)))))))));
  }
  return 1-(x*x/2)*(1-(x*x/12)*(1-(x*x/30)*(1-(x*x/56)*(1-(x*x/90)*(1-(x*x/132)*(1-(x*x/182)))))));
}

CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v) {
  return (CBN_Vec2) {
    .x = u.x + v.x,
    .y = u.y + v.y
  };
}

CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v) {
  return (CBN_Vec3) {
    .x = u.x + v.x,
    .y = u.y + v.y,
    .z = u.z + v.z
  };
}

CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v) {
  return (CBN_Vec2) {
    .x = u.x - v.x,
    .y = u.y - v.y
  };
}

CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v) {
  return (CBN_Vec3) {
    .x = u.x - v.x,
    .y = u.y - v.y,
    .z = u.z - v.z
  };
}

f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v) {
  return (u.x * v.x) + (u.y * v.y);
}

f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v) {
  return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v) {
  return (CBN_Vec3) {
    .x = (u.y * v.z) - (u.z * v.y),
    .y = (u.z * v.x) - (u.x * v.z),
    .z = (u.x * v.y) - (u.y * v.x)
  };
}

CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle) {
  f32 rads = angle * (CARBON_PI / 180);
  f32 c = carbon_math_cos(rads), s = carbon_math_sin(rads);
  return (CBN_Vec2) {
    .x = (v.x * c) - (v.y * s),
    .y = (v.x * s) + (v.y * c)
  };
}

CBN_Matrix carbon_math_mat_create(usz rows, usz cols) {
  CBN_Matrix m = {
    .items = (f32 *) CARBON_MALLOC(rows * cols * sizeof(f32)),
    .rows = rows,
    .cols = cols
  };
  if (!m.items) {
    CARBON_ERROR("failed to allocate memory (%zuB)", rows * cols * sizeof(f32));
    memset(&m, 0, sizeof(CBN_Matrix));
  }
  return m;
}

void carbon_math_mat_destroy(CBN_Matrix *m) {
  if (!m) {
    CARBON_WARNING("`m` is not a valid pointer, skipping destruction");
    return;
  }
  CARBON_FREE(m->items);
  memset(m, 0, sizeof(CBN_Matrix));
  m = 0;
}

void carbon_math_mat_fill(CBN_Matrix m, f32 x) {
  for (usz i = 0; i < m.rows; ++i) {
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_MAT_AT(m, i, j) = x;
    }
  }
}

void carbon_math_mat_rand(CBN_Matrix m, f32 min, f32 max) {
  for (usz i = 0; i < m.rows; ++i) {
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_MAT_AT(m, i, j) = carbon_math_randf() * (max - min) + min;
    }
  }
}

CBN_Row carbon_math_mat_row(CBN_Matrix m, usz row) {
  return (CBN_Row) {
    .items = &CARBON_MAT_AT(m, row, 0),
    .cols = m.cols
  };
}

void carbon_math_mat_copy(CBN_Matrix dst, CBN_Matrix src) {
  CARBON_ASSERT(dst.rows == src.rows);
  CARBON_ASSERT(dst.cols == src.cols);
  for (usz i = 0; i < dst.rows; ++i) {
    for (usz j = 0; j < dst.cols; ++j) {
      CARBON_MAT_AT(dst, i, j) = CARBON_MAT_AT(src, i, j);
    }
  }
}

void carbon_math_mat_add(CBN_Matrix dst, CBN_Matrix m) {
  CARBON_ASSERT(dst.rows == m.rows);
  CARBON_ASSERT(dst.cols == m.cols);
  for (usz i = 0; i < dst.rows; ++i) {
    for (usz j = 0; j < dst.cols; ++j) {
      CARBON_MAT_AT(dst, i, j) += CARBON_MAT_AT(m, i, j);
    }
  }
}

void carbon_math_mat_dot(CBN_Matrix dst, CBN_Matrix a, CBN_Matrix b) {
  CARBON_ASSERT(a.cols == b.rows);
  CARBON_ASSERT(dst.rows == a.rows);
  CARBON_ASSERT(dst.cols == b.cols);
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

void carbon_math_mat_map(CBN_Matrix m, f32 (*f)(f32)) {
  for (usz i = 0; i < m.rows; ++i) {
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_MAT_AT(m, i, j) = f(CARBON_MAT_AT(m, i, j));
    }
  }
}

void carbon_math_mat_print(CBN_Matrix m, const char *name) {
  CARBON_INFO("%s = [", name);
  for (usz i = 0; i < m.rows; ++i) {
    CARBON_INFO_RAW("  ");
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_INFO_RAW("%f ", CARBON_MAT_AT(m, i, j));
    }
    CARBON_INFO();
  }
  CARBON_INFO("];");
}

CBN_Row carbon_math_row_create(usz cols) {
  return carbon_math_mat_row(carbon_math_mat_create(1, cols), 0);
}

void carbon_math_row_destroy(CBN_Row *r) {
  if (!r) {
    CARBON_WARNING("`r` is not a valid pointer, skipping destruction");
    return;
  }
  CARBON_FREE(r->items);
  memset(r, 0, sizeof(CBN_Row));
  r = 0;
}

CBN_Matrix carbon_math_row_to_mat(CBN_Row r) {
  return (CBN_Matrix) {
    .items = r.items,
    .rows = 1,
    .cols = r.cols
  };
}

CBN_Row carbon_math_row_slice(CBN_Row r, usz i, usz cols) {
  CARBON_ASSERT(i < r.cols);
  CARBON_ASSERT(i + cols <= r.cols);
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
