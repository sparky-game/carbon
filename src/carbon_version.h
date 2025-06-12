/*
**  $$=====================$$
**  ||       Version       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Gets the library API version at runtime.
 * @param major The major component of the version (output argument pointer).
 * @param minor The minor component of the version (output argument pointer).
 * @param patch The patch component of the version (output argument pointer).
 * @return The full version string formatted as `vX.Y.Z[-EXTRA]`.
 */
CARBON_API char *carbon_version(usz *major, usz *minor, usz *patch);
