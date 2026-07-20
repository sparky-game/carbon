// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

u32 carbon_math_ch32(u32 x, u32 y, u32 z) {
  return z ^ (x & (y ^ z));
}

u32 carbon_math_maj32(u32 x, u32 y, u32 z) {
  return (x & y) | (z & (x | y));
}

u32 carbon_math_rotl32(u32 x, u32 y) {
  return (x << (y & 31)) | (x >> ((32 - (y & 31)) & 31));
}

u32 carbon_math_rotr32(u32 x, u32 y) {
  return (x >> (y & 31)) | (x << ((32 - (y & 31)) & 31));
}

u64 carbon_math_rotl64(u64 x, u32 y) {
  return (x << (y & 63)) | (x >> ((64 - (y & 63)) & 63));
}

u64 carbon_math_rotr64(u64 x, u32 y) {
  return (x >> (y & 63)) | (x << ((64 - (y & 63)) & 63));
}

u32 carbon_math_bswap32(u32 x) {
#if defined(CARBON_CPU_ARCH_AMD64)
  __asm__("bswapl %0" : "=r"(x) : "0"(x));
  return x;
#elif defined(CARBON_CPU_ARCH_AARCH64)
  u32 res;
  __asm__("rev %w0, %w1" : "=r"(res) : "r"(x));
  return res;
#else
  return ((x & 0x000000ff) << 24)
    |    ((x & 0x0000ff00) <<  8)
    |    ((x & 0x00ff0000) >>  8)
    |    ((x & 0xff000000) >> 24);
#endif
}

f32 carbon_math_abs(f32 x) {
#if defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("fabs %s0, %s1" : "=w"(res) : "w"(x));
  return res;
#else
  union { f32 f; u32 i; } u = {x};
  u.i &= 0x7fffffff;
  return u.f;
#endif
}

f32 carbon_math_round(f32 x) {
#if defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("frinta %s0, %s1" : "=w"(res) : "w"(x));
  return res;
#else
  return x >= 0 ? (i32) (x + 0.5) : (i32) (x - 0.5);
#endif
}

f32 carbon_math_floor(f32 x) {
#if defined(CARBON_CPU_ARCH_AMD64)
  f32 res;
  __asm__("roundss $1, %1, %0" : "=x"(res) : "x"(x));
  return res;
#elif defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("frintm %s0, %s1" : "=w"(res) : "w"(x));
  return res;
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
#if defined(CARBON_CPU_ARCH_AMD64)
  f32 res;
  __asm__("roundss $2, %1, %0" : "=x"(res) : "x"(x));
  return res;
#elif defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("frintp %s0, %s1" : "=w"(res) : "w"(x));
  return res;
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

f32 carbon_math_min(f32 x, f32 y) {
#if defined(CARBON_CPU_ARCH_AMD64)
  f32 res;
  __asm__("minss %2, %0" : "=x"(res) : "0"(x), "x"(y));
  return res;
#elif defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("fmin %s0, %s1, %s2" : "=w"(res) : "w"(x), "w"(y));
  return res;
#else
  return x < y ? x : y;
#endif
}

f32 carbon_math_min_3(f32 x, f32 y, f32 z) {
  return carbon_math_min(x, carbon_math_min(y, z));
}

f32 carbon_math_max(f32 x, f32 y) {
#if defined(CARBON_CPU_ARCH_AMD64)
  f32 res;
  __asm__("maxss %2, %0" : "=x"(res) : "0"(x), "x"(y));
  return res;
#elif defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("fmax %s0, %s1, %s2" : "=w"(res) : "w"(x), "w"(y));
  return res;
#else
  return x > y ? x : y;
#endif
}

f32 carbon_math_max_3(f32 x, f32 y, f32 z) {
  return carbon_math_max(x, carbon_math_max(y, z));
}

f32 carbon_math_clamp(f32 x, f32 min, f32 max) {
  return carbon_math_min(carbon_math_max(x, min), max);
}

f32 carbon_math_snap(f32 x, f32 dx) {
  f32 res = x + CARBON_SIGN(dx) * CARBON_EPS;
  if (dx > 0) return carbon_math_ceil(res);
  if (dx < 0) return carbon_math_floor(res);
  return x;
}

f32 carbon_math_sqrt(f32 x) {
#if defined(CARBON_CPU_ARCH_AMD64)
  f32 res;
  __asm__("sqrtss %1, %0" : "=x"(res) : "x"(x));
  return res;
#elif defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("fsqrt %s0, %s1" : "=w"(res) : "w"(x));
  return res;
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
#if defined(CARBON_CPU_ARCH_AMD64)
  f32 res;
  __asm__("rsqrtss %1, %0" : "=x"(res) : "x"(x));
  return res;
#elif defined(CARBON_CPU_ARCH_AARCH64)
  f32 res;
  __asm__("frsqrte %s0, %s1" : "=w"(res) : "w"(x));
  return res;
#else
  // Fast Inverse Square Root algorithm
  f32 x2 = x/2, y = x;
  i32 i = *((i32 *) &y);      // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1);  // what the fuck?
  y = *((f32 *) &i);
  y *= (1.5 - (x2 * y * y));
  return y;
#endif
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

CBNINL i32 carbon_math__checkint(u32 iy) {
  // Returns 0 if not int, 1 if odd int, 2 if even int.
  // The argument is the bit representation of a non-zero finite f32 value.
  i32 e = iy >> 23 & 0xff;
  if (e < 0x7f) return 0;
  if (e > 0x7f + 23) return 2;
  if (iy & ((1 << (0x7f + 23 - e)) - 1)) return 0;
  if (iy & (1 << (0x7f + 23 - e))) return 1;
  return 2;
}

f32 carbon_math_pow(f32 x, f32 y) {
  if (y == 0) return 1;
  if (x != x || y != y) return x + y;
  if (x == 0) {
    if (y < 0) CBN_ASSERT(0 && "division by 0 is not defined");
    return 0;
  }
  if (x < 0) {
    i32 ycheck = carbon_math__checkint(*(u32 *)&y);
    if (ycheck == 0) CBN_ASSERT(0 && "negative base with non-int exp is not defined");
    f32 r = carbon_math_exp(y * carbon_math_log(-x));
    return ycheck == 1 ? -r : r;
  }
  return carbon_math_exp(y * carbon_math_log(x));
}

f32 carbon_math_log2(f32 x) {
  static const f32 dinv = 6.4000000000e+1;
  static const f32 dsq6 = 4.0690105379e-5;
  alignas(16) static const f32 q1[] = {
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
  alignas(16) static const f32 q2[] = {
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
  alignas(16) static const f32 p[] = {
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
  union { f32 f; u32 i; } y = {x};
  i32 ee = y.i >> 23 & 0xff;
  if (!ee) {
    if (x) {
      x = carbon_math_frexp(x * 0x1p64, n);
      *n -= 64;
    }
    else *n = 0;
    return x;
  }
  if (ee == 0xff) return x;
  *n = ee - 0x7e;
  y.i &= 0x807fffffU;
  y.i |= 0x3f000000U;
  return y.f;
}

f32 carbon_math_sigmoid(f32 x) {
  return 1 / (1 + carbon_math_exp(-x));
}

f32 carbon_math_lerp(f32 a, f32 b, f32 t) {
  return a + (b - a) * t;
}

f32 carbon_math_smoothstep(f32 a, f32 b, f32 x) {
  CBN_ASSERT(a < b);
  f32 t = carbon_math_clamp((x - a)/(b - a), 0, 1);
  return 3*t*t - 2*t*t*t;
}

i8 carbon_math_cmp(f32 x, f32 y) {
  if      ((x - CARBON_EPS) < y) return -1;
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
  return carbon_math_cos(x - CARBON_PI_2);
}

f32 carbon_math_cos(f32 x) {
  x = carbon_math_abs(x);
  x -= CARBON_2PI * carbon_math_floor(x/CARBON_2PI);
  if (x >= CARBON_PI) {
    x -= CARBON_PI;
    return -(1-(x*x/2)*(1-(x*x/12)*(1-(x*x/30)*(1-(x*x/56)*(1-(x*x/90)*(1-(x*x/132)*(1-(x*x/182))))))));
  }
  return 1-(x*x/2)*(1-(x*x/12)*(1-(x*x/30)*(1-(x*x/56)*(1-(x*x/90)*(1-(x*x/132)*(1-(x*x/182)))))));
}

f32 carbon_math_tan(f32 x) {
  f32 c = carbon_math_cos(x);
  if (carbon_math_abs(c) < CARBON_EPS) return x > 0 ? CARBON_F32_INF : -CARBON_F32_INF;
  return carbon_math_sin(x) / c;
}

f32 carbon_math_tanh(f32 x) {
  if (x >  9) return  1;
  if (x < -9) return -1;
  f32 e2x = carbon_math_exp(2*x);
  return (e2x - 1)/(e2x + 1);
}

f32 carbon_math_asin(f32 x) {
  if (x >  1) x =  1;
  if (x < -1) x = -1;
  return carbon_math_atan(x / carbon_math_sqrt(1 - x*x));
}

f32 carbon_math_acos(f32 x) {
  return CARBON_PI_2 - carbon_math_asin(x);
}

f32 carbon_math_atan(f32 x) {
  bool flip = x < 0;
  if (flip) x = -x;
  bool inv = x > 1;
  if (inv) x = 1/x;
  bool half = x > CARBON_SQRT2 - 1;
  if (half) x = (x - 1)/(x + 1);
  x *= (1 - x*x*(1/3. - x*x*(1/5. - x*x*(1/7. - x*x*(1/9. - x*x*(1/11. - x*x*(1/13.)))))));
  if (half) x += CARBON_PI_4;
  if (inv) x = CARBON_PI_2 - x;
  return flip ? -x : x;
}

f32 carbon_math_atan2(f32 y, f32 x) {
  if (x == 0) {
    if (y > 0) return  CARBON_PI_2;
    if (y < 0) return -CARBON_PI_2;
    return 0;
  }
  f32 r, phi, y_abs = carbon_math_abs(y);
  if (x < 0) {
    r = (x + y_abs)/(y_abs - x);
    phi = 0.75 * CARBON_PI;
  }
  else {
    r = (x - y_abs)/(x + y_abs);
    phi = CARBON_PI_4;
  }
  phi += (0.1963*r*r - 0.9817) * r;
  return y < 0 ? -phi : phi;
}

u32 carbon_math_gf2_mat_mult_vec(const u32 *m, u32 v, usz N) {
  u32 s = 0;
  for (usz i = 0; i < N && v; ++i, v >>= 1) {
    if (v & 1) s ^= m[i];
  }
  return s;
}

void carbon_math_gf2_mat_sq(u32 *dest, const u32 *src, usz N) {
  for (usz i = 0; i < N; ++i) {
    dest[i] = carbon_math_gf2_mat_mult_vec(src, src[i], N);
  }
}
