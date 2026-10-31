/*
  $$===================$$
  ||       Audio       ||
  $$===================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents ...
 */
CBNDEF_TEQ(CBN_Audio_UID, CBN_SlotMap_Key);
CBNDEF_TAKA(cbn::audio, UID, CBN_Audio_UID);

// Forward declaration
CBNDEF_T(CBN_SKAP);

/**
 * @brief Initializes the audio subsystem.
 */
CBNDEF void carbon_audio_init(void);
CBNDEF_AKA(cbn::audio, Init, carbon_audio_init);

/**
 * @brief Shutdowns the audio subsystem.
 */
CBNDEF void carbon_audio_shutdown(void);
CBNDEF_AKA(cbn::audio, Shutdown, carbon_audio_shutdown);

/**
 * @brief Pauses the execution of the audio subsystem.
 */
CBNDEF void carbon_audio_pause_exec(void);
CBNDEF_AKA(cbn::audio, PauseExec, carbon_audio_pause_exec);

/**
 * @brief Resumes the execution of the audio subsystem.
 */
CBNDEF void carbon_audio_resume_exec(void);
CBNDEF_AKA(cbn::audio, ResumeExec, carbon_audio_resume_exec);

/**
 * @brief Gets the current global engine volume level.
 * @return 32-bit float in [0..1] for normal levels, or in [1..+inf) if amplified.
 */
CBNDEF f32 carbon_audio_get_volume(void);
CBNDEF_AKA(cbn::audio, GetVolume, carbon_audio_get_volume);

/**
 * @brief Sets the desired global engine volume level.
 * @param volume 32-bit float in [0..1] for normal levels, or in [1..+inf] for amplification.
 */
CBNDEF void carbon_audio_set_volume(f32 volume);
CBNDEF_AKA(cbn::audio, SetVolume, carbon_audio_set_volume);

/**
 * @brief ...
 * @param file ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_audio_load_from_file(const char *file, CBN_Audio_UID *out_uid);
CBNDEF_AKA(cbn::audio, LoadFromFile, carbon_audio_load_from_file);

/**
 * @brief ...
 * @param file ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_audio_load_from_file_streaming(const char *file, CBN_Audio_UID *out_uid);
CBNDEF_AKA(cbn::audio, LoadFromFileStreaming, carbon_audio_load_from_file_streaming);

/**
 * @brief ...
 * @param name ...
 * @param skap_handle ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_audio_load_from_skap(const char *name, const CBN_SKAP *skap, CBN_Audio_UID *out_uid);
CBNDEF_AKA(cbn::audio, LoadFromSKAP, carbon_audio_load_from_skap);

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_unload(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_play(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_stop(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_resume(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 * @return ...
 */
CBNDEF bool carbon_audio_is_playing(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 * @return ... in (0..+inf) ...
 */
CBNDEF f32 carbon_audio_get_pitch(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 * @param pitch ... in (0..+inf) ...
 */
CBNDEF void carbon_audio_set_pitch(CBN_Audio_UID uid, f32 pitch);

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_shift_pitch(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 * @param yn A boolean value representing whether the sound must loop or not.
 */
CBNDEF void carbon_audio_set_looping(CBN_Audio_UID uid, bool yn);

/**
 * @brief ...
 * @param uid ...
 * @return ...
 */
CBNDEF f32 carbon_audio_get_cursor(CBN_Audio_UID uid);

/**
 * @brief ...
 * @param uid ...
 * @param target ...
 * @param len_ms ...
 * @return ...
 */
CBNDEF void carbon_audio_set_fade(CBN_Audio_UID uid, f32 target, u64 len_ms);
