/*
**  $$=======================$$
**  ||       Windowing       ||
**  $$=======================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

CARBON_API void carbon_win_open(u16 width, u16 height, const char *title);
CARBON_API void carbon_win_close(void);
CARBON_API void carbon_win_set_max_fps(u32 fps);

/**
 * @brief Set the window icon to a specific image object.
 *
 * The windowing module holds the ownership of the image object passed through.
 * This means you mustn't worry about managing its memory, it's the window's
 * responsability when it closes.
 *
 * @param img The image object of the icon.
 */
CARBON_API void carbon_win_set_icon(CBN_Image img);

CARBON_API f64 carbon_win_get_deltatime(void);
CARBON_API void carbon_win_update(CBN_DrawCanvas dc);

/**
 * @brief Checks if the window should close or not.
 * @return A boolean value representing whether the window should close or not.
 */
CARBON_API u8 carbon_win_shouldclose(void);

#ifdef __cplusplus
/**
 * @brief Organizes the main window loop by calling back to the user's code for each frame.
 * @param callback The code to be executed in each frame, wrapped around to provide the dt (DeltaTime).
 */
template <typename T>
void carbon_win_forframe(T &&callback) {
  while (!carbon_win_shouldclose()) {
    const auto dt = carbon_win_get_deltatime();
    callback(dt);
  }
}
#endif

// Local Variables:
// mode: c++
// End:
