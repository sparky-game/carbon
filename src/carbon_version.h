/*
  $$=====================$$
  ||       Version       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Gets the library API version at runtime.
 * @return The version packed in u32 ([0x00, 0xMM, 0xNN, 0xPP]).
 */
CBNDEF u32 carbon_version(void);
CBNDEF_FN(cbn, Version, carbon_version);

/**
 * @brief Gets the library API version (as string) at runtime.
 * @return The version as a string.
 */
CBNDEF char *carbon_version_str(void);
CBNDEF_FN(cbn, VersionStr, carbon_version_str);
