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
 * @param file ...
 * @param out_uid ... (output argument pointer).
 */
CARBON_API u8 carbon_audio_load_from_file(const char *file, CBN_Audio_UID *out_uid);

/**
 * @brief ...
 * @param file ...
 * @param out_uid ... (output argument pointer).
 */
CARBON_API u8 carbon_audio_load_stream_from_file(const char *file, CBN_Audio_UID *out_uid);

#ifdef __cplusplus
/**
 * @see carbon_audio_load_sound_from_file
 */
[[nodiscard]] cbn::Opt<CBN_Audio_UID> carbon_audio_load(const char *file);
/**
 * @see carbon_audio_load_sound_streaming_from_file
 */
[[nodiscard]] cbn::Opt<CBN_Audio_UID> carbon_audio_load_stream(const char *file);
#endif

/**
 * @brief ...
 * @param uid ...
 */
CARBON_API void carbon_audio_play(const CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 */
CARBON_API void carbon_audio_stop(const CBN_Audio_UID uid);

// Local Variables:
// mode: c++
// End:
