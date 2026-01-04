// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_SKAP__HEX_SPEC "%#012x"

// @type_dependant
static const char * const carbon_skap__allowed_types[] = {
  "images",
  "binaries",
  "meshes"
};
static_assert(CARBON_SKAP_ASSET_TYPE_COUNT == CARBON_ARRAY_LEN(carbon_skap__allowed_types),
              "Amount of allowed asset types has changed");

static CBN_List carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_COUNT];
static CBN_List carbon_skap__asset_idxs[CARBON_SKAP_ASSET_TYPE_COUNT];
static CBN_List carbon_skap__asset_idx_locs[CARBON_SKAP_ASSET_TYPE_COUNT];

// @type_dependant
CBNINL CBN_SKAP_AssetType carbon_skap__str2type(const char *s) {
  if (!carbon_string_cmp(s, "images")) return CARBON_SKAP_ASSET_TYPE_IMAGE;
  else if (!carbon_string_cmp(s, "binaries")) return CARBON_SKAP_ASSET_TYPE_BINARY;
  else if (!carbon_string_cmp(s, "meshes")) return CARBON_SKAP_ASSET_TYPE_MESH;
  else {
    CARBON_UNREACHABLE;
    return CARBON_SKAP_ASSET_TYPE_COUNT;
  };
}

CBNINL bool carbon_skap__parse_decl_file(FILE *decl_fd, CBN_List *asset_groups) {
  bool first = true;
  bool prev_line_was_new_group = false;
  usz line_n = 1;
  char line[CARBON_SKAP_DECL_FILE_MAX_LINE_LEN];
  while (fgets(line, CARBON_SKAP_DECL_FILE_MAX_LINE_LEN, decl_fd)) {
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
                      CARBON_QUOTE(CARBON_SKAP_DECL_FILE_MAX_PATH_LEN)
                      "[^\"]\" as %"
                      CARBON_QUOTE(CARBON_SKAP_DECL_FILE_MAX_TYPE_LEN)
                      "[^]]",
                      ag.path, ag.type)) {
        CBN_ERROR("on line %zu, syntax error; expected expression `[\"<PATH>\" as <TYPE>]`", line_n);
        return false;
      }
      // Check whether type is valid
      bool type_is_valid = false;
      for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
        if (!carbon_string_cmp(ag.type, carbon_skap__allowed_types[i])) {
          type_is_valid = true;
          break;
        }
      }
      // @type_dependant
      if (!type_is_valid) {
        if (!carbon_string_cmp(ag.type, "image") ||
            !carbon_string_cmp(ag.type, "img")   ||
            !carbon_string_cmp(ag.type, "imgs")) {
          CBN_ERROR("on line %zu, syntax error; type `%s` not recognized, maybe you ment `images`?", line_n, ag.type);
        }
        else if (!carbon_string_cmp(ag.type, "binary") ||
                 !carbon_string_cmp(ag.type, "bin")    ||
                 !carbon_string_cmp(ag.type, "bins")) {
          CBN_ERROR("on line %zu, syntax error; type `%s` not recognized, maybe you ment `binaries`?", line_n, ag.type);
        }
        else if (!carbon_string_cmp(ag.type, "mesh")) {
          CBN_ERROR("on line %zu, syntax error; type `%s` not recognized, maybe you ment `meshes`?", line_n, ag.type);
        }
        else CBN_ERROR("on line %zu, syntax error; type `%s` not recognized", line_n, ag.type);
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
      carbon_list_pop(asset_groups, &ag);
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
    if (!ag_it.var.assets.size) continue;
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
  if (asset_groups->size) {
    carbon_list_foreach(CBN_SKAP_AssetGroup, *asset_groups) {
      // CBN_DEBUG("asset_groups[%$].assets.size = %$", $(it.i), $(it.var.assets.size));
      carbon_strlist_destroy(&it.var.assets);
    }
  }
  carbon_list_destroy(asset_groups);
}

// @type_dependant
CBNINL void carbon_skap__create_global_lists(void) {
  carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE] = carbon_list_create(sizeof(CBN_Image));
  carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_BINARY] = carbon_list_create(sizeof(CBN_Binary));
  carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_MESH] = carbon_list_create(sizeof(CBN_Mesh));
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_skap__asset_idxs[i] = carbon_list_create(sizeof(CBN_SKAP_AssetIdx));
    carbon_skap__asset_idx_locs[i] = carbon_list_create(sizeof(usz));
  }
}

// @type_dependant
CBNINL void carbon_skap__destroy_global_lists(void) {
  if (!carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE].size) return;
  carbon_list_foreach(CBN_Image, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE]) {
    carbon_fs_destroy_img(&it.var);
  }
  if (!carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_BINARY].size) return;
  carbon_list_foreach(CBN_Binary, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_BINARY]) {
    carbon_memory_free(it.var.data);
  }
  if (!carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_MESH].size) return;
  carbon_list_foreach(CBN_Mesh, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_MESH]) {
    carbon_mesh_destroy(&it.var);
  }
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
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
      if (!carbon_string_cmp(it.var.type, carbon_skap__allowed_types[i])) {
        asset_count += it.var.assets.size;
      }
    }
    fwrite(&asset_count, sizeof(asset_count), 1, fd);
  }
}

// @type_dependant
CBNINL void carbon_skap__append_idxs(FILE *fd, const char *decl, CBN_List *asset_groups) {
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_list_foreach(CBN_SKAP_AssetGroup, ag_it, *asset_groups) {
      if (!ag_it.var.assets.size || carbon_string_cmp(ag_it.var.type, carbon_skap__allowed_types[i])) continue;
      carbon_strlist_foreach(ag_it.var.assets) {
        const char *cwd = carbon_fs_get_curr_directory();
        CBN_ASSERT(carbon_fs_change_directory(carbon_fs_get_directory(decl)));
        const char *asset_name = carbon_string_fmt("%s%s", ag_it.var.path, carbon_strview_to_cstr(it.sv));
        CBN_SKAP_AssetIdx idx;
        carbon_memory_set(&idx, 0, sizeof(idx));
        strncpy(idx.name, asset_name, sizeof(idx.name));
        idx.metadata.type = carbon_skap__str2type(ag_it.var.type);
        switch (idx.metadata.type) {
        case CARBON_SKAP_ASSET_TYPE_IMAGE: {
          CBN_Image asset = carbon_fs_read_img_from_file(asset_name);
          carbon_list_push(&carbon_skap__assets[i], &asset);
          idx.metadata.as_img = asset.metadata;
        } break;
        case CARBON_SKAP_ASSET_TYPE_BINARY: {
          CBN_Binary asset = {
            .data = 0,
            .metadata.size = carbon_fs_get_file_size(asset_name)
          };
          asset.data = (u8 *) carbon_memory_alloc(asset.metadata.size);
          FILE *asset_fd = fopen(asset_name, "rb");
          fread(asset.data, asset.metadata.size, 1, asset_fd);
          fclose(asset_fd);
          carbon_list_push(&carbon_skap__assets[i], &asset);
          idx.metadata.as_bin = asset.metadata;
        } break;
        case CARBON_SKAP_ASSET_TYPE_MESH: {
          CBN_Mesh asset = {0};
          carbon_mesh_create_from_file(&asset, asset_name);
          carbon_list_push(&carbon_skap__assets[i], &asset);
          idx.metadata.as_mesh = asset.metadata;
        } break;
        case CARBON_SKAP_ASSET_TYPE_COUNT:
        default: CARBON_UNREACHABLE;
        }
        carbon_list_push(&carbon_skap__asset_idxs[i], &idx);
        usz idx_loc = ftell(fd);
        carbon_list_push(&carbon_skap__asset_idx_locs[i], &idx_loc);
        CBN_ASSERT(carbon_fs_change_directory(cwd));
        fwrite(&idx, sizeof(idx), 1, fd);
      }
    }
  }
}

CBNINL void carbon_skap__append_blobs__images(const char *skap, FILE *fd) {
  if (!carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE].size) return;
  carbon_list_foreach(CBN_Image, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE]) {
    CBN_Image *asset = &carbon_list_at_raw(CBN_Image, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE], it.i);
    CBN_SKAP_AssetIdx *idx = &carbon_list_at_raw(CBN_SKAP_AssetIdx, carbon_skap__asset_idxs[CARBON_SKAP_ASSET_TYPE_IMAGE], it.i);
    usz *loc = &carbon_list_at_raw(usz, carbon_skap__asset_idx_locs[CARBON_SKAP_ASSET_TYPE_IMAGE], it.i);
    // Save the position where the blob needs to be written
    usz blob_loc = ftell(fd);
    // Complete the empty fields in the idx struct
    idx->blob_offset = blob_loc;
    idx->blob_size = idx->metadata.as_img.width * idx->metadata.as_img.height * idx->metadata.as_img.channels;
    idx->checksum = carbon_crypto_crc32(asset->data, idx->blob_size);
    // Go back and overwrite the idx with its complete version
    fseek(fd, *loc, SEEK_SET);
    fwrite(idx, sizeof(*idx), 1, fd);
    // Return and write the blob
    fseek(fd, blob_loc, SEEK_SET);
    carbon_println("  WRITE   %s -> %s @ [" CARBON_SKAP__HEX_SPEC "]+(" CARBON_SKAP__HEX_SPEC ")", idx->name, skap, idx->blob_offset, idx->blob_size);
    fwrite(asset->data, idx->blob_size, 1, fd);
  }
}

CBNINL void carbon_skap__append_blobs__binaries(const char *skap, FILE *fd) {
  if (!carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_BINARY].size) return;
  carbon_list_foreach(CBN_Binary, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_BINARY]) {
    CBN_Binary *asset = &carbon_list_at_raw(CBN_Binary, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_BINARY], it.i);
    CBN_SKAP_AssetIdx *idx = &carbon_list_at_raw(CBN_SKAP_AssetIdx, carbon_skap__asset_idxs[CARBON_SKAP_ASSET_TYPE_BINARY], it.i);
    usz *loc = &carbon_list_at_raw(usz, carbon_skap__asset_idx_locs[CARBON_SKAP_ASSET_TYPE_BINARY], it.i);
    // Save the position where the blob needs to be written
    usz blob_loc = ftell(fd);
    // Complete the empty fields in the idx struct
    idx->blob_offset = blob_loc;
    idx->blob_size = idx->metadata.as_bin.size;
    idx->checksum = carbon_crypto_crc32(asset->data, idx->blob_size);
    // Go back and overwrite the idx with its complete version
    fseek(fd, *loc, SEEK_SET);
    fwrite(idx, sizeof(*idx), 1, fd);
    // Return and write the blob
    fseek(fd, blob_loc, SEEK_SET);
    carbon_println("  WRITE   %s -> %s @ [" CARBON_SKAP__HEX_SPEC "]+(" CARBON_SKAP__HEX_SPEC ")", idx->name, skap, idx->blob_offset, idx->blob_size);
    fwrite(asset->data, idx->blob_size, 1, fd);
  }
}

CBNINL void carbon_skap__append_blobs__meshes(const char *skap, FILE *fd) {
  if (!carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_MESH].size) return;
  carbon_list_foreach(CBN_Mesh, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_MESH]) {
    CBN_Mesh *asset = &carbon_list_at_raw(CBN_Mesh, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_MESH], it.i);
    CBN_SKAP_AssetIdx *idx = &carbon_list_at_raw(CBN_SKAP_AssetIdx, carbon_skap__asset_idxs[CARBON_SKAP_ASSET_TYPE_MESH], it.i);
    usz *loc = &carbon_list_at_raw(usz, carbon_skap__asset_idx_locs[CARBON_SKAP_ASSET_TYPE_MESH], it.i);
    // Save the position where the blob needs to be written
    usz blob_loc = ftell(fd);
    // Complete the empty fields in the idx struct
    idx->blob_offset = blob_loc;
    usz sz_v  = idx->metadata.as_mesh.vertices_count * sizeof(CBN_Vec3);
    usz sz_vt = idx->metadata.as_mesh.texcoords_count * sizeof(CBN_Vec2);
    usz sz_vn = idx->metadata.as_mesh.normals_count * sizeof(CBN_Vec3);
    usz sz_f  = idx->metadata.as_mesh.faces_count * CARBON_MESH_FACE_COMPS * 3 * sizeof(usz);
    idx->blob_size = sz_v + sz_vt + sz_vn + sz_f;
    idx->checksum = carbon_mesh_compute_crc32(asset);
    // Go back and overwrite the idx with its complete version
    fseek(fd, *loc, SEEK_SET);
    fwrite(idx, sizeof(*idx), 1, fd);
    // Return and write the blob
    fseek(fd, blob_loc, SEEK_SET);
    carbon_println("  WRITE   %s -> %s @ [" CARBON_SKAP__HEX_SPEC "]+(" CARBON_SKAP__HEX_SPEC ")", idx->name, skap, idx->blob_offset, idx->blob_size);
    fwrite(asset->vertices, sz_v, 1, fd);
    fwrite(asset->texcoords, sz_vt, 1, fd);
    fwrite(asset->normals, sz_vn, 1, fd);
    fwrite(asset->faces, sz_f, 1, fd);
  }
}

