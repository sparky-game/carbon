/*
**  $$============================$$
**  ||       Sprite Manager       ||
**  $$============================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef CBN_SlotMap_Key CBN_Sprite_UID;
typedef struct CBN_SKAP CBN_SKAP;  // Forward declaration

/**
 * @brief Initializes the sprite subsystem.
 */
CBNDEF void carbon_sprite_manager_init(void);
CBNDEF_FN(cbn::sprite_mgr, Init, carbon_sprite_manager_init);

/**
 * @brief Shutdowns the sprite subsystem.
 */
CBNDEF void carbon_sprite_manager_shutdown(void);
CBNDEF_FN(cbn::sprite_mgr, Shutdown, carbon_sprite_manager_shutdown);

/**
 * @brief ...
 * @param file ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_sprite_manager_load_from_file(const char *file, CBN_Sprite_UID *out_uid);

/**
 * @param name ...
 * @param skap_handle ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_sprite_manager_load_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Sprite_UID *out_uid);

#ifdef __cplusplus
/**
 * @brief carbon_sprite_manager_load_from_file
 */
[[nodiscard]] cbn::Opt<CBN_Sprite_UID> carbon_sprite_manager_load(const char *file);
#endif

/**
 * @brief ...
 * @param uid ...
 * @return ...
 */
CBNDEF CBN_Sprite *carbon_sprite_manager_lookup(const CBN_Sprite_UID uid);
CBNDEF_FN(cbn::sprite_mgr, Lookup, carbon_sprite_manager_lookup);

// Local Variables:
// mode: c++
// End:
