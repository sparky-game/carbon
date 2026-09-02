// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef CARBON_SIMD_INTRINSICS

CBNINL void carbon_memory_set__simd(u8 *d, u8 v, usz n) {
#if defined(CARBON_SIMD_HAS_AVX) || defined(CARBON_SIMD_HAS_AVX2)
  if (n < 32) {
    while (n--) *d++ = v;
    return;
  }
  u8x32 fill = _mm256_set1_epi8((char)v);
  usz misalign = (usz)d & 31;
  if (misalign) {
    usz head = 32 - misalign;
    _mm256_storeu_si256((u8x32 *)d, fill);
    d += head;
    n -= head;
  }
  while (n >= 128) {
    _mm256_store_si256((u8x32 *)(d +  0), fill);
    _mm256_store_si256((u8x32 *)(d + 32), fill);
    _mm256_store_si256((u8x32 *)(d + 64), fill);
    _mm256_store_si256((u8x32 *)(d + 96), fill);
    d += 128;
    n -= 128;
  }
  while (n >= 32) {
    _mm256_store_si256((u8x32 *)d, fill);
    d += 32;
    n -= 32;
  }
  if (n) _mm256_storeu_si256((u8x32 *)(d + n - 32), fill);
#elif defined(CARBON_SIMD_HAS_SSE2) || defined(CARBON_SIMD_HAS_SSE3) || defined(CARBON_SIMD_HAS_SSSE3) || defined(CARBON_SIMD_HAS_SSE41) || defined(CARBON_SIMD_HAS_SSE42)
  if (n < 16) {
    while (n--) *d++ = v;
    return;
  }
  u8x16 fill = _mm_set1_epi8((char)v);
  usz misalign = (usz)d & 15;
  if (misalign) {
    usz head = 16 - misalign;
    _mm_storeu_si128((u8x16 *)d, fill);
    d += head;
    n -= head;
  }
  while (n >= 64) {
    _mm_store_si128((u8x16 *)(d +  0), fill);
    _mm_store_si128((u8x16 *)(d + 16), fill);
    _mm_store_si128((u8x16 *)(d + 32), fill);
    _mm_store_si128((u8x16 *)(d + 48), fill);
    d += 64;
    n -= 64;
  }
  while (n >= 16) {
    _mm_store_si128((u8x16 *)d, fill);
    d += 16;
    n -= 16;
  }
  if (n) _mm_storeu_si128((u8x16 *)(d + n - 16), fill);
#elif defined(CARBON_SIMD_HAS_NEON) || defined(CARBON_SIMD_HAS_NEON_FMA)
  if (n < 16) {
    while (n--) *d++ = v;
    return;
  }
  u8x16 fill = vdupq_n_u8(v);
  while (n >= 64) {
    vst1q_u8(d +  0, fill);
    vst1q_u8(d + 16, fill);
    vst1q_u8(d + 32, fill);
    vst1q_u8(d + 48, fill);
    d += 64;
    n -= 64;
  }
  while (n >= 16) {
    vst1q_u8(d, fill);
    d += 16;
    n -= 16;
  }
  if (n) vst1q_u8(d + n - 16, fill);
#else
#error No SIMD support for `memory_set` operation
#endif
}

#endif

// Local Variables:
// mode: c
// End:
