// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

// NOTE: we ignore here the `-Wswitch-enum` warning, because the
// miniaudio library doesn't handle switch-cases rigorously.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#endif

#define MINIAUDIO_IMPLEMENTATION
#include "../vendor/miniaudio/miniaudio.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

static ma_engine carbon_audio__engine;
static CBN_SlotMap carbon_audio__library;

void carbon_audio_init(void) {
  ma_result res = ma_engine_init(0, &carbon_audio__engine);
  CARBON_ASSERT(res == MA_SUCCESS && "Failed to initialize the audio engine");
  carbon_audio__library = carbon_slotmap_create(sizeof(ma_sound *));
  carbon_log_info("Initialized audio subsystem successfully");
}

void carbon_audio_shutdown(void) {
  if (carbon_audio__library.size) {
    carbon_slotmap_foreach(ma_sound *, carbon_audio__library) {
      ma_sound_uninit(it.var);
      CARBON_FREE(it.var);
    }
  }
  carbon_slotmap_destroy(&carbon_audio__library);
  ma_engine_uninit(&carbon_audio__engine);
  carbon_log_info("Shutdowned audio subsystem successfully");
}

CARBON_INLINE u8 carbon_audio__load_sound_from_file_ex(const char *file, CBN_SlotMap_Key *out_key, ma_sound_flags flags) {
  if (!out_key) {
    carbon_log_error("`out_key` must be a valid pointer");
    return false;
  }
  ma_sound *sound = (ma_sound *) CARBON_MALLOC(sizeof(ma_sound));
  CARBON_ASSERT(sound && "failed to allocate memory");
  if (MA_SUCCESS != ma_sound_init_from_file(&carbon_audio__engine, file, flags, 0, 0, sound)) {
    carbon_log_error("Failed to load sound from file (`%s`)", file);
    CARBON_FREE(sound);
    return false;
  }
  *out_key = carbon_slotmap_push(&carbon_audio__library, &sound);
  return true;
}

u8 carbon_audio_load_sound_from_file(const char *file, CBN_SlotMap_Key *out_key) {
  return carbon_audio__load_sound_from_file_ex(file, out_key, MA_SOUND_FLAG_ASYNC);
}

u8 carbon_audio_load_sound_streaming_from_file(const char *file, CBN_SlotMap_Key *out_key) {
  return carbon_audio__load_sound_from_file_ex(file, out_key, MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM);
}

void carbon_audio_play_sound(const CBN_SlotMap_Key key) {
  ma_sound *sound = 0;
  if (carbon_slotmap_lookup(&carbon_audio__library, key, &sound)) {
    ma_sound_seek_to_pcm_frame(sound, 0);
    ma_sound_start(sound);
  }
}
