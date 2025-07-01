/*
**  $$===================$$
**  ||       Audio       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Initializes the audio subsystem.
 */
CARBON_API void carbon_audio_init(void);

/**
 * @brief Shutdowns the audio subsystem.
 */
CARBON_API void carbon_audio_shutdown(void);

/**
 * @brief ...
 */
CARBON_API u8 carbon_audio_load_sound_from_file(const char *file, CBN_SlotMap_Key *out_key);

/**
 * @brief ...
 */
CARBON_API void carbon_audio_play_sound(const CBN_SlotMap_Key key);
