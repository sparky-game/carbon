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

void carbon_audio_init(void) {
  ma_result res = ma_engine_init(0, &carbon_audio__engine);
  CARBON_ASSERT(res == MA_SUCCESS && "Failed to initialize the audio engine");
}

void carbon_audio_shutdown(void) {
  ma_engine_uninit(&carbon_audio__engine);
}
