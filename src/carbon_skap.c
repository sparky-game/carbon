// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_SKAP__HEX_SPEC "%#012zx"
#define CARBON_SKAP__DECL_FILE_MAX_LINE_LEN 256
#define CARBON_SKAP__DECL_FILE_MAX_PATH_LEN 127
#define CARBON_SKAP__DECL_FILE_MAX_TYPE_LEN 63

typedef struct {
  CBN_SKAP_AssetType type;
  union {
    CBN_Image_Metadata as_img;
    struct { usz size; } as_bin;
    CBN_Mesh_Metadata as_mesh;
    CBN_Font_Metadata as_font;
  };
} CBN_SKAP_AssetMetadata;

typedef struct {
  char name[CARBON_SKAP__DECL_FILE_MAX_LINE_LEN];
  usz blob_offset;
  usz blob_size;
  u32 checksum;
  CBN_SKAP_AssetMetadata metadata;
} CBN_SKAP_AssetIdx;

typedef struct {
  char path[CARBON_SKAP__DECL_FILE_MAX_PATH_LEN + 1];
  char type[CARBON_SKAP__DECL_FILE_MAX_TYPE_LEN + 1];
  CBN_StrList assets;
} CBN_SKAP_AssetGroup;

typedef void (*CBN_SKAP_AssetDestroyFunc)(void *);
typedef void (*CBN_SKAP_AssetAppendIdxFunc)(CBN_SKAP_AssetIdx *);
typedef void (*CBN_SKAP_AssetAppendBlobFunc)(void *, CBN_SKAP_AssetIdx *, FILE *);
typedef bool (*CBN_SKAP_AssetLookupBlobFunc)(const CBN_SKAP *, CBN_SKAP_AssetIdx *, void *);

static CBN_List carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_COUNT];
static CBN_List carbon_skap__asset_idxs[CARBON_SKAP_ASSET_TYPE_COUNT];
static CBN_List carbon_skap__asset_idx_locs[CARBON_SKAP_ASSET_TYPE_COUNT];

///////////////////// BINARY /////////////////////////////////////////

#define CARBON_SKAP__HINTS_BINARY(h, c)         \
  h(c, "binary") h(c, "bin") h(c, "bins")

CBNINL void carbon_skap__destroy_binary(void *p) { carbon_memory_free(((CBN_Span *) p)->data); }

CBNINL void carbon_skap__append_idx_binary(CBN_SKAP_AssetIdx *idx) {
  CBN_List data = carbon_list_create(sizeof(u8));
  CBN_ASSERT(carbon_fs_read_entire_file(&data, idx->name));
  CBN_Span view = carbon_span_from_list(&data);
  carbon_list_push(&carbon_skap__assets[idx->metadata.type], &view);
  idx->metadata.as_bin.size = view.size;
}

CBNINL void carbon_skap__append_blob_binary(void *p, CBN_SKAP_AssetIdx *idx, FILE *fd) {
  CBN_Span *asset = p;
  idx->blob_size = idx->metadata.as_bin.size;
  idx->checksum = carbon_crypto_crc32(asset->data, idx->blob_size);
  fwrite(asset->data, idx->blob_size, 1, fd);
}

CBNINL bool carbon_skap__lookup_binary(const CBN_SKAP *handle, CBN_SKAP_AssetIdx *idx, void *p) {
  CBN_Span *out_blob = p;
  u8 *p_data = carbon_memory_alloc(idx->blob_size);
  fseek(handle->fd, idx->blob_offset, SEEK_SET);
  fread(p_data, idx->blob_size, 1, handle->fd);
  fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
  if (idx->checksum != carbon_crypto_crc32(p_data, idx->blob_size)) {
    CBN_ERROR("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
    carbon_memory_free(p_data);
    return false;
  }
  out_blob->data = p_data;
  out_blob->size = idx->metadata.as_bin.size;
  return true;
}

///////////////////// IMAGE //////////////////////////////////////////

#define CARBON_SKAP__HINTS_IMAGE(h, c)          \
  h(c, "image") h(c, "img") h(c, "imgs")

CBNINL void carbon_skap__append_idx_image(CBN_SKAP_AssetIdx *idx) {
  CBN_Image asset = carbon_image_read_from_file(idx->name);
  carbon_list_push(&carbon_skap__assets[idx->metadata.type], &asset);
  idx->metadata.as_img = asset.metadata;
}

CBNINL void carbon_skap__append_blob_image(void *p, CBN_SKAP_AssetIdx *idx, FILE *fd) {
  CBN_Image *asset = p;
  idx->blob_size = idx->metadata.as_img.width * idx->metadata.as_img.height * idx->metadata.as_img.channels;
  idx->checksum = carbon_crypto_crc32(asset->data, idx->blob_size);
  fwrite(asset->data, idx->blob_size, 1, fd);
}

CBNINL bool carbon_skap__lookup_image(const CBN_SKAP *handle, CBN_SKAP_AssetIdx *idx, void *p) {
  CBN_Image *out_blob = p;
  u8 *p_data = carbon_memory_alloc(idx->blob_size);
  fseek(handle->fd, idx->blob_offset, SEEK_SET);
  fread(p_data, idx->blob_size, 1, handle->fd);
  fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
  if (idx->checksum != carbon_crypto_crc32(p_data, idx->blob_size)) {
    CBN_ERROR("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
    carbon_memory_free(p_data);
    return false;
  }
  out_blob->data = p_data;
  out_blob->metadata = idx->metadata.as_img;
  return true;
}

///////////////////// MESH ///////////////////////////////////////////

#define CARBON_SKAP__HINTS_MESH(h, c)           \
  h(c, "mesh")

CBNINL void carbon_skap__append_idx_mesh(CBN_SKAP_AssetIdx *idx) {
  CBN_Mesh asset = {0};
  CBN_ASSERT(carbon_mesh_create_from_file(&asset, idx->name));
  carbon_list_push(&carbon_skap__assets[idx->metadata.type], &asset);
  idx->metadata.as_mesh = asset.metadata;
}

CBNINL void carbon_skap__append_blob_mesh(void *p, CBN_SKAP_AssetIdx *idx, FILE *fd) {
  CBN_Mesh *asset = p;
  usz sz_v  = idx->metadata.as_mesh.vertices_count * sizeof(CBN_Vec3);
  usz sz_vt = idx->metadata.as_mesh.texcoords_count * sizeof(CBN_Vec2);
  usz sz_vn = idx->metadata.as_mesh.normals_count * sizeof(CBN_Vec3);
  usz sz_f  = idx->metadata.as_mesh.faces_count * CARBON_MESH_FACE_COMPS * 3 * sizeof(usz);
  idx->blob_size = sz_v + sz_vt + sz_vn + sz_f;
  idx->checksum = carbon_mesh_compute_crc32(asset);
  fwrite(asset->vertices, sz_v, 1, fd);
  fwrite(asset->texcoords, sz_vt, 1, fd);
  fwrite(asset->normals, sz_vn, 1, fd);
  fwrite(asset->faces, sz_f, 1, fd);
}

CBNINL bool carbon_skap__lookup_mesh(const CBN_SKAP *handle, CBN_SKAP_AssetIdx *idx, void *p) {
  CBN_Mesh *out_blob = p;
  usz sz_v  = idx->metadata.as_mesh.vertices_count * sizeof(CBN_Vec3);
  usz sz_vt = idx->metadata.as_mesh.texcoords_count * sizeof(CBN_Vec2);
  usz sz_vn = idx->metadata.as_mesh.normals_count * sizeof(CBN_Vec3);
  usz sz_f = idx->metadata.as_mesh.faces_count * CARBON_MESH_FACE_COMPS * 3 * sizeof(usz);
  CBN_Mesh asset = {
    .vertices = carbon_memory_alloc(sz_v),
    .texcoords = carbon_memory_alloc(sz_vt),
    .normals = carbon_memory_alloc(sz_vn),
    .faces = carbon_memory_alloc(sz_f),
    .metadata = idx->metadata.as_mesh
  };
  fseek(handle->fd, idx->blob_offset, SEEK_SET);
  fread(asset.vertices, sz_v, 1, handle->fd);
  fread(asset.texcoords, sz_vt, 1, handle->fd);
  fread(asset.normals, sz_vn, 1, handle->fd);
  fread(asset.faces, sz_f, 1, handle->fd);
  fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
  if (idx->checksum != carbon_mesh_compute_crc32(&asset)) {
    CBN_ERROR("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
    carbon_mesh_destroy(&asset);
    return false;
  }
  out_blob->vertices = asset.vertices;
  out_blob->texcoords = asset.texcoords;
  out_blob->normals = asset.normals;
  out_blob->faces = asset.faces;
  out_blob->metadata = asset.metadata;
  return true;
}

/////////////////////// FONT /////////////////////////////////////////

#define CARBON_SKAP__FONT_SIZE 64

#define CARBON_SKAP__HINTS_FONT(h, c)           \
  h(c, "font") h(c, "ttf")

CBNINL void carbon_skap__destroy_font(void *p) { CARBON_UNUSED(p); }

CBNINL void carbon_skap__append_idx_font(CBN_SKAP_AssetIdx *idx) {
  CBN_Font asset = carbon_font_create_from_file(idx->name, CARBON_SKAP__FONT_SIZE);
  carbon_list_push(&carbon_skap__assets[idx->metadata.type], &asset);
  idx->metadata.as_font = asset.metadata;
}

CBNINL void carbon_skap__append_blob_font(void *p, CBN_SKAP_AssetIdx *idx, FILE *fd) {
  CBN_Font *asset = p;
  idx->blob_size = sizeof(asset->data);
  idx->checksum = carbon_crypto_crc32(asset->data, idx->blob_size);
  fwrite(asset->data, idx->blob_size, 1, fd);
}

CBNINL bool carbon_skap__lookup_font(const CBN_SKAP *handle, CBN_SKAP_AssetIdx *idx, void *p) {
  CBN_Font *out_blob = p;
  fseek(handle->fd, idx->blob_offset, SEEK_SET);
  fread(out_blob->data, idx->blob_size, 1, handle->fd);
  fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
  if (idx->checksum != carbon_crypto_crc32(out_blob->data, idx->blob_size)) {
    CBN_ERROR("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
    return false;
  }
  out_blob->metadata = idx->metadata.as_font;
  return true;
}

//////////////////////////////////////////////////////////////////////

/*
  x(t, v, d, s, h, ai, ab, lb)
  ----------------------------
  t  :: TYPE
  v  :: ENUM_VALUE
  d  :: DESTROY_FN
  s  :: STRING
  h  :: HINTS_MACRO
  ai :: APPEND_IDX_FN
  ab :: APPEND_BLOB_FN
  lb :: LOOKUP_BLOB_FN
*/
#define CARBON_SKAP__ASSET_TYPES                \
  x(CBN_Span,                                   \
  CARBON_SKAP_ASSET_TYPE_BINARY,                \
  carbon_skap__destroy_binary,                  \
  "binaries",                                   \
  CARBON_SKAP__HINTS_BINARY,                    \
  carbon_skap__append_idx_binary,               \
  carbon_skap__append_blob_binary,              \
  carbon_skap__lookup_binary                    \
  )                                             \
  x(CBN_Image,                                  \
  CARBON_SKAP_ASSET_TYPE_IMAGE,                 \
  carbon_image_destroy,                         \
  "images",                                     \
  CARBON_SKAP__HINTS_IMAGE,                     \
  carbon_skap__append_idx_image,                \
  carbon_skap__append_blob_image,               \
  carbon_skap__lookup_image                     \
  )                                             \
  x(CBN_Mesh,                                   \
  CARBON_SKAP_ASSET_TYPE_MESH,                  \
  carbon_mesh_destroy,                          \
  "meshes",                                     \
  CARBON_SKAP__HINTS_MESH,                      \
  carbon_skap__append_idx_mesh,                 \
  carbon_skap__append_blob_mesh,                \
  carbon_skap__lookup_mesh                      \
  )                                             \
  x(CBN_Font,                                   \
  CARBON_SKAP_ASSET_TYPE_FONT,                  \
  carbon_skap__destroy_font,                    \
  "fonts",                                      \
  CARBON_SKAP__HINTS_FONT,                      \
  carbon_skap__append_idx_font,                 \
  carbon_skap__append_blob_font,                \
  carbon_skap__lookup_font                      \
  )

#define x(t, v, d, s, h, ai, ab, lb) t: v,
#define carbon_skap__type(t) _Generic((t), CARBON_SKAP__ASSET_TYPES)
#undef x

static usz carbon_skap__type2size[] = {
#define x(t, v, d, s, h, ai, ab, lb) [v] = sizeof(t),
  CARBON_SKAP__ASSET_TYPES
#undef x
};
static_assert(CARBON_ARRAY_LEN(carbon_skap__type2size) == CARBON_SKAP_ASSET_TYPE_COUNT,
              "@new_asset_type: add entry to CARBON_SKAP__ASSET_TYPES");

static CBN_SKAP_AssetDestroyFunc carbon_skap__type2destroy[] = {
#define x(t, v, d, s, h, ai, ab, lb) [v] = (CBN_SKAP_AssetDestroyFunc) d,
  CARBON_SKAP__ASSET_TYPES
#undef x
};
static_assert(CARBON_ARRAY_LEN(carbon_skap__type2destroy) == CARBON_SKAP_ASSET_TYPE_COUNT,
              "@new_asset_type: add entry to CARBON_SKAP__ASSET_TYPES");

static char * const carbon_skap__type2str[] = {
#define x(t, v, d, s, h, ai, ab, lb) [v] = s,
  CARBON_SKAP__ASSET_TYPES
#undef x
};
static_assert(CARBON_ARRAY_LEN(carbon_skap__type2str) == CARBON_SKAP_ASSET_TYPE_COUNT,
              "@new_asset_type: add entry to CARBON_SKAP__ASSET_TYPES");

static CBN_SKAP_AssetAppendIdxFunc carbon_skap__type2aifn[] = {
#define x(t, v, d, s, h, ai, ab, lb) [v] = ai,
  CARBON_SKAP__ASSET_TYPES
#undef x
};
static_assert(CARBON_ARRAY_LEN(carbon_skap__type2aifn) == CARBON_SKAP_ASSET_TYPE_COUNT,
              "@new_asset_type: add entry to CARBON_SKAP__ASSET_TYPES");

static CBN_SKAP_AssetAppendBlobFunc carbon_skap__type2abfn[] = {
#define x(t, v, d, s, h, ai, ab, lb) [v] = ab,
  CARBON_SKAP__ASSET_TYPES
#undef x
};
static_assert(CARBON_ARRAY_LEN(carbon_skap__type2abfn) == CARBON_SKAP_ASSET_TYPE_COUNT,
              "@new_asset_type: add entry to CARBON_SKAP__ASSET_TYPES");

static CBN_SKAP_AssetLookupBlobFunc carbon_skap__type2lbfn[] = {
#define x(t, v, d, s, h, ai, ab, lb) [v] = lb,
  CARBON_SKAP__ASSET_TYPES
#undef x
};
static_assert(CARBON_ARRAY_LEN(carbon_skap__type2lbfn) == CARBON_SKAP_ASSET_TYPE_COUNT,
              "@new_asset_type: add entry to CARBON_SKAP__ASSET_TYPES");

CBNINL CBN_SKAP_AssetType carbon_skap__str2type(const char *str) {
  for (CBN_SKAP_AssetType i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    if (!carbon_string_cmp(str, carbon_skap__type2str[i])) return i;
  }
  CARBON_UNREACHABLE;
  return CARBON_SKAP_ASSET_TYPE_COUNT;
}

CBNINL char *carbon_skap__str2hint(const char *str) {
#define x(t, v, d, s, h, ai, ab, lb) h(hh, s)
#define hh(c, s) if (!carbon_string_cmp(str, s)) return c;
  CARBON_SKAP__ASSET_TYPES;
#undef hh
#undef x
  return 0;
}

//////////////////////////////////////////////////////////////////////

CBNINL bool carbon_skap__parse_decl_file(FILE *decl_fd, CBN_List *asset_groups) {
  bool first = true;
  bool prev_line_was_new_group = false;
  usz line_n = 1;
  char line[CARBON_SKAP__DECL_FILE_MAX_LINE_LEN];
  while (fgets(line, CARBON_SKAP__DECL_FILE_MAX_LINE_LEN, decl_fd)) {
    line[strcspn(line, "\r")] = 0;
    line[strcspn(line, "\n")] = 0;
    CBN_SKAP_AssetGroup ag;
    // CBN_DEBUG("%s", line);
    // Empty or commented line
    if (line[0] == 0 || (line[0] == '/' && line[1] == '/')) {
      ++line_n;
      continue;
    }
    // Whitespace-only line
    bool all_is_whitespaced = true;
    char *line_ptr = line;
    while (*line_ptr) {
      if (!isspace(*line_ptr)) {
        all_is_whitespaced = false;
        break;
      }
      ++line_ptr;
    }
    if (all_is_whitespaced) {
      ++line_n;
      continue;
    }
    // New group
    else if (line[0] == '[') {
      if (prev_line_was_new_group) {
        CBN_ERROR("on line %zu, syntax error; expected expression `<ASSET>`", line_n);
        return false;
      }
      if (first) first = false;
      if (2 != sscanf(line,
                      "[\"%"
                      CARBON_QUOTE(CARBON_SKAP__DECL_FILE_MAX_PATH_LEN)
                      "[^\"]\" as %"
                      CARBON_QUOTE(CARBON_SKAP__DECL_FILE_MAX_TYPE_LEN)
                      "[^]]",
                      ag.path, ag.type)) {
        CBN_ERROR("on line %zu, syntax error; expected expression `[\"<PATH>\" as <TYPE>]`", line_n);
        return false;
      }
      // Check whether type is valid
      bool type_is_valid = false;
      for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
        if (!carbon_string_cmp(ag.type, carbon_skap__type2str[i])) {
          type_is_valid = true;
          break;
        }
      }
      if (!type_is_valid) {
        const char *hint = carbon_skap__str2hint(ag.type);
        if (hint) CBN_ERROR("on line %zu, syntax error; type `%s` not recognized, maybe you meant `%s`?", line_n, ag.type, hint);
        else      CBN_ERROR("on line %zu, syntax error; type `%s` not recognized", line_n, ag.type);
        return false;
      }
      // Check whether path is valid
      if (ag.path[carbon_string_len(ag.path) - 1] != '/') {
        CBN_ERROR("on line %zu, syntax error; path needs to end with `/` char", line_n);
        return false;
      }
      ag.assets = carbon_strlist_create(true);
      carbon_list_push(asset_groups, &ag);
      prev_line_was_new_group = true;
      // CBN_DEBUG("  -> Path: `%s`, Type: `%s`", ag.path, ag.type);
    }
    // New asset in previous group
    else {
      if (first) {
        CBN_ERROR("on line %zu, asset without group declaration makes no sense", line_n);
        return false;
      }
      carbon_list_pop_back(asset_groups, &ag);
      carbon_strlist_push(&ag.assets, line);
      carbon_list_push(asset_groups, &ag);
      prev_line_was_new_group = false;
      // CBN_DEBUG("  -> Asset: `%s`", line);
    }
    ++line_n;
  }
  return true;
}

CBNINL bool carbon_skap__check_decl_assets(const char *decl, CBN_List *asset_groups) {
  const char *cwd = carbon_fs_get_curr_directory();
  CBN_ASSERT(carbon_fs_change_directory(carbon_fs_get_directory(decl)));
  bool status = true;
  carbon_list_foreach(CBN_SKAP_AssetGroup, ag_it, *asset_groups) {
    carbon_strlist_foreach(ag_it.var.assets) {
      const char *asset_name = carbon_string_fmt("%s%s", ag_it.var.path, carbon_strview_to_cstr(it.sv));
      if (!carbon_fs_exists(asset_name)) {
        CBN_ERROR("asset declared as `%s` doesn't exist", asset_name);
        status = false;
      }
    }
  }
  CBN_ASSERT(carbon_fs_change_directory(cwd));
  return status;
}

CBNINL void carbon_skap__destroy_asset_groups(CBN_List *asset_groups) {
  // CBN_DEBUG("asset_groups->size = %$", $(asset_groups->size));
  carbon_list_foreach(CBN_SKAP_AssetGroup, *asset_groups) {
    // CBN_DEBUG("asset_groups[%$].assets.size = %$", $(it.i), $(it.var.assets.size));
    carbon_strlist_destroy(&it.var.assets);
  }
  carbon_list_destroy(asset_groups);
}

CBNINL void carbon_skap__create_global_lists(void) {
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_skap__assets[i] = carbon_list_create(carbon_skap__type2size[i]);
    carbon_skap__asset_idxs[i] = carbon_list_create(sizeof(CBN_SKAP_AssetIdx));
    carbon_skap__asset_idx_locs[i] = carbon_list_create(sizeof(usz));
  }
}

CBNINL void carbon_skap__destroy_global_lists(void) {
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_list_foreach_raw(carbon_skap__assets[i]) {
      carbon_skap__type2destroy[i](it.ptr);
    }
    carbon_list_destroy(&carbon_skap__assets[i]);
    carbon_list_destroy(&carbon_skap__asset_idxs[i]);
    carbon_list_destroy(&carbon_skap__asset_idx_locs[i]);
  }
}

CBNINL void carbon_skap__append_header(FILE *fd) {
  CBN_SKAP_Header header = {
    .signature = {'S', 'K', 'A', 'P'},
    .fmt_ver = 1,
    .build_ver = carbon_time_get_yyyymmddhhmmss()
  };
  fwrite(&header, sizeof(header), 1, fd);
}

CBNINL void carbon_skap__append_type_counters(FILE *fd, CBN_List *asset_groups) {
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    usz asset_count = 0;
    carbon_list_foreach(CBN_SKAP_AssetGroup, *asset_groups) {
      if (!carbon_string_cmp(it.var.type, carbon_skap__type2str[i])) {
        asset_count += it.var.assets.size;
      }
    }
    fwrite(&asset_count, sizeof(asset_count), 1, fd);
  }
}

