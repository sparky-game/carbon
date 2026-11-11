// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Audio_UID : CBN_Audio_UID_t {
  const CBN_Audio_UID &Unload(void) const { carbon_audio_unload(*this); return *this; }

  const CBN_Audio_UID &Play(void) const { carbon_audio_play(*this); return *this; }

  const CBN_Audio_UID &Stop(void) const { carbon_audio_stop(*this); return *this; }

  const CBN_Audio_UID &Resume(void) const { carbon_audio_resume(*this); return *this; }

  bool IsPlaying(void) const { return carbon_audio_is_playing(*this); }

  f32 GetPitch(void) const
  { return carbon_audio_get_pitch(*this); }
  const CBN_Audio_UID &SetPitch(f32 pitch) const
  { carbon_audio_set_pitch(*this, pitch); return *this; }
  const CBN_Audio_UID &ShiftPitch(void) const
  { carbon_audio_shift_pitch(*this); return *this; }

  const CBN_Audio_UID &SetLooping(bool yn) const
  { carbon_audio_set_looping(*this, yn); return *this; }

  f32 GetCursor(void) const { return carbon_audio_get_cursor(*this); }

  const CBN_Audio_UID &SetFade(f32 from, f32 to, u64 len_ms) const
  { carbon_audio_set_fade(*this, from, to, len_ms); return *this; }
  const CBN_Audio_UID &SetFadeIn(u64 len_ms) const
  { SetFade(0, 1, len_ms); return *this; }
  const CBN_Audio_UID &SetFadeOut(u64 len_ms) const
  { SetFade(-1, 0, len_ms); return *this; }

  f32 GetVolume(void) const
  { return carbon_audio_sound_get_volume(*this); }
  const CBN_Audio_UID &SetVolume(f32 volume) const
  { carbon_audio_sound_set_volume(*this, volume); return *this; }
};

struct CBN_Audio_GID : CBN_Audio_GID_t {
  f32 GetVolume(void) const
  { return carbon_audio_group_get_volume(*this); }
  const CBN_Audio_GID &SetVolume(f32 volume) const
  { carbon_audio_group_set_volume(*this, volume); return *this; }
};

namespace cbn::audio {
  inline Opt<UID> LoadFromFile(const char *file, const GID *gid = nullptr) {
    return meta::OptWrap<UID>(carbon_audio_load_from_file, file, gid);
  }

  inline Opt<UID> LoadFromFileStreaming(const char *file, const GID *gid = nullptr) {
    return meta::OptWrap<UID>(carbon_audio_load_from_file_streaming, file, gid);
  }

  inline Opt<UID> LoadFromSKAP(const char *name, const CBN_SKAP *skap, const GID *gid = nullptr) {
    return meta::OptWrap<UID>(carbon_audio_load_from_skap, name, skap, gid);
  }

  inline Opt<GID> NewGroup(void) { return meta::OptWrap<GID>(carbon_audio_new_group); }
}

#endif
