/*
  $$================================$$
  ||       Namespaced Aliases       ||
  $$================================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifdef __cplusplus

namespace cbn {
  namespace co {
    inline void Create(void (*f)(void *), void *arg = 0) { carbon_coroutine_create(f, arg); }
  }

  namespace math {
    constexpr auto ToRadians(const auto phi)   { return CARBON_TO_RADIANS(phi); }
    constexpr auto FromRadians(const auto phi) { return CARBON_FROM_RADIANS(phi); }
    inline namespace literals {
             consteval f64 operator""_deg(const u64 n)   { return ToRadians(n); }
             consteval f64 operator""_deg(const flong n) { return ToRadians(n); }
    }
    template <meta::Numeric T, meta::Numeric U>
    auto Mod(const T x, const U y) {
      if constexpr (meta::Float<T> or meta::Float<U>) {
        return static_cast<f32>(Fmod(x, y));
      }
      else return static_cast<i32>(Imod(x, y));
    }
  }

  namespace time {
    inline namespace literals {
             consteval f64 operator""_ns(const u64 n)    { return n * CARBON_SECS_PER_NANO; }
             consteval f64 operator""_ns(const flong n)  { return n * CARBON_SECS_PER_NANO; }
             consteval f64 operator""_us(const u64 n)    { return n * CARBON_SECS_PER_MICRO; }
             consteval f64 operator""_us(const flong n)  { return n * CARBON_SECS_PER_MICRO; }
             consteval f64 operator""_ms(const u64 n)    { return n * CARBON_SECS_PER_MILLI; }
             consteval f64 operator""_ms(const flong n)  { return n * CARBON_SECS_PER_MILLI; }
             consteval f64 operator""_s(const u64 n)     { return n; }
             consteval f64 operator""_s(const flong n)   { return n; }
             consteval f64 operator""_min(const u64 n)   { return n * CARBON_SECS_PER_MIN; }
             consteval f64 operator""_min(const flong n) { return n * CARBON_SECS_PER_MIN; }
    }
  }
}

#endif  // __cplusplus