CBNINL void carbon_skap__append_idxs(FILE *fd, const char *decl, CBN_List *asset_groups) {
  for (CBN_SKAP_AssetType i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_list_foreach(CBN_SKAP_AssetGroup, ag_it, *asset_groups) {
      if (carbon_string_cmp(ag_it.var.type, carbon_skap__type2str[i])) continue;
      carbon_strlist_foreach(ag_it.var.assets) {
        const char *cwd = carbon_fs_get_curr_directory();
        CBN_ASSERT(carbon_fs_change_directory(carbon_fs_get_directory(decl)));
        const char *asset_name = carbon_string_fmt("%s%s", ag_it.var.path, carbon_strview_to_cstr(it.sv));
        CBN_SKAP_AssetIdx idx = {0};
        idx.metadata.type = i;
        carbon_string_sfmt(idx.name, sizeof(idx.name), "%s", asset_name);
        carbon_skap__type2aifn[i](&idx);
        carbon_list_push(&carbon_skap__asset_idxs[i], &idx);
        usz idx_loc = ftell(fd);
        carbon_list_push(&carbon_skap__asset_idx_locs[i], &idx_loc);
        CBN_ASSERT(carbon_fs_change_directory(cwd));
        fwrite(&idx, sizeof(idx), 1, fd);
      }
    }
  }
}

