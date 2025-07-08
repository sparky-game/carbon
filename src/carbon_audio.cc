// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CARBON_INLINE cbn::Opt<CBN_SlotMap_Key> carbon_audio__load_sound_ex(const char *file, u8 (*f)(const char *, CBN_SlotMap_Key *)) {
  CBN_SlotMap_Key key;
  if (!f(file, &key)) return {};
  return key;
}

cbn::Opt<CBN_SlotMap_Key> carbon_audio_load_sound(const char *file) {
  return carbon_audio__load_sound_ex(file, carbon_audio_load_sound_from_file);
}

cbn::Opt<CBN_SlotMap_Key> carbon_audio_load_sound_streaming(const char *file) {
  return carbon_audio__load_sound_ex(file, carbon_audio_load_sound_streaming_from_file);
}