// @type_dependant
CBNINL void carbon_skap__append_blobs(const char *skap, FILE *fd) {
  carbon_skap__append_blobs__images(skap, fd);
  carbon_skap__append_blobs__binaries(skap, fd);
  carbon_skap__append_blobs__meshes(skap, fd);
}

bool carbon_skap_create(const char *decl, const char *skap) {
  // Input: DECL text file
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
  // Output: SKAP binary file
  if (carbon_fs_exists(skap) && !carbon_fs_rename(skap, carbon_string_fmt("%s.old", skap))) return false;
  carbon_skap__create_global_lists();
  FILE *skap_fd = fopen(skap, "wb");
  carbon_skap__append_header(skap_fd);
  carbon_skap__append_type_counters(skap_fd, &asset_groups);
  carbon_skap__append_idxs(skap_fd, decl, &asset_groups);
  carbon_skap__append_blobs(skap, skap_fd);
  fclose(skap_fd);
  // End
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
    CBN_ERROR("file `%s` is SKAP, but it's using a different fmt version (%$) from the expected one (1)", skap, $(out_handle->header.fmt_ver));
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
    out_handle->idxs[i] = carbon_hashmap_create(out_handle->type_counters[i], sizeof(CBN_SKAP_AssetIdx));
    for (usz j = 0; j < out_handle->type_counters[i]; ++j) {
      CBN_SKAP_AssetIdx idx;
      fread(&idx, sizeof(idx), 1, out_handle->fd);
      // CBN_DEBUG("----------------------------------------");
      // CBN_DEBUG("idx.name = `%s`", idx.name);
      // CBN_DEBUG("idx.metadata.type = `%s`", carbon_skap__allowed_types[idx.metadata.type]);
      // CBN_DEBUG("idx.metadata.as_img.width = %zu", idx.metadata.as_img.width);
      // CBN_DEBUG("idx.metadata.as_img.height = %zu", idx.metadata.as_img.height);
      // CBN_DEBUG("idx.metadata.as_img.channels = %zu", idx.metadata.as_img.channels);
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
  if (!handle) {
    CBN_WARN("`handle` is not a valid pointer, skipping printing");
    return;
  }
  carbon_println("fd: %p", handle->fd);
  carbon_println("blob_section_start_pos: " CARBON_SKAP__HEX_SPEC, handle->blob_section_start_pos, handle->blob_section_start_pos);
  carbon_println("header:");
  carbon_println("  - signature: %.4s", handle->header.signature);
  carbon_println("  - fmt_ver: %$", $(handle->header.fmt_ver));
  carbon_println("  - build_ver: %$", $(handle->header.build_ver));
  carbon_println("type_counters:");
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_println("  - %$: %zu", $(carbon_skap__allowed_types[i]), carbon_skap_count_of(handle, (CBN_SKAP_AssetType) i));
  }
  carbon_println("idxs:");
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_println("  - %$:", $(carbon_skap__allowed_types[i]));
    // TODO: we need to iterate the hashmaps, not the type_counters
    for (usz j = 0; j < handle->type_counters[i]; ++j) {
      carbon_println("      - (...)");
    }
  }
}

