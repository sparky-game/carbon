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
  using Vec2 = CBN_Vec2;
  using Vec3 = CBN_Vec3;
  using Rect = CBN_Rect;
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
  using Chrono     = CBN_Chrono;
  using StrBuilder = CBN_StrBuilder;
  using StrView    = CBN_StrView;
  using Image      = CBN_Image;
  using Sprite     = CBN_Sprite;
  using DrawCanvas = CBN_DrawCanvas;
  using SKAP       = CBN_SKAP;
  const auto Version = carbon_version;
  namespace log {
    using Color = CBN_Log_Color;
  }
  template <typename... Args>
  constexpr void print(const char *msg, Args &&... args) {
    carbon_print(msg, std::forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void eprint(const char *msg, Args &&... args) {
    carbon_eprint(msg, std::forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void println(const char *msg = "", Args &&... args) {
    carbon_println(msg, std::forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void eprintln(const char *msg = "", Args &&... args) {
    carbon_eprintln(msg, std::forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void cprint(const log::Color color, const char *msg, Args &&... args) {
    carbon_cprint(color, msg, std::forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void ceprint(const log::Color color, const char *msg, Args &&... args) {
    carbon_ceprint(color, msg, std::forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void cprintln(const log::Color color, const char *msg, Args &&... args) {
    carbon_cprintln(color, msg, std::forward<Args>(args)...);
  }
  template <typename... Args>
  constexpr void ceprintln(const log::Color color, const char *msg, Args &&... args) {
    carbon_ceprintln(color, msg, std::forward<Args>(args)...);
  }
  namespace mem {
    const auto Copy    = carbon_memory_copy, cp  = Copy;
    const auto Compare = carbon_memory_cmp,  cmp = Compare;
    const auto Set     = carbon_memory_set,  set = Set;
  }
  namespace math {
    const auto Sin = carbon_math_sin;
    const auto Cos = carbon_math_cos;
    inline constexpr auto ToRadians(const auto angle) { return CARBON_TO_RADIANS(angle); }
    namespace literals {
      consteval f64 operator""_deg(const u64 n)   { return ToRadians(n); }
      consteval f64 operator""_deg(const flong n) { return ToRadians(n); }
      constexpr auto pi = CARBON_PI;
      consteval f64 operator""_pi(const u64 n)   { return n * pi; }
      consteval f64 operator""_pi(const flong n) { return n * pi; }
    }
    const auto Abs = carbon_math_abs;
    template <Numeric T, Numeric U>
    auto Mod(const T x, const U y) {
      if constexpr (std::floating_point<T> or std::floating_point<U>) {
        return static_cast<f32>(carbon_math_fmod(x, y));
      }
      else return static_cast<i32>(carbon_math_imod(x, y));
    }
    inline auto Rand(void) { return carbon_math_randf(); }
    template <Numeric T, Numeric U>
    auto Rand(const T min, const U max) {
      if constexpr (std::floating_point<T> or std::floating_point<U>) {
        return static_cast<f32>(carbon_math_randf_between(min, max));
      }
      else return static_cast<i32>(carbon_math_rand_between(min, max));
    }
    const auto MT19937 = carbon_math_mt19937_64_rand;
    inline void Clamp(auto &x, const auto min, const auto max) { x = CARBON_CLAMP(x, min, max); }
    inline void Lerp(auto &a, const auto b, const auto t) { a = CARBON_LERP(a, b, t); }
  }
  namespace crypto {
    namespace b64 {
      const auto Encode = carbon_crypto_base64_encode;
      const auto Decode = carbon_crypto_base64_decode;
    }
    const auto CRC32 = carbon_crypto_crc32;
    const auto DJB2  = carbon_crypto_djb2;
    namespace SHA1 {
      const auto Compute     = carbon_crypto_sha1;
      const auto ToHexString = carbon_crypto_sha1_to_hex_cstr;
      const auto AsHexString = carbon_crypto_sha1_as_hex_cstr;
    }
    namespace SHA256 {
      const auto Compute     = carbon_crypto_sha256;
      const auto ToHexString = carbon_crypto_sha256_to_hex_cstr;
      const auto AsHexString = carbon_crypto_sha256_as_hex_cstr;
    }
    namespace Keccak256 {
      const auto Compute     = carbon_crypto_keccak256;
      const auto ToHexString = carbon_crypto_keccak256_to_hex_cstr;
      const auto AsHexString = carbon_crypto_keccak256_as_hex_cstr;
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
    const auto Get = carbon_time_get, now = Get;
  }
  namespace str {
    const auto Length     = carbon_string_len,   len  = Length;
    const auto Compare    = carbon_string_cmp,   cmp  = Compare;
    const auto Compare_n  = carbon_string_cmp_n, ncmp = Compare_n;
    const auto Duplicate  = carbon_string_dup,   dup  = Duplicate;
    const auto Format     = carbon_string_fmt,   fmt  = Format;
    const auto StartsWith = carbon_string_starts_with_substr;
    const auto EndsWith   = carbon_string_starts_with_substr;
    const auto IsNumber   = carbon_string_is_number;
    const auto LevDist    = carbon_string_lev_dist;
  }
  namespace fs {
    const auto Exists     = carbon_fs_exists;
    const auto IsFile     = carbon_fs_is_regular_file;
    const auto IsDir      = carbon_fs_is_directory;
    const auto LastMod    = carbon_fs_mtime;
    const auto Rename     = carbon_fs_rename,             mv      = Rename;
    const auto Copy       = carbon_fs_copy,               cp      = Copy;
    const auto Remove     = carbon_fs_remove,             rm      = Remove;
    const auto RemoveAll  = carbon_fs_remove_all,         rm_r    = RemoveAll;
    const auto ChangeDir  = carbon_fs_change_directory,   cd      = ChangeDir;
    const auto CreateDir  = carbon_fs_create_directory,   mkdir   = CreateDir;
    const auto CreateDirs = carbon_fs_create_directories, mkdir_p = CreateDirs;
    const auto GetCWD     = carbon_fs_get_curr_directory, cwd     = GetCWD;
    const auto GetBinDir  = carbon_fs_get_bin_directory;
    const auto Size       = carbon_fs_get_file_size;
  }
  namespace audio {
    using UID           = CBN_Audio_UID;
    const auto Init     = carbon_audio_init;
    const auto Shutdown = carbon_audio_shutdown;
    template <typename... Args>
    [[nodiscard]] auto Load(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_audio_load(std::forward<Args>(args))...);
      else return carbon_audio_load(std::forward<Args>(args)...);
    }
    template <typename... Args>
    [[nodiscard]] auto LoadStream(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_audio_load_stream(std::forward<Args>(args))...);
      else return carbon_audio_load_stream(std::forward<Args>(args)...);
    }
    const auto Play = carbon_audio_play;
    const auto Stop = carbon_audio_stop;
  }
  namespace sprite_mgr {
    using UID           = CBN_Sprite_UID;
    const auto Init     = carbon_sprite_manager_init;
    const auto Shutdown = carbon_sprite_manager_shutdown;
    template <typename... Args>
    [[nodiscard]] auto Load(Args &&... args) {
      if constexpr (sizeof...(args) > 1) return std::make_tuple(carbon_sprite_manager_load(std::forward<Args>(args))...);
      else return carbon_sprite_manager_load(std::forward<Args>(args)...);
    }
    const auto Lookup = carbon_sprite_manager_lookup;
  }
  namespace win {
    using KeyCode           = CBN_KeyCode;
    using MouseButton       = CBN_MouseButton;
    const auto Open         = carbon_win_open;
    const auto Close        = carbon_win_close;
    const auto SetMaxFPS    = carbon_win_set_max_fps;
    const auto SetIcon      = carbon_win_set_icon;
    const auto GetDeltaTime = carbon_win_get_deltatime;
    const auto GetFPS       = carbon_win_get_fps;
    const auto Update       = carbon_win_update;
    const auto ShouldClose  = carbon_win_shouldclose;
    template <typename T>
    void ForFrame(T &&callback) {
      carbon_win_forframe(callback);
    }
    const auto GetKeyDown         = carbon_win_get_key_down;
    const auto GetKey             = carbon_win_get_key;
    const auto GetKeyUp           = carbon_win_get_key_up;
    const auto GetMouseButtonDown = carbon_win_get_mouse_button_down;
    const auto GetMouseButton     = carbon_win_get_mouse_button;
    const auto GetMouseButtonUp   = carbon_win_get_mouse_button_up;
    const auto GetMousePosition   = carbon_win_get_mouse_position;
  }
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
