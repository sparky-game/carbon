// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

static u64 carbon_math__rand_seed;

void carbon_math_srand(u64 seed) {
  carbon_math__rand_seed = seed - 1;
}

int carbon_math_rand(void) {
  carbon_math__rand_seed = CARBON_PCG_RAND_MAGIC * carbon_math__rand_seed + 1;
  return carbon_math__rand_seed >> 33;
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

f32 carbon_math_floor(f32 x) {
  union { f32 f; u32 i; } u = {x};
  i32 e = (i32) (u.i >> 23 & 0xff) - 0x7f;
  u32 m;
  if (e >= 23) return x;
  if (e >= 0) {
    m = 0x007fffff >> e;
    if (0 == (u.i & m)) return x;
    volatile f32 __tmp = x + 0x1p120f;
    CARBON_NOTUSED(__tmp);
    if (u.i >> 31) u.i += m;
    u.i &= ~m;
  }
  else {
    volatile f32 __tmp = x + 0x1p120f;
    CARBON_NOTUSED(__tmp);
    if (0 == u.i >> 31) u.i = 0;
    else if (u.i << 1) u.f = -1;
  }
  return u.f;
}

f32 carbon_math_ceil(f32 x) {
  union { f32 f; u32 i; } u = {x};
  i32 e = (i32) (u.i >> 23 & 0xff) - 0x7f;
  u32 m;
  if (e >= 23) return x;
  if (e >= 0) {
    m = 0x007fffff >> e;
    if (0 == (u.i & m)) return x;
    volatile f32 __tmp = x + 0x1p120f;
    CARBON_NOTUSED(__tmp);
    if (0 == u.i >> 31) u.i += m;
    u.i &= ~m;
  }
  else {
    volatile f32 __tmp = x + 0x1p120f;
    CARBON_NOTUSED(__tmp);
    if (u.i >> 31) u.f = -0;
    else if (u.i << 1) u.f = 1;
  }
  return u.f;
}

f32 carbon_math_sqrt(f32 x) {
  f32 s = x;
  for (usz i = 0; i < 1e3 && carbon_math_abs(s*s - x) > 1e-6; ++i) {
    s -= (s*s - x) / (2*s);
  }
  return s;
}

f32 carbon_math_exp2(f32 x) {
  __attribute__((aligned(16))) static const f32 p[] = {
    1.535336188319500e-4,
    1.339887440266574e-3,
    9.618437357674640e-3,
    5.550332471162809e-2,
    2.402264791363012e-1,
    6.931472028550421e-1,
    1
  };
  f32 ipart, fpart;
  union { f32 f; u32 u; i32 i; } epart;
  ipart = carbon_math_floor(x + 0.5);
  fpart = x - ipart;
  epart.i = (((i32) ipart) + 127) << 23;
  x =             p[0];
  x = x * fpart + p[1];
  x = x * fpart + p[2];
  x = x * fpart + p[3];
  x = x * fpart + p[4];
  x = x * fpart + p[5];
  x = x * fpart + p[6];
  return epart.f * x;
}

f32 carbon_math_exp(f32 x) {
  return carbon_math_exp2(CARBON_LOG2_E * x);
}

f32 carbon_math_exp10(f32 x) {
  return carbon_math_exp2(CARBON_LOG2_10 * x);
}

f32 carbon_math_sigmoid(f32 x) {
  return 1 / (1 + carbon_math_exp(-x));
}

f32 carbon_math_tanh(f32 x) {
  f32 ex = carbon_math_exp(x);
  f32 enx = carbon_math_exp(-x);
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

u8 carbon_math_rect_contains_point(CBN_Rect r, CBN_Vec2 p) {
  return (p.x >= r.x && p.x <= r.x + r.w) && (p.y >= r.y && p.y <= r.y + r.h);
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
