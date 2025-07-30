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

#include "../vendor/stb_vorbis/stb_vorbis.c"
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
  CBN_ASSERT(res == MA_SUCCESS && "Failed to initialize the audio engine");
  carbon_audio__library = carbon_slotmap_create(sizeof(ma_sound *));
  CBN_INFO("Initialized audio subsystem successfully");
}

void carbon_audio_shutdown(void) {
  if (carbon_audio__library.size) {
    carbon_slotmap_foreach(ma_sound *, carbon_audio__library) {
      ma_sound_uninit(it.var);
      CBN_FREE(it.var);
    }
  }
  carbon_slotmap_destroy(&carbon_audio__library);
  ma_engine_uninit(&carbon_audio__engine);
  CBN_INFO("Shutdowned audio subsystem successfully");
}

CARBON_INLINE u8 carbon_audio__load_from_file_ex(const char *file, CBN_Audio_UID *out_uid, ma_sound_flags flags) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  ma_sound *sound = (ma_sound *) CBN_MALLOC(sizeof(ma_sound));
  CBN_ASSERT(sound && "failed to allocate memory");
  if (MA_SUCCESS != ma_sound_init_from_file(&carbon_audio__engine, file, flags, 0, 0, sound)) {
    CBN_ERROR("Failed to load sound from file (`%s`)", file);
    CBN_FREE(sound);
    return false;
  }
  *out_uid = carbon_slotmap_push(&carbon_audio__library, &sound);
  return true;
}

u8 carbon_audio_load_from_file(const char *file, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_file_ex(file, out_uid, MA_SOUND_FLAG_ASYNC);
}

u8 carbon_audio_load_stream_from_file(const char *file, CBN_Audio_UID *out_uid) {
  return carbon_audio__load_from_file_ex(file, out_uid, MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM);
}

void carbon_audio_play(const CBN_Audio_UID uid) {
  ma_sound *sound = 0;
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &sound)) return;
  ma_sound_seek_to_pcm_frame(sound, 0);
  ma_sound_start(sound);
}

void carbon_audio_stop(const CBN_Audio_UID uid) {
  ma_sound *sound = 0;
  if (!carbon_slotmap_lookup(&carbon_audio__library, uid, &sound)) return;
  ma_sound_stop(sound);
}
