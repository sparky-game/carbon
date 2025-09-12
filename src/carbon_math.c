// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_MATH__RAND_PCG_MAGIC 6364136223846793005ULL
#define CARBON_MATH__MT19937_64_RAND_NN 312
#define CARBON_MATH__MT19937_64_RAND_MM 156
#define CARBON_MATH__MT19937_64_RAND_MATRIX_A 0xB5026F5AA96619E9ULL
#define CARBON_MATH__MT19937_64_RAND_UM 0xFFFFFFFF80000000ULL
#define CARBON_MATH__MT19937_64_RAND_LM 0x7FFFFFFFULL

static u64 carbon_math__rand_seed;
static u64 carbon_math__mt19937_64_rand_state_vec[CARBON_MATH__MT19937_64_RAND_NN];
static u64 carbon_math__mt19937_64_rand_state_idx = CARBON_MATH__MT19937_64_RAND_NN + 1;

void carbon_math_srand(u64 seed) {
  carbon_math__rand_seed = seed - 1;
}

i32 carbon_math_rand(void) {
  if (!carbon_math__rand_seed) carbon_math_srand((u64)(carbon_time_get() * 1e6));
  carbon_math__rand_seed = CARBON_MATH__RAND_PCG_MAGIC * carbon_math__rand_seed + 1;
  return carbon_math__rand_seed >> 33;
}

i32 carbon_math_rand_between(i32 min, i32 max) {
  return carbon_math_rand() % (max + 1 - min) + min;
}

f32 carbon_math_randf(void) {
  return (f32) carbon_math_rand() / (f32) 0x7fffffff;
}

f32 carbon_math_randf_between(f32 min, f32 max) {
  return carbon_math_randf() * (max - min) + min;
}

void carbon_math_mt19937_64_srand(u64 seed) {
  u64 *sv = carbon_math__mt19937_64_rand_state_vec;
  u64 *si = &carbon_math__mt19937_64_rand_state_idx;
  sv[0] = seed;
  for (*si = 1; *si < CARBON_MATH__MT19937_64_RAND_NN; ++(*si)) {
    sv[*si] = (6364136223846793005ULL * (sv[*si - 1] ^ (sv[*si - 1] >> 62)) + *si);
  }
}

u64 carbon_math_mt19937_64_rand(void) {
  // NOTE: default seed was `5489ULL` and was changed to `carbon_time_get()` in ms
  static u64 mag[] = {0ULL, CARBON_MATH__MT19937_64_RAND_MATRIX_A};
  i32 i; u64 x;
  u64 *sv = carbon_math__mt19937_64_rand_state_vec;
  u64 *si = &carbon_math__mt19937_64_rand_state_idx;
  if (*si >= CARBON_MATH__MT19937_64_RAND_NN) {
    if (*si == CARBON_MATH__MT19937_64_RAND_NN + 1) carbon_math_mt19937_64_srand((u64)(carbon_time_get() * 1e6));
    for (i = 0; i < CARBON_MATH__MT19937_64_RAND_NN - CARBON_MATH__MT19937_64_RAND_MM; ++i) {
      x = (sv[i] & CARBON_MATH__MT19937_64_RAND_UM) | (sv[i + 1] & CARBON_MATH__MT19937_64_RAND_LM);
      sv[i] = sv[i + CARBON_MATH__MT19937_64_RAND_MM] ^ (x >> 1) ^ mag[(i32) (x & 1ULL)];
    }
    for (; i < CARBON_MATH__MT19937_64_RAND_NN - 1; ++i) {
      x = (sv[i] & CARBON_MATH__MT19937_64_RAND_UM) | (sv[i + 1] & CARBON_MATH__MT19937_64_RAND_LM);
      sv[i] = sv[i + (CARBON_MATH__MT19937_64_RAND_MM - CARBON_MATH__MT19937_64_RAND_NN)] ^ (x >> 1) ^ mag[(i32) (x & 1ULL)];
    }
    x = (sv[CARBON_MATH__MT19937_64_RAND_NN - 1] & CARBON_MATH__MT19937_64_RAND_UM) | (sv[0] & CARBON_MATH__MT19937_64_RAND_LM);
    sv[CARBON_MATH__MT19937_64_RAND_NN - 1] = sv[CARBON_MATH__MT19937_64_RAND_MM - 1] ^ (x >> 1) ^ mag[(i32) (x & 1ULL)];
    *si = 0;
  }
  x = sv[(*si)++];
  x ^= (x >> 29) & 0x5555555555555555ULL;
  x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
  x ^= (x << 37) & 0xFFF7EEE000000000ULL;
  x ^= (x >> 43);
  return x;
}

f32 carbon_math_abs(f32 x) {
  union { f32 f; u32 i; } u = {x};
  u.i &= 0x7fffffff;
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
    if (u.i >> 31) u.f = 0;
    else if (u.i << 1) u.f = 1;
  }
  return u.f;
}

f32 carbon_math_snap(f32 x, f32 dx) {
  f32 res = x + CARBON_SIGN(dx) * CARBON_EPS;
  if (dx > 0) return carbon_math_ceil(res);
  if (dx < 0) return carbon_math_floor(res);
  return x;
}

f32 carbon_math_sqrt(f32 x) {
  if (x == 2) return CARBON_SQRT2;
  if (x == 3) return CARBON_SQRT3;
  f32 s = x;
  for (usz i = 0; i < 1e3 && carbon_math_abs(s*s - x) > CARBON_EPS; ++i) {
    s -= (s*s - x) / (2*s);
  }
  return s;
}

i32 carbon_math_imod(i32 x, i32 y) {
  CBN_ASSERT(y && "division by 0 is not defined");
  return (x%y + y) % y;
}

f32 carbon_math_fmod(f32 x, f32 y) {
  CBN_ASSERT(y && "division by 0 is not defined");
  if (x != x || y != y) return x;
  f32 r = x - (i32) (x / y) * y;
  if ((r < 0 && y > 0) || (r > 0 && y < 0)) r += y;
  return r;
}

f32 carbon_math_pow(f32 x, f32 y) {
  if (y == 0) return 1;
  if (x == 0) {
    if (y < 0) CBN_ASSERT(0 && "division by 0 is not defined");
    return 0;
  }
  return carbon_math_exp(y * carbon_math_log(x));
}

f32 carbon_math_log2(f32 x) {
  static const f32 dinv = 6.4000000000e+1;
  static const f32 dsq6 = 4.0690105379e-5;
  __attribute__((aligned(16))) static const f32 q1[] = {
    0.0000000000e+0, 1.5504186973e-2, 3.0771657825e-2,
    4.5809537172e-2, 6.0624621809e-2, 7.5223423541e-2,
    8.9612156153e-2, 1.0379679501e-1, 1.1778303236e-1,
    1.3157635927e-1, 1.4518201351e-1, 1.5860502422e-1,
    1.7185026407e-1, 1.8492233753e-1, 1.9782574475e-1,
    2.1056476235e-1, 2.2314354777e-1, 2.3556606472e-1,
    2.4783615768e-1, 2.5995752215e-1, 2.7193370461e-1,
    2.8376817703e-1, 2.9546421766e-1, 3.0702504516e-1,
    3.1845372915e-1, 3.2975327969e-1, 3.4092658758e-1,
    3.5197642446e-1, 3.6290550232e-1, 3.7371641397e-1,
    3.8441169262e-1, 3.9499381185e-1, 4.0546509624e-1,
    4.1582790017e-1, 4.2608439922e-1, 4.3623676896e-1,
    4.4628709555e-1, 4.5623743534e-1, 4.6608972549e-1,
    4.7584590316e-1, 4.8550781608e-1, 4.9507725239e-1,
    5.0455600023e-1, 5.1394575834e-1, 5.2324813604e-1,
    5.3246480227e-1, 5.4159730673e-1, 5.5064713955e-1,
    5.5961579084e-1, 5.6850475073e-1, 5.7731539011e-1,
    5.8604902029e-1, 5.9470713139e-1, 6.0329085588e-1,
    6.1180156469e-1, 6.2024039030e-1, 6.2860864401e-1,
    6.3690745831e-1, 6.4513796568e-1, 6.5330129862e-1,
    6.6139847040e-1, 6.6943067312e-1, 6.7739880085e-1,
    6.8530398607e-1, 6.9314718246e-1
  };
  __attribute__((aligned(16))) static const f32 q2[] = {
    -9.9986231327e-1, -9.6937632561e-1, -9.4016802311e-1,
    -9.1239231825e-1, -8.8566547632e-1, -8.6031460762e-1,
    -8.3573484421e-1, -8.1255424023e-1, -7.8996384144e-1,
    -7.6855713129e-1, -7.4805134535e-1, -7.2788482904e-1,
    -7.0930588245e-1, -6.9062888622e-1, -6.7332005501e-1,
    -6.5613007545e-1, -6.4002591372e-1, -6.2422031164e-1,
    -6.0906451941e-1, -5.9467309713e-1, -5.8023625612e-1,
    -5.6704795361e-1, -5.5367153883e-1, -5.4126393795e-1,
    -5.2883124352e-1, -5.1699501276e-1, -5.0572794676e-1,
    -4.9451699853e-1, -4.8397746682e-1, -4.7362583876e-1,
    -4.6331596375e-1, -4.5411628485e-1, -4.4409587979e-1,
    -4.3551570177e-1, -4.2639014125e-1, -4.1800573468e-1,
    -4.0939944983e-1, -4.0167182684e-1, -3.9358118176e-1,
    -3.8606393337e-1, -3.7881350517e-1, -3.7138715386e-1,
    -3.6439764500e-1, -3.5803401470e-1, -3.5092726350e-1,
    -3.4470224380e-1, -3.3862310648e-1, -3.3254361153e-1,
    -3.2631972432e-1, -3.2067367435e-1, -3.1579026580e-1,
    -3.0874338746e-1, -3.0519321561e-1, -2.9860872030e-1,
    -2.9480555654e-1, -2.8877067566e-1, -2.8448674083e-1,
    -2.7982532978e-1, -2.7492287755e-1, -2.7136212587e-1,
    -2.6560515165e-1, -2.6289695501e-1, -2.5751405954e-1,
    -2.5392177701e-1, -2.5023636222e-1
  };
  union { f32 f; u32 u; i32 i; } val;
  val.f = x;
  i32 hx = val.i;
  i32 ipart = (((hx & 2139095040) >> 23) - 127);
  hx &= 8388607;
  val.i = hx | 1065353216;
  x = val.f;
  hx >>= 17;
  val.i = 1065353216 | (hx << 17);
  f32 b = (x - val.f) * dinv;
  f32 a = 1 - b;
  f32 y = a * q1[hx] + b * q1[hx + 1];
  a = (a*a*a-a) * q2[hx];
  b = (b*b*b-b) * q2[hx + 1];
  y += (a + b) * dsq6;
  return ((f32) ipart) + (y * CARBON_LOG2_E);
}

