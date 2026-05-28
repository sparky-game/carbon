// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

// NOTE: we ignore here these warnings because these dependencies
// do strange things like shadowing variables, not handling
// switch-cases rigorously, and such.
CARBON_COMPILER_DIAG_BEGIN;
CARBON_COMPILER_DIAG_IGNORE("-Wswitch-enum");
CARBON_COMPILER_DIAG_IGNORE("-Wshadow");
#define STB_VORBIS_NO_INTEGER_CONVERSION
#include "../thirdparty/stb/stb_vorbis.c"
#define MINIAUDIO_IMPLEMENTATION
#include "../thirdparty/miniaudio/miniaudio.h"
CARBON_COMPILER_DIAG_END;

typedef struct {
  ma_sound *sound;
  ma_decoder *decoder;
  CBN_Span binary;
} CBN_Audio_Entry;

static ma_engine carbon_audio__engine;
static CBN_SlotMap carbon_audio__library;

#define CARBON_AUDIO__SOUND_FLAGS (MA_SOUND_FLAG_ASYNC)
#define CARBON_AUDIO__SOUND_STREAM_FLAGS (MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_LOOPING | MA_SOUND_FLAG_NO_PITCH)

void carbon_audio_init(void) {
  ma_result res = ma_engine_init(0, &carbon_audio__engine);
  CBN_ASSERT(res == MA_SUCCESS && "Failed to initialize the audio engine");
  carbon_audio__library = carbon_slotmap_create(sizeof(CBN_Audio_Entry));
  CBN_INFO("Initialized audio subsystem successfully");
}

void carbon_audio_shutdown(void) {
  carbon_slotmap_foreach(CBN_Audio_Entry, carbon_audio__library) {
    ma_sound_uninit(it.var.sound);
    carbon_memory_free(it.var.sound);
    if (it.var.decoder) {
      ma_decoder_uninit(it.var.decoder);
      carbon_memory_free(it.var.decoder);
    }
    if (it.var.binary.data) {
      carbon_memory_free(it.var.binary.data);
    }
  }
  carbon_slotmap_destroy(&carbon_audio__library);
  ma_engine_uninit(&carbon_audio__engine);
  CBN_INFO("Shutdowned audio subsystem successfully");
}

f32 carbon_audio_get_volume(void) {
  return ma_engine_get_volume(&carbon_audio__engine);
}

void carbon_audio_set_volume(f32 volume) {
  ma_engine_set_volume(&carbon_audio__engine, volume);
}

CBNINL bool carbon_audio__load_from_file_ex(const char *file, CBN_Audio_UID *out_uid, ma_sound_flags flags) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  CBN_Audio_Entry entry = {0};
  entry.sound = carbon_memory_alloc(sizeof(ma_sound));
  if (MA_SUCCESS != ma_sound_init_from_file(&carbon_audio__engine, file, flags, 0, 0, entry.sound)) {
    CBN_ERROR("Failed to load sound from file (`%s`)", file);
    carbon_memory_free(entry.sound);
    return false;
  }
  *out_uid = carbon_slotmap_push(&carbon_audio__library, &entry);
  return true;
}

bool carbon_audio_load_from_file(const char *file, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_file_ex(file, out_uid, CARBON_AUDIO__SOUND_FLAGS);
}

bool carbon_audio_load_from_file_streaming(const char *file, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_file_ex(file, out_uid, CARBON_AUDIO__SOUND_STREAM_FLAGS);
}

bool carbon_audio_load_from_skap(const char *name, const CBN_SKAP *skap, CBN_Audio_UID *out_uid) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  CBN_Audio_Entry entry = {0};
  if (!carbon_skap_lookup(skap, CARBON_SKAP_ASSET_TYPE_BINARY, name, &entry.binary)) return false;
  entry.decoder = carbon_memory_alloc(sizeof(ma_decoder));
  if (MA_SUCCESS != ma_decoder_init_memory(entry.binary.data, entry.binary.size, 0, entry.decoder)) {
    CBN_ERROR("Failed to create decoder");
    carbon_memory_free(entry.decoder);
    carbon_memory_free(entry.binary.data);
    return false;
  }
  entry.sound = carbon_memory_alloc(sizeof(ma_sound));
  if (MA_SUCCESS != ma_sound_init_from_data_source(&carbon_audio__engine, entry.decoder, CARBON_AUDIO__SOUND_FLAGS, 0, entry.sound)) {
    CBN_ERROR("Failed to load sound from audio buffer");
    carbon_memory_free(entry.sound);
    carbon_memory_free(entry.decoder);
    carbon_memory_free(entry.binary.data);
    return false;
  }
  *out_uid = carbon_slotmap_push(&carbon_audio__library, &entry);
  return true;
}

void carbon_audio_unload(CBN_Audio_UID uid) {
  CBN_Audio_Entry entry = {0};
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &entry)) return;
  ma_sound_uninit(entry.sound);
  carbon_memory_free(entry.sound);
  if (entry.decoder) {
    ma_decoder_uninit(entry.decoder);
    carbon_memory_free(entry.decoder);
  }
  if (entry.binary.data) {
    carbon_memory_free(entry.binary.data);
  }
  carbon_slotmap_remove(&carbon_audio__library, uid);
}

void carbon_audio_play(CBN_Audio_UID uid) {
  CBN_Audio_Entry entry = {0};
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &entry)) return;
  ma_sound_seek_to_pcm_frame(entry.sound, 0);
  ma_sound_start(entry.sound);
}

void carbon_audio_stop(CBN_Audio_UID uid) {
  CBN_Audio_Entry entry = {0};
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &entry)) return;
  ma_sound_stop(entry.sound);
}

void carbon_audio_resume(CBN_Audio_UID uid) {
  CBN_Audio_Entry entry = {0};
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &entry)) return;
  ma_sound_start(entry.sound);
}

bool carbon_audio_is_playing(CBN_Audio_UID uid) {
  CBN_Audio_Entry entry = {0};
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &entry)) return false;
  return ma_sound_is_playing(entry.sound);
}

f32 carbon_audio_get_pitch(CBN_Audio_UID uid) {
  CBN_Audio_Entry entry = {0};
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &entry)) return 0;
  return ma_sound_get_pitch(entry.sound);
}

void carbon_audio_set_pitch(CBN_Audio_UID uid, f32 pitch) {
  CBN_Audio_Entry entry = {0};
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &entry)) return;
  ma_sound_set_pitch(entry.sound, pitch);
}

void carbon_audio_shift_pitch(CBN_Audio_UID uid) {
  static const i8 semitones[] = {-4, -2, 0, 2, 4, 7, 9};
  i8 semitone = semitones[carbon_rng_lcg_range(0, CARBON_ARRAY_LEN(semitones) - 1)];
  f32 pitch = carbon_math_pow(2, (f32) semitone/12);
  carbon_audio_set_pitch(uid, pitch);
}
