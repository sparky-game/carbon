// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

static CBN_SlotMap carbon_sprite__library;

void carbon_sprite_manager_init(void) {
  carbon_sprite__library = carbon_slotmap_create(sizeof(CBN_Sprite *));
  CBN_INFO("Initialized sprite subsystem successfully");
}

void carbon_sprite_manager_shutdown(void) {
  if (carbon_sprite__library.size) {
    carbon_slotmap_foreach(CBN_Sprite *, carbon_sprite__library) {
      carbon_image_destroy(it.var);
      carbon_memory_free(it.var);
    }
  }
  carbon_slotmap_destroy(&carbon_sprite__library);
  CBN_INFO("Shutdowned sprite subsystem successfully");
}

bool carbon_sprite_manager_load_from_file(const char *file, CBN_Sprite_UID *out_uid) {
  if (!out_uid) return false;
  CBN_Sprite *img = carbon_memory_alloc(sizeof(CBN_Sprite));
  *img = carbon_image_read_from_file(file);
  *out_uid = carbon_slotmap_set(&carbon_sprite__library, &img);
  return true;
}

bool carbon_sprite_manager_load_from_skap(const char *name, const CBN_SKAP *skap, CBN_Sprite_UID *out_uid) {
  if (!out_uid) return false;
  CBN_Sprite *img = carbon_memory_alloc(sizeof(CBN_Sprite));
  if (!carbon_skap_lookup(skap, CARBON_SKAP_ASSET_TYPE_IMAGE, name, img)) {
    carbon_memory_free(img);
    return false;
  }
  *out_uid = carbon_slotmap_set(&carbon_sprite__library, &img);
  return true;
}

void carbon_sprite_manager_unload(CBN_Sprite_UID uid) {
  CBN_Sprite *img = carbon_sprite_manager_lookup(uid);
  if (!img) return;
  carbon_image_destroy(img);
  carbon_memory_free(img);
  carbon_slotmap_remove(&carbon_sprite__library, uid);
}

CBN_Sprite *carbon_sprite_manager_lookup(CBN_Sprite_UID uid) {
  CBN_Sprite *img = 0;
  carbon_slotmap_get(&carbon_sprite__library, uid, &img);
  return img;
}