CBNINL void carbon_skap__append_blobs(const char *skap, FILE *fd) {
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_list_foreach_raw(carbon_skap__assets[i]) {
      CBN_SKAP_AssetIdx *idx = &carbon_list_at_raw(CBN_SKAP_AssetIdx, carbon_skap__asset_idxs[i], it.i);
      usz *loc = &carbon_list_at_raw(usz, carbon_skap__asset_idx_locs[i], it.i);
      usz blob_loc = ftell(fd);
      idx->blob_offset = blob_loc;
      carbon_skap__type2abfn[i](it.ptr, idx, fd);
      usz next_loc = ftell(fd);
      fseek(fd, *loc, SEEK_SET);
      fwrite(idx, sizeof(*idx), 1, fd);
      fseek(fd, next_loc, SEEK_SET);
      carbon_println("  WRITE   %s -> %s @ [" CARBON_SKAP__HEX_SPEC "]+(" CARBON_SKAP__HEX_SPEC ")",
                     idx->name, skap, idx->blob_offset, idx->blob_size);
    }
  }
}

bool carbon_skap_create(const char *decl, const char *skap) {
  if (!carbon_fs_exists(decl) || !carbon_fs_is_regular_file(decl)) {
    CBN_ERROR("there is no SKAP declarations file named `%s`", decl);
    return false;
  }
  CBN_List asset_groups = carbon_list_create(sizeof(CBN_SKAP_AssetGroup));
  FILE *decl_fd = fopen(decl, "r");
  if (!carbon_skap__parse_decl_file(decl_fd, &asset_groups)) {
    CBN_ERROR("SKAP declarations file (`%s`) has syntax errors and can't be parsed correctly", decl);
    carbon_skap__destroy_asset_groups(&asset_groups);
    return false;
  }
  fclose(decl_fd);
  if (!carbon_skap__check_decl_assets(decl, &asset_groups)) {
    CBN_ERROR("SKAP declarations file (`%s`) has semantic errors and can't be interpreted correctly", decl);
    carbon_skap__destroy_asset_groups(&asset_groups);
    return false;
  }
  if (carbon_fs_exists(skap) && !carbon_fs_rename(skap, carbon_string_fmt("%s.old", skap))) return false;
  carbon_skap__create_global_lists();
  FILE *skap_fd = fopen(skap, "wb");
  carbon_skap__append_header(skap_fd);
  carbon_skap__append_type_counters(skap_fd, &asset_groups);
  carbon_skap__append_idxs(skap_fd, decl, &asset_groups);
  carbon_skap__append_blobs(skap, skap_fd);
  fclose(skap_fd);
  carbon_skap__destroy_global_lists();
  carbon_skap__destroy_asset_groups(&asset_groups);
  return true;
}

