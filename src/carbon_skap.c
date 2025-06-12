// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "../carbon.h.in"
#include "carbon_deps.h"
#include "carbon_defs.h"
#include "carbon_types.h"
#include "carbon_assert.h"
#include "carbon_memory.h"
#include "carbon_list.h"
#include "carbon_strlist.h"
#include "carbon_string.h"
#include "carbon_strview.h"
#include "carbon_strbuilder.h"
#include "carbon_hashmap.h"
#include "carbon_log.h"
#include "carbon_time.h"
#include "carbon_fs.h"
#include "carbon_skap.h"
#include "carbon_crypto.h"

#define CARBON_SKAP__HEX_SPEC "%#012x"

// @type_dependant
static const char * const carbon_skap__allowed_types[] = {
  "images"
};
CARBON_STATIC_ASSERT(CARBON_SKAP_ASSET_TYPE_COUNT == CARBON_ARRAY_LEN(carbon_skap__allowed_types),
                     "Amount of allowed asset types has changed");

static CBN_List carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_COUNT];
static CBN_List carbon_skap__asset_idxs[CARBON_SKAP_ASSET_TYPE_COUNT];
static CBN_List carbon_skap__asset_idx_locs[CARBON_SKAP_ASSET_TYPE_COUNT];

// @type_dependant
CARBON_INLINE CBN_SKAP_AssetType carbon_skap__str2type(const char *s) {
  if (!carbon_string_cmp(s, "images")) return CARBON_SKAP_ASSET_TYPE_IMAGE;
  else {
    CARBON_UNREACHABLE;
    return CARBON_SKAP_ASSET_TYPE_COUNT;
  };
}

CARBON_INLINE u8 carbon_skap__parse_decl_file(FILE *decl_fd, CBN_List *asset_groups) {
  u8 first = true;
  u8 prev_line_was_new_group = false;
  usz line_n = 1;
  char line[CARBON_SKAP_DECL_FILE_MAX_LINE_LEN];
  while (fgets(line, CARBON_SKAP_DECL_FILE_MAX_LINE_LEN, decl_fd)) {
    line[strcspn(line, "\r")] = 0;
    line[strcspn(line, "\n")] = 0;
    CBN_SKAP_AssetGroup ag;
    carbon_println("%s", line);  // DEBUG
    // Empty or commented line
    if (line[0] == 0 || (line[0] == '/' && line[1] == '/')) {
      ++line_n;
      continue;
    }
    // Whitespace-only line
    u8 all_is_whitespaced = true;
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
        carbon_log_error("on line %zu, syntax error; expected expression `<ASSET>`", line_n);
        return false;
      }
      if (first) first = false;
      if (2 != sscanf(line,
                      "[\"%"
                      CARBON_EXPAND_AND_QUOTE(CARBON_SKAP_DECL_FILE_MAX_PATH_LEN)
                      "[^\"]\" as %"
                      CARBON_EXPAND_AND_QUOTE(CARBON_SKAP_DECL_FILE_MAX_TYPE_LEN)
                      "[^]]",
                      ag.path, ag.type)) {
        carbon_log_error("on line %zu, syntax error; expected expression `[\"<PATH>\" as <TYPE>]`", line_n);
        return false;
      }
      // Check whether type is valid
      u8 type_is_valid = false;
      for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
        if (!carbon_string_cmp(ag.type, carbon_skap__allowed_types[i])) {
          type_is_valid = true;
          break;
        }
      }
      if (!type_is_valid) {
        if (!carbon_string_cmp(ag.type, "image") ||
            !carbon_string_cmp(ag.type, "img")   ||
            !carbon_string_cmp(ag.type, "imgs")) {
          carbon_log_error("on line %zu, syntax error; type `%s` not recognized, maybe you ment `images`?", line_n, ag.type);
        }
        else carbon_log_error("on line %zu, syntax error; type `%s` not recognized", line_n, ag.type);
        return false;
      }
      // Check whether path is valid
      if (ag.path[carbon_string_len(ag.path) - 1] != '/') {
        carbon_log_error("on line %zu, syntax error; path needs to end with `/` char", line_n);
        return false;
      }
      ag.assets = carbon_strlist_create(true);
      carbon_list_push(asset_groups, &ag);
      prev_line_was_new_group = true;
      carbon_println("  -> Path: `%s`, Type: `%s`", ag.path, ag.type);  // DEBUG
    }
    // New asset in previous group
    else {
      if (first) {
        carbon_log_error("on line %zu, asset without group declaration makes no sense", line_n);
        return false;
      }
      carbon_list_pop(asset_groups, &ag);
      carbon_strlist_push(&ag.assets, line);
      carbon_list_push(asset_groups, &ag);
      prev_line_was_new_group = false;
      carbon_println("  -> Asset: `%s`", line);  // DEBUG
    }
    ++line_n;
  }
  return true;
}

CARBON_INLINE u8 carbon_skap__check_decl_assets(CBN_List *asset_groups) {
  // TODO: cd to the dir which holds the decl file (e.g. `assets.txt`)
  u8 status = true;
  carbon_list_foreach(CBN_SKAP_AssetGroup, *asset_groups) {
    CBN_SKAP_AssetGroup ag = it.var;
    carbon_strlist_foreach(ag.assets) {
      const char *asset_name = carbon_string_fmt("%s%s", ag.path, carbon_strview_to_cstr(it.sv));
      if (!carbon_fs_exists(asset_name)) {
        carbon_log_error("asset declared as `%s` doesn't exist", asset_name);
        status = false;
      }
    }
  }
  return status;
}

CARBON_INLINE void carbon_skap__destroy_asset_groups(CBN_List *asset_groups) {
  carbon_log_debug("asset_groups->size = %$", $(asset_groups->size));
  if (asset_groups->size) {
    carbon_list_foreach(CBN_SKAP_AssetGroup, *asset_groups) {
      carbon_log_debug("asset_groups[%$].assets.size = %$", $(it.i), $(it.var.assets.size));
      carbon_strlist_destroy(&it.var.assets);
    }
  }
  carbon_list_destroy(asset_groups);
}

// @type_dependant
CARBON_INLINE void carbon_skap__create_global_lists(void) {
  carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE] = carbon_list_create(sizeof(CBN_Image));
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_skap__asset_idxs[i] = carbon_list_create(sizeof(CBN_SKAP_AssetIdx));
    carbon_skap__asset_idx_locs[i] = carbon_list_create(sizeof(usz));
  }
}

// @type_dependant
CARBON_INLINE void carbon_skap__destroy_global_lists(void) {
  carbon_list_foreach(CBN_Image, carbon_skap__assets[CARBON_SKAP_ASSET_TYPE_IMAGE]) {
    carbon_fs_destroy_img(&it.var);
  }
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_list_destroy(&carbon_skap__assets[i]);
    carbon_list_destroy(&carbon_skap__asset_idxs[i]);
    carbon_list_destroy(&carbon_skap__asset_idx_locs[i]);
  }
}

CARBON_INLINE void carbon_skap__append_header(FILE *fd) {
  CBN_SKAP_Header header = {
    .signature = {'S', 'K', 'A', 'P'},
    .fmt_ver = 1,
    .build_ver = carbon_time_get_yyyymmddhhmmss()
  };
  fwrite(&header, sizeof(header), 1, fd);
}

CARBON_INLINE void carbon_skap__append_type_counters(FILE *fd, CBN_List *asset_groups) {
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
CARBON_INLINE void carbon_skap__append_idxs(FILE *fd, CBN_List *asset_groups) {
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_list_foreach(CBN_SKAP_AssetGroup, *asset_groups) {
      CBN_SKAP_AssetGroup ag = it.var;
      if (carbon_string_cmp(ag.type, carbon_skap__allowed_types[i])) continue;
      carbon_strlist_foreach(ag.assets) {
        const char *asset_name = carbon_string_fmt("%s%s", ag.path, carbon_strview_to_cstr(it.sv));
        CBN_SKAP_AssetIdx idx;
        carbon_memory_set(&idx, 0, sizeof(idx));
        strncpy(idx.name, asset_name, sizeof(idx.name));
        idx.metadata.type = carbon_skap__str2type(ag.type);
        switch (idx.metadata.type) {
        case CARBON_SKAP_ASSET_TYPE_IMAGE: {
          CBN_Image asset = carbon_fs_read_img_from_file(asset_name);
          carbon_list_push(&carbon_skap__assets[i], &asset);
          idx.metadata.as_img = asset.metadata;
          break;
        }
        case CARBON_SKAP_ASSET_TYPE_COUNT:
        default: CARBON_UNREACHABLE;
        }
        carbon_list_push(&carbon_skap__asset_idxs[i], &idx);
        usz idx_loc = ftell(fd);
        carbon_list_push(&carbon_skap__asset_idx_locs[i], &idx_loc);
        fwrite(&idx, sizeof(idx), 1, fd);
      }
    }
  }
}

// @type_dependant
CARBON_INLINE void carbon_skap__append_blobs(FILE *fd) {
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
    carbon_println("  WRITE   %-30s @ [" CARBON_SKAP__HEX_SPEC "]+(" CARBON_SKAP__HEX_SPEC ")", idx->name, idx->blob_offset, idx->blob_size);
    fwrite(asset->data, idx->blob_size, 1, fd);
  }
}

u8 carbon_skap_create(const char *decl, const char *skap) {
  // Input: DECL text file
  if (!carbon_fs_exists(decl)) {
    carbon_log_error("there is no SKAP declarations file named `%s`", decl);
    return false;
  }
  CBN_List asset_groups = carbon_list_create(sizeof(CBN_SKAP_AssetGroup));
  FILE *decl_fd = fopen(decl, "r");
  if (!carbon_skap__parse_decl_file(decl_fd, &asset_groups)) {
    carbon_log_error("SKAP declarations file (`%s`) has syntax errors and can't be parsed correctly", decl);
    carbon_skap__destroy_asset_groups(&asset_groups);
    return false;
  }
  fclose(decl_fd);
  if (!carbon_skap__check_decl_assets(&asset_groups)) {
    carbon_log_error("SKAP declarations file (`%s`) has semantic errors and can't be interpreted correctly", decl);
    carbon_skap__destroy_asset_groups(&asset_groups);
    return false;
  }
  // Output: SKAP binary file
  if (carbon_fs_exists(skap) && !carbon_fs_rename(skap, carbon_string_fmt("%s.old", skap))) return false;
  carbon_skap__create_global_lists();
  FILE *skap_fd = fopen(skap, "wb");
  carbon_skap__append_header(skap_fd);
  carbon_skap__append_type_counters(skap_fd, &asset_groups);
  carbon_skap__append_idxs(skap_fd, &asset_groups);
  carbon_skap__append_blobs(skap_fd);
  fclose(skap_fd);
  // End
  carbon_skap__destroy_global_lists();
  carbon_skap__destroy_asset_groups(&asset_groups);
  return true;
}

u8 carbon_skap_open(const char *skap, CBN_SKAP_Handle *out_handle) {
  if (!out_handle) {
    carbon_log_error("`out_handle` must be a valid pointer");
    return false;
  }
  if (!carbon_fs_exists(skap)) {
    carbon_log_error("there is no SKAP file named `%s`", skap);
    return false;
  }
  carbon_memory_set(out_handle, 0, sizeof(*out_handle));
  out_handle->fd = fopen(skap, "rb");
  // Read header
  fread(&out_handle->header, sizeof(out_handle->header), 1, out_handle->fd);
  if (carbon_string_cmp_n(out_handle->header.signature, "SKAP", 4)) {
    carbon_log_error("file `%s` is not a valid SKAP", skap);
    fclose(out_handle->fd);
    return false;
  }
  if (out_handle->header.fmt_ver != 1) {
    carbon_log_error("file `%s` is SKAP, but it's using a different fmt version (%$) from the expected one (1)", skap, $(out_handle->header.fmt_ver));
    fclose(out_handle->fd);
    return false;
  }
  // Read type_counters and idxs
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    fread(&out_handle->type_counters[i], sizeof(usz), 1, out_handle->fd);
    out_handle->idxs[i] = carbon_hashmap_create(out_handle->type_counters[i], sizeof(CBN_SKAP_AssetIdx));
    for (usz j = 0; j < out_handle->type_counters[i]; ++j) {
      CBN_SKAP_AssetIdx idx;
      fread(&idx, sizeof(idx), 1, out_handle->fd);
      carbon_log_debug("----------------------------------------");
      carbon_log_debug("idx.name = `%s`", idx.name);
      carbon_log_debug("idx.metadata.type = `%s`", carbon_skap__allowed_types[idx.metadata.type]);
      carbon_log_debug("idx.metadata.as_img.width = %zu", idx.metadata.as_img.width);
      carbon_log_debug("idx.metadata.as_img.height = %zu", idx.metadata.as_img.height);
      carbon_log_debug("idx.metadata.as_img.channels = %zu", idx.metadata.as_img.channels);
      carbon_log_debug("idx.blob_offset = " CARBON_SKAP__HEX_SPEC, idx.blob_offset);
      carbon_log_debug("idx.blob_size = " CARBON_SKAP__HEX_SPEC, idx.blob_size);
      carbon_log_debug("idx.checksum = %#010x", idx.checksum);
      carbon_log_debug("----------------------------------------");
      carbon_hashmap_set(&out_handle->idxs[i], idx.name, &idx);
    }
  }
  // Write position of start of blob section
  out_handle->blob_section_start_pos = ftell(out_handle->fd);
  return true;
}

