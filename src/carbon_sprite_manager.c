// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

static CBN_SlotMap carbon_sprite__library;

void carbon_sprite_manager_init(void) {
  carbon_sprite__library = carbon_slotmap_create(sizeof(CBN_Sprite *));
  CBN_INFO("Initialized sprite subsystem successfully");
}

void carbon_sprite_manager_shutdown(void) {
  if (carbon_sprite__library.size) {
    carbon_slotmap_foreach(CBN_Sprite *, carbon_sprite__library) {
      carbon_sprite_destroy(it.var);
      CBN_FREE(it.var);
    }
  }
  carbon_slotmap_destroy(&carbon_sprite__library);
  CBN_INFO("Shutdowned sprite subsystem successfully");
}

u8 carbon_sprite_manager_load_from_file(const char *file, CBN_Sprite_UID *out_uid) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  CBN_Sprite *sprite = (CBN_Sprite *) CBN_MALLOC(sizeof(CBN_Sprite));
  CBN_ASSERT(sprite && "failed to allocate memory");
  CBN_Image img = carbon_fs_read_img_from_file(file);
  *sprite = carbon_sprite_from_img(&img);
  carbon_fs_destroy_img(&img);
  *out_uid = carbon_slotmap_push(&carbon_sprite__library, &sprite);
  return true;
}

u8 carbon_sprite_manager_load_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Sprite_UID *out_uid) {
  if (!out_uid) {
    CBN_ERROR("`out_uid` must be a valid pointer");
    return false;
  }
  CBN_Sprite *sprite = (CBN_Sprite *) CBN_MALLOC(sizeof(CBN_Sprite));
  CBN_ASSERT(sprite && "failed to allocate memory");
  CBN_Image img;
  if (!carbon_skap_lookup(skap_handle, CARBON_SKAP_ASSET_TYPE_IMAGE, name, &img)) return false;
  *sprite = carbon_sprite_from_img(&img);
  carbon_fs_destroy_img(&img);
  *out_uid = carbon_slotmap_push(&carbon_sprite__library, &sprite);
  return true;
}

CBN_Sprite *carbon_sprite_manager_lookup(const CBN_Sprite_UID uid) {
  CBN_Sprite *sprite = 0;
  carbon_slotmap_lookup(&carbon_sprite__library, uid, &sprite);
  return sprite;
}