bool carbon_skap_open(const char *skap, CBN_SKAP *out_handle) {
  if (!out_handle) {
    CBN_ERROR("`out_handle` must be a valid pointer");
    return false;
  }
  if (!carbon_fs_exists(skap)) {
    CBN_ERROR("there is no SKAP file named `%s`", skap);
    return false;
  }
  carbon_memory_set(out_handle, 0, sizeof(*out_handle));
  out_handle->fd = fopen(skap, "rb");
  // Read header
  fread(&out_handle->header, sizeof(out_handle->header), 1, out_handle->fd);
  if (carbon_string_cmp_n(out_handle->header.signature, "SKAP", 4)) {
    CBN_ERROR("file `%s` is not a valid SKAP", skap);
    fclose(out_handle->fd);
    return false;
  }
  if (out_handle->header.fmt_ver != 1) {
    CBN_ERROR("file `%s` is SKAP, but it's using a different fmt version (%u) from the expected one (1)", skap, out_handle->header.fmt_ver);
    fclose(out_handle->fd);
    return false;
  }
  // Read type_counters
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    fread(&out_handle->type_counters[i], sizeof(usz), 1, out_handle->fd);
    // CBN_DEBUG("out_handle->type_counters[%zu] = %zu", i, out_handle->type_counters[i]);
  }
  // Read idxs
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    out_handle->idxs[i] = carbon_hashmap_create(CARBON_SKAP__DECL_FILE_MAX_LINE_LEN,
                                                sizeof(CBN_SKAP_AssetIdx),
                                                carbon_hashmap_hash_mem,
                                                carbon_hashmap_eq_mem);
    for (usz j = 0; j < out_handle->type_counters[i]; ++j) {
      CBN_SKAP_AssetIdx idx;
      fread(&idx, sizeof(idx), 1, out_handle->fd);
      // CBN_DEBUG("----------------------------------------");
      // CBN_DEBUG("idx.name = `%s`", idx.name);
      // CBN_DEBUG("idx.metadata.type = `%s`", carbon_skap__type2str[idx.metadata.type]);
      // CBN_DEBUG("idx.blob_offset = " CARBON_SKAP__HEX_SPEC, idx.blob_offset);
      // CBN_DEBUG("idx.blob_size = " CARBON_SKAP__HEX_SPEC, idx.blob_size);
      // CBN_DEBUG("idx.checksum = %#010x", idx.checksum);
      // CBN_DEBUG("----------------------------------------");
      carbon_hashmap_set(&out_handle->idxs[i], idx.name, &idx);
    }
  }
  // Write position of start of blob section
  out_handle->blob_section_start_pos = ftell(out_handle->fd);
  CBN_INFO("SKAP (`%s`) opened successfully", skap);
  return true;
}

void carbon_skap_close(CBN_SKAP *handle) {
  if (!handle) {
    CBN_WARN("`handle` is not a valid pointer, skipping closing");
    return;
  }
  fclose(handle->fd);
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_hashmap_destroy(&handle->idxs[i]);
  }
  CBN_INFO("SKAP closed successfully");
}

void carbon_skap_print(const CBN_SKAP *handle) {
  if (!handle) return;
  carbon_println("SKAP v%u (build %llu) with %zu assets:",
                 handle->header.fmt_ver,
                 handle->header.build_ver,
                 carbon_skap_count(handle));
  for (CBN_SKAP_AssetType i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_println("  - %s: %zu", carbon_skap__type2str[i], carbon_skap_count_of(handle, i));
  }
}

bool carbon_skap_lookup(const CBN_SKAP *handle, const CBN_SKAP_AssetType asset_type, const char *asset_name, void *out_blob) {
  if (!handle || !asset_name || !out_blob) {
    CBN_ERROR("`handle`, `asset_name` and `out_blob` must be valid pointers");
    return false;
  }
  if (asset_type >= CARBON_SKAP_ASSET_TYPE_COUNT) {
    CBN_ERROR("`asset_type` is not valid");
    return false;
  }
  if (!handle->type_counters[asset_type]) {
    CBN_ERROR("No assets of the asset_type (`%s`) requested", carbon_skap__type2str[asset_type]);
    return false;
  }
  char key[CARBON_SKAP__DECL_FILE_MAX_LINE_LEN] = {0};
  carbon_string_sfmt(key, sizeof(key), "%s", asset_name);
  CBN_SKAP_AssetIdx *idx = carbon_hashmap_get(&handle->idxs[asset_type], key);
  if (!idx) {
    CBN_ERROR("No assets with the asset_name (`%s`) requested", asset_name);
    return false;
  }
  if (carbon_string_cmp(idx->name, asset_name)) {
    CBN_ERROR("idx.name (`%s`) doesn't match the asset_name (`%s`) requested", idx->name, asset_name);
    return false;
  }
  if (idx->metadata.type != asset_type) {
    CBN_ERROR("`idx.metadata.type` doesn't match the `asset_type` requested");
    return false;
  }
  bool ok = carbon_skap__type2lbfn[asset_type](handle, idx, out_blob);
  if (ok) CBN_INFO("asset `%s` retrieved successfully", asset_name);
  return ok;
}

usz carbon_skap_count(const CBN_SKAP *handle) {
  usz count = 0;
  for (CBN_SKAP_AssetType i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    count += carbon_skap_count_of(handle, i);
  }
  return count;
}

usz carbon_skap_count_of(const CBN_SKAP *handle, const CBN_SKAP_AssetType type) {
  if (type >= CARBON_SKAP_ASSET_TYPE_COUNT) {
    CBN_ERROR("`type` is not valid");
    return 0;
  }
  return handle->type_counters[type];
}
