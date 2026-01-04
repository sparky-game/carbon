// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

// NOTE: we ignore here these warnings because these dependencies
// do strange things like shadowing variables, not handling
// switch-cases rigorously, and such.
CARBON_COMPILER_DIAG_BEGIN;
CARBON_COMPILER_DIAG_IGNORE("-Wswitch-enum");
CARBON_COMPILER_DIAG_IGNORE("-Wshadow");
#include "../thirdparty/stb_vorbis/stb_vorbis.c"
#define MINIAUDIO_IMPLEMENTATION
#include "../thirdparty/miniaudio/miniaudio.h"
CARBON_COMPILER_DIAG_END;

static ma_engine carbon_audio__engine;
static CBN_SlotMap carbon_audio__sounds;

static CBN_List carbon_audio__decoders;
static CBN_List carbon_audio__binaries;

#define CARBON_AUDIO__SOUND_FLAGS (MA_SOUND_FLAG_ASYNC)
#define CARBON_AUDIO__SOUND_STREAM_FLAGS (MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_LOOPING | MA_SOUND_FLAG_NO_PITCH)

void carbon_audio_init(void) {
  ma_result res = ma_engine_init(0, &carbon_audio__engine);
  CBN_ASSERT(res == MA_SUCCESS && "Failed to initialize the audio engine");
  carbon_audio__sounds = carbon_slotmap_create(sizeof(ma_sound *));
  carbon_audio__decoders = carbon_list_create(sizeof(ma_decoder *));
  carbon_audio__binaries = carbon_list_create(sizeof(CBN_Binary));
  CBN_INFO("Initialized audio subsystem successfully");
}

void carbon_audio_shutdown(void) {
  if (carbon_audio__sounds.size) {
    carbon_slotmap_foreach(ma_sound *, carbon_audio__sounds) {
      ma_sound_uninit(it.var);
      carbon_memory_free(it.var);
    }
  }
  if (carbon_audio__decoders.size) {
    carbon_list_foreach(ma_decoder *, carbon_audio__decoders) {
      ma_decoder_uninit(it.var);
    }
  }
  if (carbon_audio__binaries.size) {
    carbon_list_foreach(CBN_Binary, carbon_audio__binaries) {
      carbon_memory_free(it.var.data);
    }
  }
  carbon_list_destroy(&carbon_audio__decoders);
  carbon_list_destroy(&carbon_audio__binaries);
  carbon_slotmap_destroy(&carbon_audio__sounds);
  ma_engine_uninit(&carbon_audio__engine);
  CBN_INFO("Shutdowned audio subsystem successfully");
}

f32 carbon_audio_get_volume(void) {
  return ma_engine_get_volume(&carbon_audio__engine);
}

void carbon_audio_set_volume(const f32 volume) {
  ma_engine_set_volume(&carbon_audio__engine, volume);
}

CBNINL bool carbon_audio__load_from_file_ex(const char *file, CBN_Audio_UID *out_uid, ma_sound_flags flags) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  ma_sound *sound = (ma_sound *) carbon_memory_alloc(sizeof(ma_sound));
  if (MA_SUCCESS != ma_sound_init_from_file(&carbon_audio__engine, file, flags, 0, 0, sound)) {
    CBN_ERROR("Failed to load sound from file (`%s`)", file);
    carbon_memory_free(sound);
    return false;
  }
  *out_uid = carbon_slotmap_push(&carbon_audio__sounds, &sound);
  return true;
}

bool carbon_audio_load_from_file(const char *file, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_file_ex(file, out_uid, CARBON_AUDIO__SOUND_FLAGS);
}

bool carbon_audio_load_stream_from_file(const char *file, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_file_ex(file, out_uid, CARBON_AUDIO__SOUND_STREAM_FLAGS);
}

CBNINL bool carbon_audio__load_from_skap_ex(const char *name, const CBN_SKAP *skap_handle, CBN_Audio_UID *out_uid, ma_sound_flags flags) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  // Lookup binary from SKAP
  CBN_Binary bin;
  if (!carbon_skap_lookup(skap_handle, CARBON_SKAP_ASSET_TYPE_BINARY, name, &bin)) return false;
  // Create decoder
  ma_decoder *decoder = (ma_decoder *) carbon_memory_alloc(sizeof(ma_decoder));
  if (MA_SUCCESS != ma_decoder_init_memory(bin.data, bin.metadata.size, 0, decoder)) {
    CBN_ERROR("Failed to create decoder");
    carbon_memory_free(decoder);
    carbon_memory_free(bin.data);
    return false;
  }
  // Create sound
  ma_sound *sound = (ma_sound *) carbon_memory_alloc(sizeof(ma_sound));
  if (MA_SUCCESS != ma_sound_init_from_data_source(&carbon_audio__engine, decoder, flags, 0, sound)) {
    CBN_ERROR("Failed to load sound from audio buffer");
    carbon_memory_free(sound);
    carbon_memory_free(decoder);
    carbon_memory_free(bin.data);
    return false;
  }
  // Store decoder, binary and sound
  carbon_list_push(&carbon_audio__decoders, &decoder);
  carbon_list_push(&carbon_audio__binaries, &bin);
  *out_uid = carbon_slotmap_push(&carbon_audio__sounds, &sound);
  return true;
}

bool carbon_audio_load_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_skap_ex(name, skap_handle, out_uid, CARBON_AUDIO__SOUND_FLAGS);
}

bool carbon_audio_load_stream_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_skap_ex(name, skap_handle, out_uid, CARBON_AUDIO__SOUND_STREAM_FLAGS);
}

void carbon_audio_play(const CBN_Audio_UID uid) {
  ma_sound *sound = 0;
  if (!carbon_slotmap_lookup(&carbon_audio__sounds, uid, &sound)) return;
  ma_sound_seek_to_pcm_frame(sound, 0);
  ma_sound_start(sound);
}

void carbon_audio_stop(const CBN_Audio_UID uid) {
  ma_sound *sound = 0;
  if (!carbon_slotmap_lookup(&carbon_audio__sounds, uid, &sound)) return;
  ma_sound_stop(sound);
}

f32 carbon_audio_get_pitch(const CBN_Audio_UID uid) {
  ma_sound *sound = 0;
  if (!carbon_slotmap_lookup(&carbon_audio__sounds, uid, &sound)) return 0;
  return ma_sound_get_pitch(sound);
}

void carbon_audio_set_pitch(const CBN_Audio_UID uid, const f32 pitch) {
  ma_sound *sound = 0;
  if (!carbon_slotmap_lookup(&carbon_audio__sounds, uid, &sound)) return;
  ma_sound_set_pitch(sound, pitch);
}

void carbon_audio_shift_pitch(const CBN_Audio_UID uid) {
  static const i8 semitones[] = {-4, -2, 0, 2, 4, 7, 9};
  i8 semitone = semitones[carbon_rng_lcg_range(0, CARBON_ARRAY_LEN(semitones) - 1)];
  f32 pitch = carbon_math_pow(2, (f32) semitone/12);
  carbon_audio_set_pitch(uid, pitch);
}
