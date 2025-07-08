/*
**  $$===================$$
**  ||       Audio       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef CBN_SlotMap_Key CBN_Audio_UID;

/**
 * @brief Initializes the audio subsystem.
 */
CARBON_API void carbon_audio_init(void);

/**
 * @brief Shutdowns the audio subsystem.
 */
CARBON_API void carbon_audio_shutdown(void);

/**
 * @brief ...
 */
CARBON_API u8 carbon_audio_load_sound_from_file(const char *file, CBN_Audio_UID *out_uid);

/**
 * @brief ...
 */
CARBON_API u8 carbon_audio_load_sound_streaming_from_file(const char *file, CBN_Audio_UID *out_uid);

#ifdef __cplusplus
/**
 * @brief carbon_audio_load_sound_from_file
 */
[[nodiscard]] cbn::Opt<CBN_Audio_UID> carbon_audio_load_sound(const char *file);
/**
 * @brief carbon_audio_load_sound_streaming_from_file
 */
[[nodiscard]] cbn::Opt<CBN_Audio_UID> carbon_audio_load_sound_streaming(const char *file);
/**
 * @brief carbon_audio_load_sound(...)
 */
template <typename... Args>
[[nodiscard]] auto carbon_audio_load_sounds(Args &&... files) {
  return std::make_tuple(carbon_audio_load_sound(std::forward<Args>(files))...);
}
/**
 * @brief carbon_audio_load_sound_streaming(...)
 */
template <typename... Args>
[[nodiscard]] auto carbon_audio_load_sounds_streaming(Args &&... files) {
  return std::make_tuple(carbon_audio_load_sound_streaming(std::forward<Args>(files))...);
}
#endif

/**
 * @brief ...
 */
CARBON_API void carbon_audio_play_sound(const CBN_Audio_UID key);

/**
 * @brief ...
 */
CARBON_API void carbon_audio_stop_sound(const CBN_Audio_UID key);

// Local Variables:
// mode: c++
// End:
