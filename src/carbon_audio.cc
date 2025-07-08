// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CARBON_STATIC_ASSERT(CARBON_TYPE_IS_SAME(decltype(carbon_audio_load_sound_from_file), decltype(carbon_audio_load_sound_streaming_from_file)), "Expected types to be equal");

CARBON_INLINE cbn::Opt<CBN_Audio_UID> carbon_audio__load_sound_ex(const char *file, decltype(carbon_audio_load_sound_from_file) f) {
  CBN_SlotMap_Key key;
  if (!f(file, &key)) return {};
  return key;
}

cbn::Opt<CBN_Audio_UID> carbon_audio_load_sound(const char *file) {
  return carbon_audio__load_sound_ex(file, carbon_audio_load_sound_from_file);
}

cbn::Opt<CBN_Audio_UID> carbon_audio_load_sound_streaming(const char *file) {
  return carbon_audio__load_sound_ex(file, carbon_audio_load_sound_streaming_from_file);
}
