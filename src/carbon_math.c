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
    sv[*si] = (CARBON_MATH__RAND_PCG_MAGIC * (sv[*si - 1] ^ (sv[*si - 1] >> 62)) + *si);
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

u32 carbon_math_bswap32(u32 x) {
#if CARBON_HAS_BUILTIN(__builtin_bswap32)
  return __builtin_bswap32(x);
#else
  return ((x & 0x000000ff) << 24) | ((x & 0x0000ff00) << 8) | ((x & 0x00ff0000) >> 8) | ((x & 0xff000000) >> 24);
#endif
}

f32 carbon_math_abs(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_fabsf)
  return __builtin_fabsf(x);
#else
  union { f32 f; u32 i; } u = {x};
  u.i &= 0x7fffffff;
  return u.f;
#endif
}

f32 carbon_math_round(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_roundf)
  return __builtin_roundf(x);
#else
  return x >= 0 ? (i32) (x + 0.5) : (i32) (x - 0.5);
#endif
}

f32 carbon_math_floor(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_floorf)
  return __builtin_floorf(x);
#else
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
#endif
}

f32 carbon_math_ceil(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_ceilf)
  return __builtin_ceilf(x);
#else
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
#endif
}

f32 carbon_math_snap(f32 x, f32 dx) {
  f32 res = x + CARBON_SIGN(dx) * CARBON_EPS;
  if (dx > 0) return carbon_math_ceil(res);
  if (dx < 0) return carbon_math_floor(res);
  return x;
}

f32 carbon_math_sqrt(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_sqrtf)
  return __builtin_sqrtf(x);
#else
  if (!x || x < 0) return 0;
  if (x == 0.5) return CARBON_1_SQRT2;
  if (x == 0.75) return CARBON_SQRT3_2;
  if (x == 1) return 1;
  if (x == 2) return CARBON_SQRT2;
  if (x == 3) return CARBON_SQRT3;
  f32 s = x > 1 ? x : 1;
  for (usz i = 0; i < 100; ++i) {
    f32 prev_s = s;
    s = (s + x / s) / 2;
    if (carbon_math_abs(s - prev_s) < CARBON_EPS * carbon_math_abs(s)) break;
  }
  return s;
#endif
}

f32 carbon_math_rsqrt(f32 x) {
  // Fast Inverse Square Root algorithm
  f32 x2 = x/2, y = x;
  i32 i = *((i32 *) &y);      // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1);  // what the fuck?
  y = *((f32 *) &i);
  y *= (1.5 - (x2 * y * y));
  return y;
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
#if CARBON_HAS_BUILTIN(__builtin_powf)
  return __builtin_powf(x, y);
#else
  if (y == 0) return 1;
  if (x == 0) {
    if (y < 0) CBN_ASSERT(0 && "division by 0 is not defined");
    return 0;
  }
  return carbon_math_exp(y * carbon_math_log(x));
#endif
}

f32 carbon_math_log2(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_log2f)
  return __builtin_log2f(x);
#else
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
#endif
}

f32 carbon_math_exp2(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_exp2f)
  return __builtin_exp2f(x);
#else
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
#endif
}

f32 carbon_math_ldexp2(f32 x, i32 n) {
#if CARBON_HAS_BUILTIN(__builtin_ldexpf)
  return __builtin_ldexpf(x, n);
#else
  return x * (carbon_math_exp2(n));
#endif
}

f32 carbon_math_log(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_logf)
  return __builtin_logf(x);
#else
  return carbon_math_log2(x) / CARBON_LOG2_E;
#endif
}

f32 carbon_math_exp(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_expf)
  return __builtin_expf(x);
#else
  return carbon_math_exp2(CARBON_LOG2_E * x);
#endif
}

f32 carbon_math_ldexp(f32 x, i32 n) {
  return x * (carbon_math_exp(n));
}

f32 carbon_math_log10(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_log10f)
  return __builtin_log10f(x);
#else
  return carbon_math_log2(x) / CARBON_LOG2_10;
#endif
}

f32 carbon_math_exp10(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_exp10f)
  return __builtin_exp10f(x);
#else
  return carbon_math_exp2(CARBON_LOG2_10 * x);
#endif
}

f32 carbon_math_ldexp10(f32 x, i32 exp) {
  return x * (carbon_math_exp10(exp));
}

f32 carbon_math_frexp(f32 x, i32 *n) {
#if CARBON_HAS_BUILTIN(__builtin_frexpf)
  return __builtin_frexpf(x, n);
#else
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
#endif
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

i32 carbon_math_egcd(i32 x, i32 y) {
  if (!x) return y;
  if (!y || x == y) return x;
  if (x > y) return carbon_math_egcd(x - y, y);
  return carbon_math_egcd(x, y - x);
}

f32 carbon_math_sin(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_sinf)
  return __builtin_sinf(x);
#else
  return carbon_math_cos(x - CARBON_PI_2);
#endif
}

f32 carbon_math_cos(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_cosf)
  return __builtin_cosf(x);
#else
  if (x < 0) x = -x;
  if (0 <= carbon_math_cmp(x, CARBON_2PI)) {
    do { x -= CARBON_2PI; } while (0 <= carbon_math_cmp(x, CARBON_2PI));
  }
  if ((0 <= carbon_math_cmp(x, CARBON_PI)) && (-1 == carbon_math_cmp(x, CARBON_2PI))) {
    x -= CARBON_PI;
    return ((-1)*(1-(x*x/2)*(1-(x*x/12)*(1-(x*x/30)*(1-(x*x/56)*(1-(x*x/90)*(1-(x*x/132)*(1-(x*x/182)))))))));
  }
  return 1-(x*x/2)*(1-(x*x/12)*(1-(x*x/30)*(1-(x*x/56)*(1-(x*x/90)*(1-(x*x/132)*(1-(x*x/182)))))));
#endif
}

f32 carbon_math_tan(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_tanf)
  return __builtin_tanf(x);
#else
  f32 c = carbon_math_cos(x);
  if (carbon_math_abs(c) < CARBON_EPS) return x > 0 ? CARBON_INF : -CARBON_INF;
  return carbon_math_sin(x) / c;
#endif
}

f32 carbon_math_tanh(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_tanhf)
  return __builtin_tanhf(x);
#else
  f32 ex = carbon_math_exp(x);
  f32 enx = carbon_math_exp(-x);
  return (ex - enx) / (ex + enx);
#endif
}

f32 carbon_math_asin(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_asinf)
  return __builtin_asinf(x);
#else
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
#endif
}

f32 carbon_math_atan(f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_atanf)
  return __builtin_atanf(x);
#else
  return carbon_math_asin(x / carbon_math_sqrt(x * x + 1));
#endif
}

f32 carbon_math_atan2(f32 y, f32 x) {
#if CARBON_HAS_BUILTIN(__builtin_atan2f)
  return __builtin_atan2f(y, x);
#else
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
#endif
}
