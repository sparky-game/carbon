// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

cbn::Opt<CBN_Sprite_UID> carbon_sprite_manager_load(const char *file) {
  CBN_Sprite_UID uid;
  if (!carbon_sprite_manager_load_from_file(file, &uid)) return {};
  return uid;
}
