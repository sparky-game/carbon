// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Audio_UID : CBN_Audio_UID_t {
  void Unload(void) const { carbon_audio_unload(*this); }

  void Play(void) const { carbon_audio_play(*this); }

  void Stop(void) const { carbon_audio_stop(*this); }

  void Resume(void) const { carbon_audio_resume(*this); }

  bool IsPlaying(void) const { return carbon_audio_is_playing(*this); }

  f32 GetPitch(void) const { return carbon_audio_get_pitch(*this); }
  void SetPitch(f32 pitch) const { carbon_audio_set_pitch(*this, pitch); }
  void ShiftPitch(void) const { carbon_audio_shift_pitch(*this); }

  void SetLooping(bool yn) const { carbon_audio_set_looping(*this, yn); }

  f32 GetCursor(void) const { return carbon_audio_get_cursor(*this); }

  void SetFade(f32 target, u64 len_ms) const { carbon_audio_set_fade(*this, target, len_ms); }
};

#endif