void carbon_skap_close(CBN_SKAP_Handle *handle) {
  if (!handle) {
    carbon_log_warn("`handle` is not a valid pointer, skipping closing");
    return;
  }
  fclose(handle->fd);
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    carbon_hashmap_destroy(&handle->idxs[i]);
  }
}

void carbon_skap_print(const CBN_SKAP_Handle *handle) {
  if (!handle) {
    carbon_log_warn("`handle` is not a valid pointer, skipping printing");
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
u8 carbon_skap_lookup(const CBN_SKAP_Handle *handle, const CBN_SKAP_AssetType asset_type, const char *asset_name, void *out_blob) {
  if (!handle || !asset_name || !out_blob) {
    carbon_log_error("`handle`, `asset_name` and `out_blob` must be valid pointers");
    return false;
  }
  if (asset_type >= CARBON_SKAP_ASSET_TYPE_COUNT) {
    carbon_log_error("`asset_type` is not valid");
    return false;
  }
  CBN_SKAP_AssetIdx idx;
  carbon_hashmap_get(&handle->idxs[asset_type], asset_name, &idx);
  if (carbon_string_cmp(idx.name, asset_name)) {
    carbon_log_error("`idx.name` doesn't match the `asset_name` requested");
    return false;
  }
  if (idx.metadata.type != asset_type) {
    carbon_log_error("`idx.metadata.type` doesn't match the `asset_type` requested");
    return false;
  }
  switch (asset_type) {
  case CARBON_SKAP_ASSET_TYPE_IMAGE: {
    CBN_Image asset = {
      .data = (u8 *) CARBON_MALLOC(idx.blob_size),
      .metadata = idx.metadata.as_img
    };
    fseek(handle->fd, idx.blob_offset, SEEK_SET);
    fread(asset.data, idx.blob_size, 1, handle->fd);
    fseek(handle->fd, handle->blob_section_start_pos, SEEK_SET);
    if (idx.checksum != carbon_crypto_crc32(asset.data, idx.blob_size)) {
      carbon_log_error("`idx.checksum` doesn't match the retrieved asset data's CRC32 checksum");
      CARBON_FREE(asset.data);
      return false;
    }
    carbon_memory_copy(out_blob, &asset, sizeof(asset));
    carbon_log_info("asset `%s` retrieved successfully", asset_name);
    return true;
  }
  case CARBON_SKAP_ASSET_TYPE_COUNT:
  default: CARBON_UNREACHABLE;
  }
  return false;
}

usz carbon_skap_count(const CBN_SKAP_Handle *handle) {
  usz count = 0;
  for (usz i = 0; i < CARBON_SKAP_ASSET_TYPE_COUNT; ++i) {
    count += carbon_skap_count_of(handle, (CBN_SKAP_AssetType) i);
  }
  return count;
}

usz carbon_skap_count_of(const CBN_SKAP_Handle *handle, const CBN_SKAP_AssetType type) {
  if (type >= CARBON_SKAP_ASSET_TYPE_COUNT) {
    carbon_log_error("`type` is not valid");
    return 0;
  }
  return handle->type_counters[type];
}
