// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

CBNINL u32 carbon_rng__seed(void) {
  f64 t = carbon_time_get();
  union { f64 f; u64 i; } u = {t};
  return (u32)(u.i ^ (u.i >> 32));
}

u32 carbon_rng_lcg(void) {
  static bool has_seed = false;
  static u32 x;
  if (!has_seed) {
    x = carbon_rng__seed();
    has_seed = true;
  }
  const u32 a = 0x19660d, c = 0x3c6ef35f;
  x = a*x + c;
  return x;
}

u32 carbon_rng_lcg_range(u32 min, u32 max) {
  if (min == max) return min;
  u64 range_sz = (u64)max - (u64)min + 1;
  u32 rand = carbon_rng_lcg();
  u32 offset = (u32)(((u64)rand * range_sz) >> 32);
  return min + offset;
}

f32 carbon_rng_lcgf(void) {
  return (f32)carbon_rng_lcg() * 0x1p-32f;
}

f32 carbon_rng_lcgf_range(f32 min, f32 max) {
  return carbon_rng_lcgf() * (max - min) + min;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define CARBON_RNG__MT19937_64_RAND_NN 312
#define CARBON_RNG__MT19937_64_RAND_MM 156
#define CARBON_RNG__MT19937_64_RAND_UM 0xFFFFFFFF80000000ULL
#define CARBON_RNG__MT19937_64_RAND_LM 0x7FFFFFFFULL

static u64 carbon_rng__mt19937_64_rand_state_vec[CARBON_RNG__MT19937_64_RAND_NN];
static u64 carbon_rng__mt19937_64_rand_state_idx = CARBON_RNG__MT19937_64_RAND_NN + 1;

void carbon_rng_mt19937_64_srand(u64 seed) {
  u64 *sv = carbon_rng__mt19937_64_rand_state_vec;
  u64 *si = &carbon_rng__mt19937_64_rand_state_idx;
  sv[0] = seed;
  for (*si = 1; *si < CARBON_RNG__MT19937_64_RAND_NN; ++(*si)) {
    sv[*si] = (6364136223846793005ULL * (sv[*si - 1] ^ (sv[*si - 1] >> 62)) + *si);
  }
}

u64 carbon_rng_mt19937_64_rand(void) {
  // NOTE: default seed was `5489ULL` and was changed to `carbon_time_get()` in ms
  static u64 mag[] = {0ULL, 0xB5026F5AA96619E9ULL};
  i32 i; u64 x;
  u64 *sv = carbon_rng__mt19937_64_rand_state_vec;
  u64 *si = &carbon_rng__mt19937_64_rand_state_idx;
  if (*si >= CARBON_RNG__MT19937_64_RAND_NN) {
    if (*si == CARBON_RNG__MT19937_64_RAND_NN + 1) carbon_rng_mt19937_64_srand((u64)(carbon_time_get() * 1e6));
    for (i = 0; i < CARBON_RNG__MT19937_64_RAND_NN - CARBON_RNG__MT19937_64_RAND_MM; ++i) {
      x = (sv[i] & CARBON_RNG__MT19937_64_RAND_UM) | (sv[i + 1] & CARBON_RNG__MT19937_64_RAND_LM);
      sv[i] = sv[i + CARBON_RNG__MT19937_64_RAND_MM] ^ (x >> 1) ^ mag[(i32) (x & 1ULL)];
    }
    for (; i < CARBON_RNG__MT19937_64_RAND_NN - 1; ++i) {
      x = (sv[i] & CARBON_RNG__MT19937_64_RAND_UM) | (sv[i + 1] & CARBON_RNG__MT19937_64_RAND_LM);
      sv[i] = sv[i + (CARBON_RNG__MT19937_64_RAND_MM - CARBON_RNG__MT19937_64_RAND_NN)] ^ (x >> 1) ^ mag[(i32) (x & 1ULL)];
    }
    x = (sv[CARBON_RNG__MT19937_64_RAND_NN - 1] & CARBON_RNG__MT19937_64_RAND_UM) | (sv[0] & CARBON_RNG__MT19937_64_RAND_LM);
    sv[CARBON_RNG__MT19937_64_RAND_NN - 1] = sv[CARBON_RNG__MT19937_64_RAND_MM - 1] ^ (x >> 1) ^ mag[(i32) (x & 1ULL)];
    *si = 0;
  }
  x = sv[(*si)++];
  x ^= (x >> 29) & 0x5555555555555555ULL;
  x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
  x ^= (x << 37) & 0xFFF7EEE000000000ULL;
  x ^= (x >> 43);
  return x;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

bool carbon_rng_bernoulli_dist(f32(*gen)(void), f32 p) {
  return gen() < p;
}

f32 carbon_rng_normal_dist(f32(*gen)(void), f32 mu, f32 sigma) {
  static bool cached = false;
  static f32 z1;
  if (cached) {
    cached = false;
    return mu + z1*sigma;
  }
  f32 u1 = gen(), u2 = gen();
  if (u1 <= 0) u1 = CARBON_EPS;
  f32 R = carbon_math_sqrt(-2 * carbon_math_log(u1));
  f32 phi = CARBON_2PI * u2;
  f32 z0 = R * carbon_math_cos(phi);
  z1 = R * carbon_math_sin(phi);
  cached = true;
  return mu + z0*sigma;
}

CBNINL i32 carbon_rng__ascending_order(const void *a, const void *b) {
  const i32 x = *(const i32 *) a;
  const i32 y = *(const i32 *) b;
  return (x > y) - (x < y);
}

CBNINL i32 carbon_rng__descending_order(const void *a, const void *b) {
  const i32 x = *(const i32 *) a;
  const i32 y = *(const i32 *) b;
  return (y > x) - (y < x);
}

u32 carbon_rng_roll_dice(const char *expr) {
  const char *p = expr;
  u32 n, s, x;
  i32 cs;
  if (2 > sscanf(p, "%ud%u%n", &n, &s, &cs)) {
    CBN_ERROR("expr (%s) is not valid, must be in {N}d{S} format", p);
    return 0;
  }
  p += cs;
  CBN_Sort_CmpFunc f_cmp = 0;
  if (!*p) x = n;
  else if (!carbon_string_cmp_n(p, "kl", 2)) {
    if (1 != sscanf(p + 2, "%u", &x)) {
      CBN_ERROR("missing keep number in suffix");
      return 0;
    }
    f_cmp = carbon_rng__ascending_order;
  }
  else if (!carbon_string_cmp_n(p, "kh", 2)) {
    if (1 != sscanf(p + 2, "%u", &x)) {
      CBN_ERROR("missing keep number in suffix");
      return 0;
    }
    f_cmp = carbon_rng__descending_order;
  }
  else {
    CBN_ERROR("unrecognized suffix");
    return 0;
  }
  if (x > n) {
    CBN_ERROR("can't keep %u dice when only %u were rolled", x, n);
    return 0;
  }
  if (s != 4 && s != 6 && s != 8 && s != 10 && s != 12 && s != 20 && s != 100) {
    CBN_ERROR("invalid die size d%u", s);
    return 0;
  }
  u32 rolls[256] = {0};
  for (usz i = 0; i < n; ++i) rolls[i] = carbon_rng_lcg_range(1, s);
  if (x < n) carbon_sort_insertion(rolls, n, sizeof(u32), f_cmp);
  u32 sum = 0;
  for (usz i = 0; i < x; ++i) sum += rolls[i];
  return sum;
}
