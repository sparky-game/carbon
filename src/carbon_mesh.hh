/*
  $$==================$$
  ||       Mesh       ||
  $$==================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef enum {
  CARBON_MESH_FACE_COMP_VERTEX,
  CARBON_MESH_FACE_COMP_TEXCOORD,
  CARBON_MESH_FACE_COMP_NORMAL,
  CARBON_MESH_FACE_COMPS
} CBN_Mesh_FaceComp;
static_assert(CARBON_MESH_FACE_COMPS == 3, "Face is defined as v/vt/vn");

typedef struct {
  usz vertices_count;
  usz texcoords_count;
  usz normals_count;
  usz faces_count;
} CBN_Mesh_Metadata;

CBNDEF_T(CBN_Mesh) {
  CBN_Vec3 *vertices;
  CBN_Vec2 *texcoords;
  CBN_Vec3 *normals;
  usz (*faces)[CARBON_MESH_FACE_COMPS][3];
  CBN_Mesh_Metadata metadata;
};
CBNDEF_AKA(cbn, Mesh, CBN_Mesh);

CBNDEF bool carbon_mesh_create_from_file(CBN_Mesh *m, const char *file);

CBNDEF void carbon_mesh_destroy(CBN_Mesh *m);

CBNDEF u32 carbon_mesh_compute_crc32(CBN_Mesh *m);

#ifdef __cplusplus
struct CBN_Mesh : CBN_Mesh_t {
  static cbn::Opt<CBN_Mesh> make(const char *file) {
    CBN_Mesh m = {};
    if (!carbon_mesh_create_from_file(&m, file)) return {};
    return m;
  }

  void Free(void) {
    carbon_mesh_destroy(this);
  }

  u32 CRC32(void) const;
};
#endif
