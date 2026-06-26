/*
  $$===================$$
  ||       Types       ||
  $$===================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef void u0;  // Maybe we're all just birds staring at computer monitors.
#ifdef __cplusplus
namespace cbn::limits {
  template <typename T> consteval T min() = delete;
  template <typename T> consteval T max() = delete;
  template <typename T> consteval T eps() = delete;
  template <typename T> consteval T inf() = delete;
}
#endif

typedef unsigned char u8;
typedef signed char i8;
static_assert(sizeof(u8) == 1 && sizeof(i8) == 1, "Expected u8/i8 to be 1 byte");

typedef unsigned short u16;
typedef signed short i16;
static_assert(sizeof(u16) == 2 && sizeof(i16) == 2, "Expected u16/i16 to be 2 bytes");

typedef signed int i32;
static_assert(sizeof(i32) == 4, "Expected i32 to be 4 bytes");
#define CARBON_I32_MIN (-1 - CARBON_I32_MAX)
#define CARBON_I32_MAX 0x7fffffff
#define CARBON_I32_EPS 0
#define CARBON_I32_INF 0
#ifdef __cplusplus
namespace cbn::limits {
  template <> consteval i32 min<i32>() { return CARBON_I32_MIN; }
  template <> consteval i32 max<i32>() { return CARBON_I32_MAX; }
  template <> consteval i32 eps<i32>() { return CARBON_I32_EPS; }
  template <> consteval i32 inf<i32>() { return CARBON_I32_INF; }
}
#endif

typedef unsigned int u32;
static_assert(sizeof(u32) == 4, "Expected u32 to be 4 bytes");
#define CARBON_U32_MIN 0
#define CARBON_U32_MAX 0xffffffff
#define CARBON_U32_EPS 0
#define CARBON_U32_INF 0
#ifdef __cplusplus
namespace cbn::limits {
  template <> consteval u32 min<u32>() { return CARBON_U32_MIN; }
  template <> consteval u32 max<u32>() { return CARBON_U32_MAX; }
  template <> consteval u32 eps<u32>() { return CARBON_U32_EPS; }
  template <> consteval u32 inf<u32>() { return CARBON_U32_INF; }
}
#endif

typedef unsigned long long u64;
typedef signed long long i64;
static_assert(sizeof(u64) == 8 && sizeof(i64) == 8, "Expected u64/i64 to be 8 bytes");

typedef float f32;
static_assert(sizeof(f32) == 4, "Expected f32 to be 4 bytes");

typedef double f64;
static_assert(sizeof(f64) == 8, "Expected f64 to be 8 bytes");

typedef long double flong;
static_assert(sizeof(flong) >= sizeof(f64), "Expected flong to be at least 8 bytes");

typedef typeof(sizeof(0)) usz;
static_assert(sizeof(usz) >= 2, "Expected usz to be at least 2 bytes");

typedef ptrdiff_t isz;
typedef intptr_t iptr;
typedef uintptr_t uptr;

// TODO: consider adding a string type... maybe it's useful and pretty.
// typedef char * str;
// typedef const char * cstr;

#if defined(CARBON_SIMD_HAS_AVX) || defined(CARBON_SIMD_HAS_AVX2) || defined(CARBON_SIMD_HAS_FMA_AVX) || defined(CARBON_SIMD_HAS_FMA_AVX2)
typedef __m256 f32x8;
#endif
#if defined(CARBON_SIMD_HAS_SSE2) || defined(CARBON_SIMD_HAS_SSE3) || defined(CARBON_SIMD_HAS_SSSE3) || defined(CARBON_SIMD_HAS_SSE41) || defined(CARBON_SIMD_HAS_SSE42)
typedef __m128 f32x4;
#endif
#if defined(CARBON_SIMD_HAS_NEON) || defined(CARBON_SIMD_HAS_NEON_FMA)
typedef float32x2_t f32x2;
typedef float32x4_t f32x4;
#endif

#ifdef __cplusplus
namespace cbn {
  template <typename T, typename U = T>
  using Pair = std::pair<T, U>;
  template <typename... Ts>
  using Tuple = std::tuple<Ts...>;
  template <typename T, usz N>
  using Array = std::array<T, N>;
  template <typename T>
  using Opt = std::optional<T>;
  template <typename T>
  using Func = std::function<T>;
  template <typename T>
  using Scope = std::unique_ptr<T, void (*)(T *)>;
  template <typename T>
  using Ref = std::shared_ptr<T>;
}
#else
#undef bool
typedef _Bool bool;
#endif