f32 carbon_math_exp2(f32 x) {
  __attribute__((aligned(16))) static const f32 p[] = {
    1.535336188319500e-4, 1.339887440266574e-3,
    9.618437357674640e-3, 5.550332471162809e-2,
    2.402264791363012e-1, 6.931472028550421e-1,
    1.000000000000000e+0
  };
  union { f32 f; u32 u; i32 i; } epart;
  f32 ipart = carbon_math_floor(x + 0.5);
  f32 fpart = x - ipart;
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

f32 carbon_math_ldexp2(f32 x, i32 n) {
  return x * (carbon_math_exp2(n));
}

f32 carbon_math_log(f32 x) {
  return carbon_math_log2(x) / CARBON_LOG2_E;
}

f32 carbon_math_exp(f32 x) {
  return carbon_math_exp2(CARBON_LOG2_E * x);
}

f32 carbon_math_ldexp(f32 x, i32 n) {
  return x * (carbon_math_exp(n));
}

f32 carbon_math_log10(f32 x) {
  return carbon_math_log2(x) / CARBON_LOG2_10;
}

f32 carbon_math_exp10(f32 x) {
  return carbon_math_exp2(CARBON_LOG2_10 * x);
}

f32 carbon_math_ldexp10(f32 x, i32 exp) {
  return x * (carbon_math_exp10(exp));
}

f32 carbon_math_frexp(f32 x, i32 *n) {
  union { f64 d; u64 i; } y = {x};
  i32 ee = y.i >> 52 & 0x7ff;
  if (!ee) {
    if (x) {
      x = carbon_math_frexp(x * 0x1p64, n);
      *n -= 64;
    }
    else *n = 0;
    return x;
  }
  else if (ee == 0x7ff) return x;
  *n = ee - 0x3fe;
  y.i &= 0x800fffffffffffffULL;
  y.i |= 0x3fe0000000000000ULL;
  return y.d;
}

f32 carbon_math_sigmoid(f32 x) {
  return 1 / (1 + carbon_math_exp(-x));
}

f32 carbon_math_smoothstep(f32 a, f32 b, f32 t) {
  CBN_ASSERT(a < b);
  f32 x = CARBON_CLAMP((t - a) / (b - a), 0, 1);
  return x * x * (3 - 2 * x);
}

i8 carbon_math_cmp(f32 x, f32 y) {
  if ((x - CARBON_EPS) < y)      return -1;
  else if ((x + CARBON_EPS) > y) return  1;
  else return 0;
}

u64 carbon_math_concat(u64 x, u64 y) {
  u64 n = 10;
  while (y >= n) n *= 10;
  return n * x + y;
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

f32 carbon_math_tan(f32 x) {
  f32 c = carbon_math_cos(x);
  if (carbon_math_abs(c) < CARBON_EPS) return x > 0 ? CARBON_INF : -CARBON_INF;
  return carbon_math_sin(x) / c;
}

f32 carbon_math_tanh(f32 x) {
  f32 ex = carbon_math_exp(x);
  f32 enx = carbon_math_exp(-x);
  return (ex - enx) / (ex + enx);
}

f32 carbon_math_asin(f32 x) {
  f32 eps = 1.1920928955078125e-07;
  f32 ys, yc, y = 0;
  for (;;) {
    ys = carbon_math_sin(y);
    yc = carbon_math_cos(y);
    if (-CARBON_PI_2 > y || y > CARBON_PI_2) y = carbon_math_fmod(y, CARBON_PI);
    if (ys + eps >= x && ys - eps <= x) break;
    y = y - (ys - x) / yc;
  }
  return y;
}

f32 carbon_math_atan(f32 x) {
  return carbon_math_asin(x / carbon_math_sqrt(x * x + 1));
}

f32 carbon_math_atan2(f32 y, f32 x) {
  f32 r, phi, y_abs = carbon_math_abs(y) + CARBON_EPS;
  if (x < 0) {
    r = (x + y_abs) / (y_abs - x);
    phi = 0.75 * CARBON_PI;
  }
  else {
    r = (x - y_abs) / (x + y_abs);
    phi = CARBON_PI_4;
  }
  phi += (0.1963 * r * r - 0.9817) * r;
  if (y < 0) return -phi;
  else return phi;
}

CBN_Matrix carbon_math_mat_create(usz rows, usz cols) {
  return (CBN_Matrix) {
    .items = (f32 *) carbon_memory_alloc(rows * cols * sizeof(f32)),
    .rows = rows,
    .cols = cols
  };
}

void carbon_math_mat_destroy(CBN_Matrix *m) {
  if (!m) {
    CBN_WARN("`m` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(m->items);
  carbon_memory_set(m, 0, sizeof(*m));
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
  CBN_ASSERT(dst.rows == src.rows);
  CBN_ASSERT(dst.cols == src.cols);
  for (usz i = 0; i < dst.rows; ++i) {
    for (usz j = 0; j < dst.cols; ++j) {
      CARBON_MAT_AT(dst, i, j) = CARBON_MAT_AT(src, i, j);
    }
  }
}

void carbon_math_mat_add(CBN_Matrix dst, CBN_Matrix m) {
  CBN_ASSERT(dst.rows == m.rows);
  CBN_ASSERT(dst.cols == m.cols);
  for (usz i = 0; i < dst.rows; ++i) {
    for (usz j = 0; j < dst.cols; ++j) {
      CARBON_MAT_AT(dst, i, j) += CARBON_MAT_AT(m, i, j);
    }
  }
}

void carbon_math_mat_dot(CBN_Matrix dst, CBN_Matrix a, CBN_Matrix b) {
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

void carbon_math_mat_map(CBN_Matrix m, f32 (*f)(f32)) {
  for (usz i = 0; i < m.rows; ++i) {
    for (usz j = 0; j < m.cols; ++j) {
      CARBON_MAT_AT(m, i, j) = f(CARBON_MAT_AT(m, i, j));
    }
  }
}

void carbon_math_mat_print(CBN_Matrix m, const char *name) {
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

CBN_Matrix carbon_math_row_to_mat(CBN_Row r) {
  return (CBN_Matrix) {
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
