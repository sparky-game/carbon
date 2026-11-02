// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Mesh : CBN_Mesh_t {
  static cbn::Opt<CBN_Mesh> New(const char *file) {
    return cbn::meta::OptWrap<CBN_Mesh>(carbon_mesh_create_from_file, file);
  }

  void Free(void) { carbon_mesh_destroy(this); }

  u32 CRC32(void) const { return carbon_mesh_compute_crc32(this); }

  CBN_Box GetBounds(void) const { return carbon_mesh_get_bounds(this); }
};

#endif
