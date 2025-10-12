// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

cbn::Opt<CBN_Mesh> CBN_Mesh::make(const char *file) {
  CBN_Mesh m = {};
  if (!carbon_mesh_create_from_file(&m, file)) return {};
  return m;
}

void CBN_Mesh::Free(void) {
  carbon_mesh_destroy(this);
}
