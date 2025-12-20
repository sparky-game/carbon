/*
**  $$==========================$$
**  ||       Mesh Manager       ||
**  $$==========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

typedef CBN_SlotMap_Key CBN_Mesh_UID;

/**
 * @brief Initializes the mesh subsystem.
 */
CBNDEF void carbon_mesh_manager_init(void);
CBNDEF_FN(cbn::mesh_mgr, Init, carbon_mesh_manager_init);

/**
 * @brief Shutdowns the mesh subsystem.
 */
CBNDEF void carbon_mesh_manager_shutdown(void);
CBNDEF_FN(cbn::mesh_mgr, Shutdown, carbon_mesh_manager_shutdown);

/**
 * @brief ...
 * @param file ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_mesh_manager_load_from_file(const char *file, CBN_Mesh_UID *out_uid);

/**
 * @param name ...
 * @param skap_handle ...
 * @param out_uid ... (output argument pointer).
 * @return ...
 */
CBNDEF bool carbon_mesh_manager_load_from_skap(const char *name, const CBN_SKAP *skap_handle, CBN_Mesh_UID *out_uid);

#ifdef __cplusplus
/**
 * @brief carbon_sprite_manager_load_from_file
 */
[[nodiscard]] cbn::Opt<CBN_Mesh_UID> carbon_mesh_manager_load(const char *file);
#endif

/**
 * @brief ...
 * @param uid ...
 * @return ...
 */
CBNDEF CBN_Mesh *carbon_mesh_manager_lookup(const CBN_Mesh_UID uid);
CBNDEF_FN(cbn::mesh_mgr, Lookup, carbon_mesh_manager_lookup);

// Local Variables:
// mode: c++
// End:
