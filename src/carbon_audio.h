/*
**  $$===================$$
**  ||       Audio       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

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
CARBON_API u8 carbon_audio_load_sound_from_file(const char *file, CBN_SlotMap_Key *out_key);

/**
 * @brief ...
 */
CARBON_API u8 carbon_audio_load_sound_streaming_from_file(const char *file, CBN_SlotMap_Key *out_key);

/**
 * @brief ...
 */
CARBON_API void carbon_audio_play_sound(const CBN_SlotMap_Key key);

#ifdef __cplusplus
/**
 * @brief carbon_audio_load_sound_from_file
 */
cbn::Opt<CBN_SlotMap_Key> carbon_audio_load_sound(const char *file);

/**
 * @brief carbon_audio_load_sound_streaming_from_file
 */
cbn::Opt<CBN_SlotMap_Key> carbon_audio_load_sound_streaming(const char *file);

/**
 * @brief carbon_audio_load_sound(...)
 */
template <typename... Args>
auto carbon_audio_load_sounds(Args &&... files) {
  return std::make_tuple(carbon_audio_load_sound(std::forward<Args>(files))...);
}

/**
 * @brief carbon_audio_load_sound_streaming(...)
 */
template <typename... Args>
auto carbon_audio_load_sounds_streaming(Args &&... files) {
  return std::make_tuple(carbon_audio_load_sound_streaming(std::forward<Args>(files))...);
}
#endif

// Local Variables:
// mode: c++
// End:
