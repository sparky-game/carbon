// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

bool cbn::SKAP::Create(const char *decl, const char *skap) {
  return carbon_skap_create(decl, skap);
}

cbn::Opt<cbn::SKAP> cbn::SKAP::make(const char *skap) {
  cbn::SKAP handle;
  if (!carbon_skap_open(skap, &handle)) return {};
  return handle;
}

void cbn::SKAP::Free(void) {
  carbon_skap_close(this);
}

void cbn::SKAP::Print(void) const {
  carbon_skap_print(this);
}

usz cbn::SKAP::Count(void) const {
  return carbon_skap_count(this);
}

cbn::Opt<CBN_Sprite_UID> cbn::SKAP::LoadSprite(const char *name) const {
  CBN_Sprite_UID uid;
  if (!carbon_sprite_manager_load_from_skap(name, this, &uid)) return {};
  return uid;
}

cbn::Opt<CBN_Mesh_UID> cbn::SKAP::LoadMesh(const char *name) const {
  CBN_Mesh_UID uid;
  if (!carbon_mesh_manager_load_from_skap(name, this, &uid)) return {};
  return uid;
}

cbn::Opt<CBN_Audio_UID> cbn::SKAP::LoadAudio(const char *name) const {
  CBN_Audio_UID uid;
  if (!carbon_audio_load_from_skap(name, this, &uid)) return {};
  return uid;
}

cbn::Opt<CBN_Audio_UID> cbn::SKAP::LoadAudioStream(const char *name) const {
  CBN_Audio_UID uid;
  if (!carbon_audio_load_stream_from_skap(name, this, &uid)) return {};
  return uid;
}
