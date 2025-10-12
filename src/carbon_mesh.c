// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_MESH__SZ_V  (m->metadata.vertices_count * sizeof(CBN_Vec3))
#define CARBON_MESH__SZ_VT (m->metadata.texcoords_count * sizeof(CBN_Vec2))
#define CARBON_MESH__SZ_VN (m->metadata.normals_count * sizeof(CBN_Vec3))
#define CARBON_MESH__SZ_F  (m->metadata.faces_count * CARBON_MESH_FACE_COMPS * 3 * sizeof(usz))

CARBON_INLINE bool carbon_mesh__parse_sizes_from_file(CBN_Mesh *m, const char *file) {
  CBN_StrBuilder sb = {0};
  if (!carbon_fs_read_entire_file(&sb, file)) {
    CBN_ERROR("OBJ file (`%$`) could not be read", $(file));
    return false;
  }
  CBN_StrView txt = carbon_strview_from_strbuilder(&sb);
  while (txt.size) {
    CBN_StrView line = carbon_strview_chop(&txt, '\n');
    if (carbon_strview_starts_with(line, carbon_strview_from_cstr("v ")))  ++m->metadata.vertices_count;
    if (carbon_strview_starts_with(line, carbon_strview_from_cstr("vt "))) ++m->metadata.texcoords_count;
    if (carbon_strview_starts_with(line, carbon_strview_from_cstr("vn "))) ++m->metadata.normals_count;
    if (carbon_strview_starts_with(line, carbon_strview_from_cstr("f ")))  ++m->metadata.faces_count;
  }
  carbon_strbuilder_free(&sb);
  return true;
}

CARBON_INLINE bool carbon_mesh__parse_data_from_file(CBN_Mesh *m, const char *file) {
  CBN_StrBuilder sb = {0};
  if (!carbon_fs_read_entire_file(&sb, file)) {
    CBN_ERROR("OBJ file (`%$`) could not be read", $(file));
    return false;
  }
  CBN_StrView txt = carbon_strview_from_strbuilder(&sb);
  usz i_v = 0, i_vt = 0, i_vn = 0, i_f = 0;
  while (txt.size) {
    CBN_StrView line = carbon_strview_chop(&txt, '\n');
    if (carbon_strview_starts_with(line, carbon_strview_from_cstr("v "))) {
      CBN_Vec3 v;
      sscanf(carbon_strview_to_cstr(line), "v %f %f %f", &v.x, &v.y, &v.z);
      m->vertices[i_v++] = v;
    }
    else if (carbon_strview_starts_with(line, carbon_strview_from_cstr("vt "))) {
      CBN_Vec2 vt;
      sscanf(carbon_strview_to_cstr(line), "vt %f %f", &vt.x, &vt.y);
      m->texcoords[i_vt++] = vt;
    }
    else if (carbon_strview_starts_with(line, carbon_strview_from_cstr("vn "))) {
      CBN_Vec3 vn;
      sscanf(carbon_strview_to_cstr(line), "vn %f %f %f", &vn.x, &vn.y, &vn.z);
      m->normals[i_vn++] = vn;
    }
    else if (carbon_strview_starts_with(line, carbon_strview_from_cstr("f "))) {
      usz v1  = 0, v2  = 0, v3  = 0;
      usz vt1 = 0, vt2 = 0, vt3 = 0;
      usz vn1 = 0, vn2 = 0, vn3 = 0;
      if (carbon_strview_contains(line, carbon_strview_from_cstr("//"))) {
        sscanf(carbon_strview_to_cstr(line), "f %zu//%zu %zu//%zu %zu//%zu",
               &v1, &vn1, &v2, &vn2, &v3, &vn3);
      }
      else if (carbon_strview_contains(line, carbon_strview_from_cstr("/"))) {
        sscanf(carbon_strview_to_cstr(line), "f %zu/%zu/%zu %zu/%zu/%zu %zu/%zu/%zu",
               &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
      }
      else sscanf(carbon_strview_to_cstr(line), "f %zu %zu %zu", &v1, &v2, &v3);
      m->faces[i_f][CARBON_MESH_FACE_COMP_VERTEX][0]   = v1  ? v1-1  : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_VERTEX][1]   = v2  ? v2-1  : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_VERTEX][2]   = v3  ? v3-1  : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_TEXCOORD][0] = vt1 ? vt1-1 : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_TEXCOORD][1] = vt2 ? vt2-1 : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_TEXCOORD][2] = vt3 ? vt3-1 : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_NORMAL][0]   = vn1 ? vn1-1 : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_NORMAL][1]   = vn2 ? vn2-1 : 0;
      m->faces[i_f][CARBON_MESH_FACE_COMP_NORMAL][2]   = vn3 ? vn3-1 : 0;
      ++i_f;
    }
  }
  carbon_strbuilder_free(&sb);
  return true;
}

bool carbon_mesh_create_from_file(CBN_Mesh *m, const char *file) {
  if (!m) {
    CBN_WARN("`m` is not a valid pointer, skipping creation");
    return false;
  }
  if (!carbon_mesh__parse_sizes_from_file(m, file)) return false;
  m->vertices  = m->metadata.vertices_count  ? carbon_memory_alloc(CARBON_MESH__SZ_V) : 0;
  m->texcoords = m->metadata.texcoords_count ? carbon_memory_alloc(CARBON_MESH__SZ_VT) : 0;
  m->normals   = m->metadata.normals_count   ? carbon_memory_alloc(CARBON_MESH__SZ_VN) : 0;
  m->faces     = m->metadata.faces_count     ? carbon_memory_alloc(CARBON_MESH__SZ_F) : 0;
  if (!carbon_mesh__parse_data_from_file(m, file)) return false;
  return true;
}

void carbon_mesh_destroy(CBN_Mesh *m) {
  if (!m) {
    CBN_WARN("`m` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(m->vertices);
  carbon_memory_free(m->texcoords);
  carbon_memory_free(m->normals);
  carbon_memory_free(m->faces);
}

u32 carbon_mesh_compute_crc32(CBN_Mesh *m) {
  if (!m) {
    CBN_WARN("`m` is not a valid pointer, skipping computation");
    return 0;
  }
  u32 c_v  = carbon_crypto_crc32((u8 *) m->vertices, CARBON_MESH__SZ_V);
  u32 c_vt = carbon_crypto_crc32((u8 *) m->texcoords, CARBON_MESH__SZ_VT);
  u32 c_vn = carbon_crypto_crc32((u8 *) m->normals, CARBON_MESH__SZ_VN);
  u32 c_f  = carbon_crypto_crc32((u8 *) m->faces, CARBON_MESH__SZ_F);
  u32 c = carbon_crypto_crc32_combine(c_v, c_vt, CARBON_MESH__SZ_VT);
  c = carbon_crypto_crc32_combine(c, c_vn, CARBON_MESH__SZ_VN);
  c = carbon_crypto_crc32_combine(c, c_f, CARBON_MESH__SZ_F);
  return c;
}
