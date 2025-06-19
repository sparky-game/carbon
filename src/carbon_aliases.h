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
  const auto version = carbon_version;
  using Vec2         = CBN_Vec2;
  using Vec3         = CBN_Vec3;
  using Rect         = CBN_Rect;
  template <typename T>
  using List = CBN_List_t<T>;
  template <typename T>
  using CircularQueue  = CBN_CircularQueue_t<T>;
  template <typename T>
  using CircularBuffer = CircularQueue<T>;
  template <typename T>
  using RingQueue = CircularQueue<T>;
  template <typename T>
  using RingBuffer = CircularQueue<T>;
  using StrBuilder = CBN_StrBuilder;
  using StrView    = CBN_StrView;
  using Image      = CBN_Image;
  using DrawCanvas = CBN_DrawCanvas;
  using SKAP       = CBN_SKAP;
  namespace mem {
    const auto cpy = carbon_memory_copy;
    const auto cmp = carbon_memory_cmp;
    const auto set = carbon_memory_set;
  }
  namespace str {
    const auto len        = carbon_string_len;
    const auto cmp        = carbon_string_cmp;
    const auto ncmp       = carbon_string_cmp_n;
    const auto dup        = carbon_string_dup;
    const auto fmt        = carbon_string_fmt;
    const auto StartsWith = carbon_string_starts_with_substr;
    const auto EndsWith   = carbon_string_starts_with_substr;
    const auto IsNumber   = carbon_string_is_number;
    const auto LevDist    = carbon_string_lev_dist;
  }
  namespace win {
    const auto Open         = carbon_win_open;
    const auto Close        = carbon_win_close;
    const auto SetMaxFPS    = carbon_win_set_max_fps;
    const auto SetIcon      = carbon_win_set_icon;
    const auto GetDeltaTime = carbon_win_get_deltatime;
    const auto Update       = carbon_win_update;
    const auto ShouldClose  = carbon_win_shouldclose;
    template <typename... Args>
    auto ForFrame(Args &&... args) {
      return carbon_win_forframe(std::forward<Args>(args)...);
    }
  }
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
