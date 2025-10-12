/*
**  $$==================$$
**  ||       Mesh       ||
**  $$==================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

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
  // usz edges_count;
} CBN_Mesh_Metadata;

typedef struct CBN_Mesh {
  CBN_Vec3 *vertices;
  CBN_Vec2 *texcoords;
  CBN_Vec3 *normals;
  usz (*faces)[CARBON_MESH_FACE_COMPS][3];
  // usz (*edges)[2];
  CBN_Mesh_Metadata metadata;
#ifdef __cplusplus
  /**
   * @see carbon_mesh_create_from_file
   */
  static cbn::Opt<CBN_Mesh> make(const char *file);
  /**
   * @see carbon_mesh_destroy
   */
  void Free(void);
  /**
   * @see carbon_mesh_compute_crc32
   */
  u32 CRC32(void) const;
#endif
} CBN_Mesh;

CBNDEF bool carbon_mesh_create_from_file(CBN_Mesh *m, const char *file);

CBNDEF void carbon_mesh_destroy(CBN_Mesh *m);

CBNDEF u32 carbon_mesh_compute_crc32(CBN_Mesh *m);

// Local Variables:
// mode: c++
// End:
