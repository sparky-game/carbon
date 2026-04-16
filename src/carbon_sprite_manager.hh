// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

namespace cbn::sprite_mgr {
  cbn::Opt<CBN_Sprite_UID> LoadFromFile(const char *file) {
    CBN_Sprite_UID uid;
    if (!carbon_sprite_manager_load_from_file(file, &uid)) return {};
    return uid;
  }
}

#endif
