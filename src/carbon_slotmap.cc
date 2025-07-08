// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

const char *CBN_SlotMap_Key::ToString(void) const {
  return carbon_slotmap_key_to_cstr(*this);
}
