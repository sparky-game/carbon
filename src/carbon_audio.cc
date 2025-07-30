// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

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
