// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Sprite_UID : CBN_Sprite_UID_t {
  void Unload(void) const { carbon_sprite_manager_unload(*this); }

  CBN_Sprite *Lookup(void) const { return carbon_sprite_manager_lookup(*this); }
};

#endif