// @type_dependant
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
    CBN_ERROR("No assets of the asset_type (`%s`) requested", carbon_skap__allowed_types[asset_type]);
    return false;
  }
  CBN_SKAP_AssetIdx idx;
  if (!carbon_hashmap_get(&handle->idxs[asset_type], asset_name, &idx)) {
    CBN_ERROR("No asset with asset_name (`%s`) requested", asset_name);
    return false;
  }
  if (carbon_string_cmp(idx.name, asset_name)) {
    CBN_ERROR("idx.name (`%s`) doesn't match the asset_name (`%s`) requested", idx.name, asset_name);
    return false;
  }
  if (idx.metadata.type != asset_type) {
    CBN_ERROR("`idx.metadata.type` doesn't match the `asset_type` requested");
    return false;
  }
  switch (asset_type) {
  case CARBON_SKAP_ASSET_TYPE_IMAGE: {
    CBN_Image asset = {
      .data = (u8 *) carbon_memory_alloc(idx.blob_size),
      .metadata = idx.metadata.as_img
    };
    fseek(handle->fd, idx.blob_offset, SEEK_SET);
    fread(asset.data, idx.blob_size, 1, handle->fd);
    fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
    if (idx.checksum != carbon_crypto_crc32(asset.data, idx.blob_size)) {
      CBN_ERROR("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
      carbon_fs_destroy_img(&asset);
      return false;
    }
    carbon_memory_copy(out_blob, &asset, sizeof(asset));
    CBN_INFO("asset `%s` retrieved successfully", asset_name);
    return true;
  } break;
  case CARBON_SKAP_ASSET_TYPE_BINARY: {
    CBN_Binary asset = {
      .data = (u8 *) carbon_memory_alloc(idx.blob_size),
      .metadata = idx.metadata.as_bin
    };
    fseek(handle->fd, idx.blob_offset, SEEK_SET);
    fread(asset.data, idx.blob_size, 1, handle->fd);
    fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
    if (idx.checksum != carbon_crypto_crc32(asset.data, idx.blob_size)) {
      CBN_ERROR("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
      carbon_memory_free(asset.data);
      return false;
    }
    carbon_memory_copy(out_blob, &asset, sizeof(asset));
    CBN_INFO("asset `%s` retrieved successfully", asset_name);
    return true;
  } break;
  case CARBON_SKAP_ASSET_TYPE_MESH: {
    usz sz_v  = idx.metadata.as_mesh.vertices_count * sizeof(CBN_Vec3);
    usz sz_vt = idx.metadata.as_mesh.texcoords_count * sizeof(CBN_Vec2);
    usz sz_vn = idx.metadata.as_mesh.normals_count * sizeof(CBN_Vec3);
    usz sz_f  = idx.metadata.as_mesh.faces_count * CARBON_MESH_FACE_COMPS * 3 * sizeof(usz);
    CBN_Mesh asset = {
      .vertices = carbon_memory_alloc(sz_v),
      .texcoords = carbon_memory_alloc(sz_vt),
      .normals = carbon_memory_alloc(sz_vn),
      .faces = carbon_memory_alloc(sz_f),
      .metadata = idx.metadata.as_mesh
    };
    fseek(handle->fd, idx.blob_offset, SEEK_SET);
    fread(asset.vertices, sz_v, 1, handle->fd);
    fread(asset.texcoords, sz_vt, 1, handle->fd);
    fread(asset.normals, sz_vn, 1, handle->fd);
    fread(asset.faces, sz_f, 1, handle->fd);
    fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
    if (idx.checksum != carbon_mesh_compute_crc32(&asset)) {
      CBN_ERROR("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
      carbon_mesh_destroy(&asset);
      return false;
    }
    carbon_memory_copy(out_blob, &asset, sizeof(asset));
    CBN_INFO("asset `%s` retrieved successfully", asset_name);
    return true;
  } break;
  case CARBON_SKAP_ASSET_TYPE_COUNT:
  default: CARBON_UNREACHABLE;
  }
  return false;
}

usz carbon_skap_count(const CBN_SKAP *handle) {
  usz count = 0;
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    count += carbon_skap_count_of(handle, (CBN_SKAP_AssetType) i);
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
