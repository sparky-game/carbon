// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef CARBON_SIMD_INTRINSICS

CBNINL CBN_Vec4 carbon_math_mat4_mult_vec4__simd(CBN_Mat4 m, CBN_Vec4 v) {
#if defined(CARBON_SIMD_HAS_AVX) || defined(CARBON_SIMD_HAS_AVX2)
  f32x8 r01 = _mm256_load_ps(&m.m[0][0]);
  f32x8 r23 = _mm256_load_ps(&m.m[2][0]);
  f32x8 v_bc = _mm256_set_ps(v.w, v.w, v.w, v.w,
                             v.z, v.z, v.z, v.z,
                             v.y, v.y, v.y, v.y,
                             v.x, v.x, v.x, v.x);
  f32x8 m01 = _mm256_mul_ps(r01, v_bs);
  f32x8 m23 = _mm256_mul_ps(r23, v_bs);
  f32x4 s01 = _mm_hadd_ps(_mm256_castps256_ps128(m01), _mm256_extractf128_ps(m01, 1));
  f32x4 s23 = _mm_hadd_ps(_mm256_castps256_ps128(m23), _mm256_extractf128_ps(m23, 1));
  f32x4 res = _mm_hadd_ps(s01, s23);
  CBN_Vec4 r;
  _mm_store_ps(r.c, res);
  return r;
#elif defined(CARBON_SIMD_HAS_SSE3) || defined(CARBON_SIMD_HAS_SSSE3) || defined(CARBON_SIMD_HAS_SSE41) || defined(CARBON_SIMD_HAS_SSE42)
  f32x4 vec = _mm_load_ps(v.c);
  f32x4 r0 = _mm_load_ps(m.m[0]);
  f32x4 r1 = _mm_load_ps(m.m[1]);
  f32x4 r2 = _mm_load_ps(m.m[2]);
  f32x4 r3 = _mm_load_ps(m.m[3]);
  f32x4 m0 = _mm_mul_ps(vec, r0);
  f32x4 m1 = _mm_mul_ps(vec, r1);
  f32x4 m2 = _mm_mul_ps(vec, r2);
  f32x4 m3 = _mm_mul_ps(vec, r3);
  m0 = _mm_hadd_ps(m0, m1);
  m2 = _mm_hadd_ps(m2, m3);
  m0 = _mm_hadd_ps(m0, m2);
  CBN_Vec4 r;
  _mm_store_ps(r.c, m0);
  return r;
#elif defined(CARBON_SIMD_HAS_SSE2)
  f32x4 vec = _mm_load_ps(v.c);
  f32x4 r0 = _mm_load_ps(m.m[0]);
  f32x4 r1 = _mm_load_ps(m.m[1]);
  f32x4 r2 = _mm_load_ps(m.m[2]);
  f32x4 r3 = _mm_load_ps(m.m[3]);
  f32x4 m0 = _mm_mul_ps(vec, r0);
  f32x4 m1 = _mm_mul_ps(vec, r1);
  f32x4 m2 = _mm_mul_ps(vec, r2);
  f32x4 m3 = _mm_mul_ps(vec, r3);
  f32x4 shuf, sums;
  shuf = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(2, 3, 0, 1));
  sums = _mm_add_ps(m0, shuf);
  shuf = _mm_movehl_ps(shuf, sums);
  sums = _mm_add_ss(sums, shuf);
  f32 d0 = _mm_cvtss_f32(sums);
  shuf = _mm_shuffle_ps(m1, m1, _MM_SHUFFLE(2, 3, 0, 1));
  sums = _mm_add_ps(m1, shuf);
  shuf = _mm_movehl_ps(shuf, sums);
  sums = _mm_add_ss(sums, shuf);
  f32 d1 = _mm_cvtss_f32(sums);
  shuf = _mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2, 3, 0, 1));
  sums = _mm_add_ps(m2, shuf);
  shuf = _mm_movehl_ps(shuf, sums);
  sums = _mm_add_ss(sums, shuf);
  f32 d2 = _mm_cvtss_f32(sums);
  shuf = _mm_shuffle_ps(m3, m3, _MM_SHUFFLE(2, 3, 0, 1));
  sums = _mm_add_ps(m3, shuf);
  shuf = _mm_movehl_ps(shuf, sums);
  sums = _mm_add_ss(sums, shuf);
  f32 d3 = _mm_cvtss_f32(sums);
  return carbon_math_vec4(d0, d1, d2, d3);
#elif defined(CARBON_SIMD_HAS_NEON) || defined(CARBON_SIMD_HAS_NEON_FMA)
  f32x4 vec = vld1q_f32(v.c);
  f32x4 r0 = vld1q_f32(m.m[0]);
  f32x4 r1 = vld1q_f32(m.m[1]);
  f32x4 r2 = vld1q_f32(m.m[2]);
  f32x4 r3 = vld1q_f32(m.m[3]);
  f32x4 m0 = vmulq_f32(vec, r0);
  f32x4 m1 = vmulq_f32(vec, r0);
  f32x4 m2 = vmulq_f32(vec, r0);
  f32x4 m3 = vmulq_f32(vec, r0);
  f32x2 m01l = vadd_f32(vget_low_f32(m0), vget_high_f32(m0));
  f32x2 m01h = vadd_f32(vget_low_f32(m1), vget_high_f32(m1));
  f32x2 m23l = vadd_f32(vget_low_f32(m2), vget_high_f32(m2));
  f32x2 m23h = vadd_f32(vget_low_f32(m3), vget_high_f32(m3));
  f32x2 s01 = vpadd_f32(m01l, m01h);
  f32x2 s23 = vpadd_f32(m23l, m23h);
  f32x4 res = vcombine_f32(s01, s23);
  CBN_Vec4 r;
  vst1q_f32(r.c, res);
  return r;
#else
#error "No SIMD support for `mat4_mult_vec4` operation"
#endif
}

#endif

// Local Variables:
// mode: c
// End:
