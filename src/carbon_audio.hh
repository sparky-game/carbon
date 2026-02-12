/*
  $$===================$$
  ||       Audio       ||
  $$===================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef CBN_SlotMap_Key CBN_Audio_UID;

// Forward declaration
#ifdef __cplusplus
struct CBN_SKAP;
#else
typedef struct CBN_SKAP_t CBN_SKAP;
#endif

/**
 * @brief Initializes the audio subsystem.
 */
CBNDEF void carbon_audio_init(void);
CBNDEF_FN(cbn::audio, Init, carbon_audio_init);

/**
 * @brief Shutdowns the audio subsystem.
 */
CBNDEF void carbon_audio_shutdown(void);
CBNDEF_FN(cbn::audio, Shutdown, carbon_audio_shutdown);

/**
 * @brief Gets the current global engine volume level.
 * @return 32-bit float in [0..1] for normal levels, or in [1..+inf) if amplified.
 */
CBNDEF f32 carbon_audio_get_volume(void);
CBNDEF_FN(cbn::audio, GetVolume, carbon_audio_get_volume);

/**
 * @brief Sets the desired global engine volume level.
 * @param volume 32-bit float in [0..1] for normal levels, or in [1..+inf] for amplification.
 */
CBNDEF void carbon_audio_set_volume(const f32 volume);
CBNDEF_FN(cbn::audio, SetVolume, carbon_audio_set_volume);

/**
 * @brief ...
 * @param file ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_audio_load_from_file(const char *file, CBN_Audio_UID *out_uid);

/**
 * @brief ...
 * @param file ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_audio_load_stream_from_file(const char *file, CBN_Audio_UID *out_uid);

/**
 * @brief ...
 * @param name ...
 * @param skap_handle ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_audio_load_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Audio_UID *out_uid);

/**
 * @brief ...
 * @param name ...
 * @param skap_handle ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_audio_load_stream_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Audio_UID *out_uid);

#ifdef __cplusplus
cbn::Opt<CBN_Audio_UID> carbon_audio_load(const char *file) {
  CBN_Audio_UID uid;
  if (!carbon_audio_load_from_file(file, &uid)) return {};
  return uid;
}

cbn::Opt<CBN_Audio_UID> carbon_audio_load_stream(const char *file) {
  CBN_Audio_UID uid;
  if (!carbon_audio_load_stream_from_file(file, &uid)) return {};
  return uid;
}
#endif

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_play(const CBN_Audio_UID uid);
CBNDEF_FN(cbn::audio, Play, carbon_audio_play);

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_stop(const CBN_Audio_UID uid);
CBNDEF_FN(cbn::audio, Stop, carbon_audio_stop);

/**
 * @brief ...
 * @param uid ...
 * @return ... in (0..+inf) ...
 */
CBNDEF f32 carbon_audio_get_pitch(const CBN_Audio_UID uid);
CBNDEF_FN(cbn::audio, GetPitch, carbon_audio_get_pitch);

/**
 * @brief ...
 * @param uid ...
 * @param pitch ... in (0..+inf) ...
 */
CBNDEF void carbon_audio_set_pitch(const CBN_Audio_UID uid, const f32 pitch);
CBNDEF_FN(cbn::audio, SetPitch, carbon_audio_set_pitch);

/**
 * @brief ...
 * @param uid ...
 */
CBNDEF void carbon_audio_shift_pitch(const CBN_Audio_UID uid);
CBNDEF_FN(cbn::audio, ShiftPitch, carbon_audio_shift_pitch);
