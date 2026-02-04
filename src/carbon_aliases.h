/*
**  $$================================$$
**  ||       Namespaced Aliases       ||
**  $$================================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus
namespace cbn {
  template <typename T>
  using CircularQueue = CBN_CircularQueue_t<T>;
  template <typename T>
  using CircularBuffer = CircularQueue<T>;
  template <typename T>
  using RingQueue = CircularQueue<T>;
  template <typename T>
  using RingBuffer = CircularQueue<T>;
  template <typename T>
  using SlotMap = CBN_SlotMap_t<T>;

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
    inline void Go(void (*f)(void *), void *arg = 0) { carbon_coroutine_go(f, arg); }
  }

  namespace math {
    constexpr auto Min(auto x, auto y)         { return CARBON_MIN(x, y); }
    constexpr auto Max(auto x, auto y)         { return CARBON_MAX(x, y); }
    void ClampAt(auto &x, auto min, auto max)  { x = Clamp(x, min, max); }
    void LerpAt(auto &a, auto b, auto t)       { a = Lerp(a, b, t); }
    constexpr auto ToRadians(const auto angle) { return CARBON_TO_RADIANS(angle); }
    namespace literals {
      consteval f64 operator""_deg(const u64 n)   { return ToRadians(n); }
      consteval f64 operator""_deg(const flong n) { return ToRadians(n); }
      constexpr auto pi = CARBON_PI;
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
    using Builder = CBN_StrBuilder;
    using View    = CBN_StrView;
    namespace literals {
      inline View operator""_sv(const char *s, usz len) {
        return View::make(s, len);
      }
    }
  }

  namespace audio {
    using UID = CBN_Audio_UID;
    template <typename... Args>
    [[nodiscard]] constexpr auto Load(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_audio_load(cbn::meta::Forward<Args>(args))...);
      else return carbon_audio_load(cbn::meta::Forward<Args>(args)...);
    }
    template <typename... Args>
    [[nodiscard]] constexpr auto LoadStream(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_audio_load_stream(cbn::meta::Forward<Args>(args))...);
      else return carbon_audio_load_stream(cbn::meta::Forward<Args>(args)...);
    }
  }

  namespace sprite_mgr {
    using UID = CBN_Sprite_UID;
    template <typename... Args>
    [[nodiscard]] constexpr auto Load(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_sprite_manager_load(cbn::meta::Forward<Args>(args))...);
      else return carbon_sprite_manager_load(cbn::meta::Forward<Args>(args)...);
    }
  }

  namespace mesh_mgr {
    using UID = CBN_Mesh_UID;
    template <typename... Args>
    [[nodiscard]] constexpr auto Load(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_mesh_manager_load(cbn::meta::Forward<Args>(args))...);
      else return carbon_mesh_manager_load(cbn::meta::Forward<Args>(args)...);
    }
  }
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
