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
  using List = CBN_List_t<T>;
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
      if constexpr (std::floating_point<T> or std::floating_point<U>) {
        return static_cast<f32>(carbon_math_fmod(x, y));
      }
      else return static_cast<i32>(carbon_math_imod(x, y));
    }
    inline auto Rand(void) { return carbon_math_randf(); }
    template <meta::Numeric T, meta::Numeric U>
    auto Rand(const T min, const U max) {
      if constexpr (std::floating_point<T> or std::floating_point<U>) {
        return static_cast<f32>(carbon_math_randf_between(min, max));
      }
      else return static_cast<i32>(carbon_math_rand_between(min, max));
    }
    constexpr auto MT19937 = carbon_math_mt19937_64_rand;
    [[nodiscard]] constexpr auto ToClamped(const auto x, const auto min, const auto max) {
      return CARBON_CLAMP(x, min, max);
    }
    constexpr void Clamp(auto &x, const auto min, const auto max) { x = ToClamped(x, min, max); }
    constexpr void Lerp(auto &a, const auto b, const auto t) { a = CARBON_LERP(a, b, t); }
  }

  namespace time {
    using Chrono = CBN_Chrono;
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

  namespace net {
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

  namespace win {
    using KeyCode     = CBN_KeyCode;
    using MouseButton = CBN_MouseButton;
  }
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
