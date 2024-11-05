// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

// TODO: replace with own implementation
CARBON_API f32 expf(f32);
CARBON_API f32 sinf(f32);
CARBON_API f32 cosf(f32);

f32 carbon_math_abs(f32 x) {
  union { f32 f; u32 i; } u = {x};
  u.i &= 0x7fffffff;
  return u.f;
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

CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle) {
  f32 rads = angle * (CARBON_PI / 180);
  f32 c = cosf(rads), s = sinf(rads);
  return (CBN_Vec2) {
    .x = (v.x * c) - (v.y * s),
    .y = (v.x * s) + (v.y * c)
  };
}
