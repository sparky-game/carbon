// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

cbn::Opt<CBN_Mesh_UID> carbon_mesh_manager_load(const char *file) {
  CBN_Mesh_UID uid;
  if (!carbon_mesh_manager_load_from_file(file, &uid)) return {};
  return uid;
}
