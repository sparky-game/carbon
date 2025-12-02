// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

bool CBN_SKAP::Create(const char *decl, const char *skap) {
  return carbon_skap_create(decl, skap);
}

cbn::Opt<CBN_SKAP> CBN_SKAP::make(const char *skap) {
  CBN_SKAP handle;
  if (!carbon_skap_open(skap, &handle)) return {};
  return handle;
}

void CBN_SKAP::Free(void) {
  carbon_skap_close(this);
}

void CBN_SKAP::Print(void) const {
  carbon_skap_print(this);
}

usz CBN_SKAP::Count(void) const {
  return carbon_skap_count(this);
}

cbn::Opt<CBN_Sprite_UID> CBN_SKAP::LoadSprite(const char *name) const {
  CBN_Sprite_UID uid;
  if (!carbon_sprite_manager_load_from_skap(name, this, &uid)) return {};
  return uid;
}

cbn::Opt<CBN_Mesh_UID> CBN_SKAP::LoadMesh(const char *name) const {
  CBN_Mesh_UID uid;
  if (!carbon_mesh_manager_load_from_skap(name, this, &uid)) return {};
  return uid;
}

cbn::Opt<CBN_Audio_UID> CBN_SKAP::LoadAudio(const char *name) const {
  CBN_Audio_UID uid;
  if (!carbon_audio_load_from_skap(name, this, &uid)) return {};
  return uid;
}

cbn::Opt<CBN_Audio_UID> CBN_SKAP::LoadAudioStream(const char *name) const {
  CBN_Audio_UID uid;
  if (!carbon_audio_load_stream_from_skap(name, this, &uid)) return {};
  return uid;
}
