// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

static CBN_SlotMap carbon_mesh__library;

void carbon_mesh_manager_init(void) {
  carbon_mesh__library = carbon_slotmap_create(sizeof(CBN_Mesh *));
  CBN_INFO("Initialized mesh subsystem successfully");
}

void carbon_mesh_manager_shutdown(void) {
  if (carbon_mesh__library.size) {
    carbon_slotmap_foreach(CBN_Mesh *, carbon_mesh__library) {
      carbon_mesh_destroy(it.var);
      carbon_memory_free(it.var);
    }
  }
  carbon_slotmap_destroy(&carbon_mesh__library);
  CBN_INFO("Shutdowned mesh subsystem successfully");
}

bool carbon_mesh_manager_load_from_file(const char *file, CBN_Mesh_UID *out_uid) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  CBN_Mesh *mesh = (CBN_Mesh *) carbon_memory_alloc(sizeof(CBN_Mesh));
  if (!carbon_mesh_create_from_file(mesh, file)) {
    carbon_memory_free(mesh);
    return false;
  }
  *out_uid = carbon_slotmap_push(&carbon_mesh__library, &mesh);
  return true;
}

bool carbon_mesh_manager_load_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Mesh_UID *out_uid) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  CBN_Mesh *mesh = (CBN_Mesh *) carbon_memory_alloc(sizeof(CBN_Mesh));
  if (!carbon_skap_lookup(skap_handle, CARBON_SKAP_ASSET_TYPE_MESH, name, mesh)) {
    carbon_memory_free(mesh);
    return false;
  }
  *out_uid = carbon_slotmap_push(&carbon_mesh__library, &mesh);
  return true;
}

CBN_Mesh *carbon_mesh_manager_lookup(const CBN_Mesh_UID uid) {
  CBN_Mesh *mesh = 0;
  carbon_slotmap_lookup(&carbon_mesh__library, uid, &mesh);
  return mesh;
}
