// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Mesh_UID : CBN_Mesh_UID_t {
  void Unload(void) const { carbon_mesh_manager_unload(*this); }

  CBN_Mesh *Lookup(void) const { return carbon_mesh_manager_lookup(*this); }
};

namespace cbn::mesh_mgr {
  inline Opt<UID> LoadFromFile(const char *file) {
    return meta::OptWrap<UID>(carbon_mesh_manager_load_from_file, file);
  }

  inline Opt<UID> LoadFromSKAP(const char *name, const CBN_SKAP *skap) {
    return meta::OptWrap<UID>(carbon_mesh_manager_load_from_skap, name, skap);
  }
}

#endif
