/*
  $$================================$$
  ||       Namespaced Aliases       ||
  $$================================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifdef __cplusplus

namespace cbn {
  template <typename... Args>
  constexpr void print(const char *msg, Args &&... args) {
    carbon_print(msg, cbn::meta::Forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void eprint(const char *msg, Args &&... args) {
    carbon_eprint(msg, cbn::meta::Forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void println(const char *msg = "", Args &&... args) {
    carbon_println(msg, cbn::meta::Forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void eprintln(const char *msg = "", Args &&... args) {
    carbon_eprintln(msg, cbn::meta::Forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void cprint(const log::Color color, const char *msg, Args &&... args) {
    carbon_cprint(color, msg, cbn::meta::Forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void ceprint(const log::Color color, const char *msg, Args &&... args) {
    carbon_ceprint(color, msg, cbn::meta::Forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void cprintln(const log::Color color, const char *msg, Args &&... args) {
    carbon_cprintln(color, msg, cbn::meta::Forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void ceprintln(const log::Color color, const char *msg, Args &&... args) {
    carbon_ceprintln(color, msg, cbn::meta::Forward<Args>(args)...);
  }

  namespace co {
    inline void Create(void (*f)(void *), void *arg = 0) { carbon_coroutine_create(f, arg); }
  }

  namespace math {
    constexpr auto ToRadians(const auto phi)   { return CARBON_TO_RADIANS(phi); }
    constexpr auto FromRadians(const auto phi) { return CARBON_FROM_RADIANS(phi); }
    namespace literals {
      constexpr auto pi = CARBON_PI;
      consteval f64 operator""_deg(const u64 n)   { return ToRadians(n); }
      consteval f64 operator""_deg(const flong n) { return ToRadians(n); }
      consteval f64 operator""_pi(const u64 n)   { return n * pi; }
      consteval f64 operator""_pi(const flong n) { return n * pi; }
    }
    template <meta::Numeric T, meta::Numeric U>
    auto Mod(const T x, const U y) {
      if constexpr (meta::Float<T> or meta::Float<U>) {
        return static_cast<f32>(carbon_math_fmod(x, y));
      }
      else return static_cast<i32>(carbon_math_imod(x, y));
    }
  }

  namespace time {
    namespace literals {
      consteval f64 operator""_ns(const u64 n)    { return n * 1e-9; }
      consteval f64 operator""_ns(const flong n)  { return n * 1e-9; }
      consteval f64 operator""_us(const u64 n)    { return n * 1e-6; }
      consteval f64 operator""_us(const flong n)  { return n * 1e-6; }
      consteval f64 operator""_ms(const u64 n)    { return n * 1e-3; }
      consteval f64 operator""_ms(const flong n)  { return n * 1e-3; }
      consteval f64 operator""_s(const u64 n)     { return n; }
      consteval f64 operator""_s(const flong n)   { return n; }
      consteval f64 operator""_min(const u64 n)   { return n * 60; }
      consteval f64 operator""_min(const flong n) { return n * 60; }
    }
  }

  namespace str {
    namespace literals {
      inline View operator""_sv(const char *s, usz len) {
        return View::make(s, len);
      }
    }
  }

  namespace audio {
    using UID = CBN_Audio_UID;
  }

  namespace mesh_mgr {
    using UID = CBN_Mesh_UID;
  }
}

#endif  // __cplusplus
