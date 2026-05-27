/*
  $$============================$$
  ||       Sprite Manager       ||
  $$============================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

typedef CBN_SlotMap_Key CBN_Sprite_UID;
CBNDEF_TAKA(cbn::sprite_mgr, UID, CBN_Sprite_UID);

// Forward declaration
CBNDEF_T(CBN_SKAP);

/**
 * @brief Initializes the sprite subsystem.
 */
CBNDEF void carbon_sprite_manager_init(void);
CBNDEF_AKA(cbn::sprite_mgr, Init, carbon_sprite_manager_init);

/**
 * @brief Shutdowns the sprite subsystem.
 */
CBNDEF void carbon_sprite_manager_shutdown(void);
CBNDEF_AKA(cbn::sprite_mgr, Shutdown, carbon_sprite_manager_shutdown);

/**
 * @brief Loads a sprite from an image file and registers it into the subsystem.
 * @param file The path to the image file to load.
 * @param out_uid The loaded sprite's UID (output argument pointer).
 * @return A boolean value indicating whether it loaded successfully or not.
 */
CBNDEF bool carbon_sprite_manager_load_from_file(const char *file, CBN_Sprite_UID *out_uid);
CBNDEF_AKA(cbn::sprite_mgr, LoadFromFile, carbon_sprite_manager_load_from_file);

/**
 * @brief Loads a sprite from a SKAP asset package and registers it into the subsystem.
 * @param name Name of the image asset to lookup within the SKAP package.
 * @param skap_handle The SKAP object handle to lookup the sprite from.
 * @param out_uid The loaded sprite's UID (output argument pointer).
 * @return A boolean value indicating whether it loaded successfully or not.
 */
CBNDEF bool carbon_sprite_manager_load_from_skap(const char *name, const CBN_SKAP *skap, CBN_Sprite_UID *out_uid);
CBNDEF_AKA(cbn::sprite_mgr, LoadFromSKAP, carbon_sprite_manager_load_from_skap);

/**
 * @brief Unloads and frees a sprite from the subsystem by its UID.
 * @param uid The UID of the sprite to unload.
 */
CBNDEF void carbon_sprite_manager_unload(CBN_Sprite_UID uid);
CBNDEF_AKA(cbn::sprite_mgr, Unload, carbon_sprite_manager_unload);

/**
 * @brief Looks up a sprite in the subsystem by its UID.
 * @param uid The UID of the sprite to retrieve.
 * @return A pointer to the corresponding sprite, or null if the UID was invalid.
 */
CBNDEF CBN_Sprite *carbon_sprite_manager_lookup(CBN_Sprite_UID uid);
CBNDEF_AKA(cbn::sprite_mgr, Lookup, carbon_sprite_manager_lookup);
