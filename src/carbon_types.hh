/*
  $$===================$$
  ||       Types       ||
  $$===================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef void u0;  // Maybe we're all just birds staring at computer monitors.
typedef unsigned char u8;
typedef signed char i8;
static_assert(sizeof(u8) == 1 && sizeof(i8) == 1, "Expected u8/i8 to be 1 byte");
typedef unsigned short u16;
typedef signed short i16;
static_assert(sizeof(u16) == 2 && sizeof(i16) == 2, "Expected u16/i16 to be 2 bytes");
typedef unsigned int u32;
typedef signed int i32;
static_assert(sizeof(u32) == 4 && sizeof(i32) == 4, "Expected u32/i32 to be 4 bytes");
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
#if defined(CARBON_SIMD_HAS_SSE) || defined(CARBON_SIMD_HAS_SSE2) || defined(CARBON_SIMD_HAS_SSE3) || defined(CARBON_SIMD_HAS_SSSE3) || defined(CARBON_SIMD_HAS_SSE41) || defined(CARBON_SIMD_HAS_SSE42)
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

// Seems that multi-billion dollar company is unable to implement `pthread_barrier_t` synchronization
// object as part as the `pthread` library in macOS.
// Fucking incompetent bitches... They glow in the dark, you can see them if you're driving, you
// just run them over, that's what you do.
#ifdef __APPLE__
typedef i32 pthread_barrierattr_t;
typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  i32 count;
  i32 trip_count;
} pthread_barrier_t;
CBNINL i32 pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, u32 count)
{
  CARBON_UNUSED(attr);
  if (count == 0) {
    errno = EINVAL;
    return -1;
  }
  if (0 > pthread_mutex_init(&barrier->mutex, 0)) return -1;
  if (0 > pthread_cond_init(&barrier->cond, 0)) {
    pthread_mutex_destroy(&barrier->mutex);
    return -1;
  }
  barrier->trip_count = count;
  barrier->count = 0;
  return 0;
}
CBNINL i32 pthread_barrier_destroy(pthread_barrier_t *barrier)
{
  pthread_cond_destroy(&barrier->cond);
  pthread_mutex_destroy(&barrier->mutex);
  return 0;
}
CBNINL i32 pthread_barrier_wait(pthread_barrier_t *barrier)
{
  pthread_mutex_lock(&barrier->mutex);
  ++barrier->count;
  if (barrier->count >= barrier->trip_count) {
    barrier->count = 0;
    pthread_cond_broadcast(&barrier->cond);
    pthread_mutex_unlock(&barrier->mutex);
    return 1;
  }
  else {
    pthread_cond_wait(&barrier->cond, &barrier->mutex);
    pthread_mutex_unlock(&barrier->mutex);
    return 0;
  }
}
#endif
