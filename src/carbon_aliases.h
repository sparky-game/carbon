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
  using Binary     = CBN_Binary;
  using Image      = CBN_Image;
  using Sprite     = CBN_Sprite;
  using Transform  = CBN_Transform;
  using Mesh       = CBN_Mesh;
  using DrawCanvas = CBN_DrawCanvas;
  using Camera     = CBN_Camera;
  using SKAP       = CBN_SKAP;
  constexpr auto Version = carbon_version;

  namespace log {
    using Color = CBN_Log_Color;
  }

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

  namespace mem {
    constexpr auto Alloc   = carbon_memory_alloc;
    constexpr auto Realloc = carbon_memory_realloc;
    constexpr auto Zeroed  = carbon_memory_zeroed;
    constexpr auto Copy    = carbon_memory_copy,  cp  = Copy;
    constexpr auto Compare = carbon_memory_cmp,   cmp = Compare;
    constexpr auto Set     = carbon_memory_set;
    constexpr auto Free    = carbon_memory_free;
  }

  namespace co {
    constexpr auto ID    = carbon_coroutine_id;
    constexpr auto Alive = carbon_coroutine_alive;
    inline void Go(void (*f)(void *), void *arg = 0) { carbon_coroutine_go(f, arg); }
    constexpr auto Yield      = carbon_coroutine_yield;
    constexpr auto SleepRead  = carbon_coroutine_sleep_read;
    constexpr auto SleepWrite = carbon_coroutine_sleep_write;
    constexpr auto Wakeup     = carbon_coroutine_wakeup;
  }

  namespace math {
    using Vec2 = CBN_Vec2;
    using Vec3 = CBN_Vec3;
    using Vec4 = CBN_Vec4;
    using Quat = CBN_Quat;
    using Mat4 = CBN_Mat4;
    using Rect = CBN_Rect;
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
    constexpr auto Abs     = carbon_math_abs;
    constexpr auto Exp     = carbon_math_exp;
    constexpr auto Sin     = carbon_math_sin;
    constexpr auto Cos     = carbon_math_cos;
    constexpr auto Tan     = carbon_math_tan;
    constexpr auto Tanh    = carbon_math_tanh;
    constexpr auto Atan2   = carbon_math_atan2;
    constexpr auto MT19937 = carbon_math_mt19937_64_rand;
    [[nodiscard]] constexpr auto ToClamped(const auto x, const auto min, const auto max) {
      return CARBON_CLAMP(x, min, max);
    }
    constexpr void Clamp(auto &x, const auto min, const auto max) { x = ToClamped(x, min, max); }
    constexpr void Lerp(auto &a, const auto b, const auto t) { a = CARBON_LERP(a, b, t); }
  }

  namespace crypto {
    namespace b64 {
      constexpr auto Encode = carbon_crypto_base64_encode;
      constexpr auto Decode = carbon_crypto_base64_decode;
    }
    namespace CRC32 {
      constexpr auto Compute = carbon_crypto_crc32;
      constexpr auto Combine = carbon_crypto_crc32_combine;
    }
    constexpr auto DJB2 = carbon_crypto_djb2;
    namespace SHA1 {
      constexpr auto Compute     = carbon_crypto_sha1;
      constexpr auto ToHexString = carbon_crypto_sha1_to_hex_cstr;
      constexpr auto AsHexString = carbon_crypto_sha1_as_hex_cstr;
    }
    namespace SHA256 {
      constexpr auto Compute     = carbon_crypto_sha256;
      constexpr auto ToHexString = carbon_crypto_sha256_to_hex_cstr;
      constexpr auto AsHexString = carbon_crypto_sha256_as_hex_cstr;
    }
    namespace Keccak256 {
      constexpr auto Compute     = carbon_crypto_keccak256;
      constexpr auto ToHexString = carbon_crypto_keccak256_to_hex_cstr;
      constexpr auto AsHexString = carbon_crypto_keccak256_as_hex_cstr;
    }
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
    constexpr auto Get = carbon_time_get, now = Get;
  }

  namespace str {
    using Builder = CBN_StrBuilder;
    using View    = CBN_StrView;
    namespace literals {
      inline View operator""_sv(const char *s, usz len) {
        return View::make(s, len);
      }
    }
    constexpr auto Length     = carbon_string_len,   len  = Length;
    constexpr auto Compare    = carbon_string_cmp,   cmp  = Compare;
    constexpr auto Compare_n  = carbon_string_cmp_n, ncmp = Compare_n;
    constexpr auto Duplicate  = carbon_string_dup,   dup  = Duplicate;
    constexpr auto Format     = carbon_string_fmt,   fmt  = Format;
    constexpr auto StartsWith = carbon_string_starts_with_substr;
    constexpr auto EndsWith   = carbon_string_ends_with_substr;
    constexpr auto IsNumber   = carbon_string_is_number;
    constexpr auto ToNumber   = carbon_string_to_number;
    constexpr auto LevDist    = carbon_string_lev_dist;
  }

  namespace color {
    constexpr auto FromHSV        = carbon_color_from_hsv;
    constexpr auto ToHSV          = carbon_color_to_hsv;
    constexpr auto Scale          = carbon_color_scale;
    constexpr auto Add            = carbon_color_add;
    constexpr auto Complementary  = carbon_color_complementary;
    constexpr auto ToNormalized   = carbon_color_to_normalized;
    constexpr auto FromNormalized = carbon_color_from_normalized;
  }

  namespace fs {
    constexpr auto Exists     = carbon_fs_exists;
    constexpr auto IsFile     = carbon_fs_is_regular_file;
    constexpr auto IsDir      = carbon_fs_is_directory;
    constexpr auto LastMod    = carbon_fs_mtime;
    constexpr auto Rename     = carbon_fs_rename,             mv      = Rename;
    constexpr auto Copy       = carbon_fs_copy,               cp      = Copy;
    constexpr auto Remove     = carbon_fs_remove,             rm      = Remove;
    constexpr auto RemoveAll  = carbon_fs_remove_all,         rm_r    = RemoveAll;
    constexpr auto ChangeDir  = carbon_fs_change_directory,   cd      = ChangeDir;
    constexpr auto CreateDir  = carbon_fs_create_directory,   mkdir   = CreateDir;
    constexpr auto CreateDirs = carbon_fs_create_directories, mkdir_p = CreateDirs;
    constexpr auto GetCWD     = carbon_fs_get_curr_directory, cwd     = GetCWD;
    constexpr auto GetBinDir  = carbon_fs_get_bin_directory;
    constexpr auto Size       = carbon_fs_get_file_size;
  }

  namespace net {
    constexpr auto IsValidIPv4 = carbon_net_is_valid_ipv4;
    constexpr auto ResolveDNS  = carbon_net_resolve_dns_to_ipv4;
  }

  namespace audio {
    using UID = CBN_Audio_UID;
    constexpr auto Init      = carbon_audio_init;
    constexpr auto Shutdown  = carbon_audio_shutdown;
    constexpr auto GetVolume = carbon_audio_get_volume;
    constexpr auto SetVolume = carbon_audio_set_volume;
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
    constexpr auto Play       = carbon_audio_play;
    constexpr auto Stop       = carbon_audio_stop;
    constexpr auto GetPitch   = carbon_audio_get_pitch;
    constexpr auto SetPitch   = carbon_audio_set_pitch;
    constexpr auto ShiftPitch = carbon_audio_shift_pitch;
  }

  namespace sprite_mgr {
    using UID = CBN_Sprite_UID;
    constexpr auto Init     = carbon_sprite_manager_init;
    constexpr auto Shutdown = carbon_sprite_manager_shutdown;
    template <typename... Args>
    [[nodiscard]] constexpr auto Load(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_sprite_manager_load(cbn::meta::Forward<Args>(args))...);
      else return carbon_sprite_manager_load(cbn::meta::Forward<Args>(args)...);
    }
    constexpr auto Lookup = carbon_sprite_manager_lookup;
  }

  namespace mesh_mgr {
    using UID = CBN_Mesh_UID;
    constexpr auto Init     = carbon_mesh_manager_init;
    constexpr auto Shutdown = carbon_mesh_manager_shutdown;
    template <typename... Args>
    [[nodiscard]] constexpr auto Load(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_mesh_manager_load(cbn::meta::Forward<Args>(args))...);
      else return carbon_mesh_manager_load(cbn::meta::Forward<Args>(args)...);
    }
    constexpr auto Lookup = carbon_mesh_manager_lookup;
  }

  namespace win {
    using KeyCode     = CBN_KeyCode;
    using MouseButton = CBN_MouseButton;
    constexpr auto Open         = carbon_win_open;
    constexpr auto Close        = carbon_win_close;
    constexpr auto SetMaxFPS    = carbon_win_set_max_fps;
    constexpr auto SetIcon      = carbon_win_set_icon;
    constexpr auto GetDeltaTime = carbon_win_get_deltatime;
    constexpr auto GetFPS       = carbon_win_get_fps;
    constexpr auto Update       = carbon_win_update;
    constexpr auto ShouldClose  = carbon_win_shouldclose;
    template <typename T>
    constexpr void ForFrame(T &&callback) {
      carbon_win_forframe(callback);
    }
    constexpr auto Exit               = carbon_win_exit;
    constexpr auto GetKeyDown         = carbon_win_get_key_down;
    constexpr auto GetKey             = carbon_win_get_key;
    constexpr auto GetKeyUp           = carbon_win_get_key_up;
    constexpr auto GetMouseButtonDown = carbon_win_get_mouse_button_down;
    constexpr auto GetMouseButton     = carbon_win_get_mouse_button;
    constexpr auto GetMouseButtonUp   = carbon_win_get_mouse_button_up;
    constexpr auto GetMousePosition   = carbon_win_get_mouse_position;
    constexpr auto SetMouseVisibility = carbon_win_set_mouse_visibility;
  }
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
