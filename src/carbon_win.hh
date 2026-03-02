/*
  $$=======================$$
  ||       Windowing       ||
  $$=======================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief ...
 */
#define CARBON_WIN_KEYCODES(x, p)                                       \
  x(p, A) x(p, B) x(p, C) x(p, D) x(p, E) x(p, F) x(p, G) x(p, H)       \
  x(p, I) x(p, J) x(p, K) x(p, L) x(p, M) x(p, N) x(p, O) x(p, P)       \
  x(p, Q) x(p, R) x(p, S) x(p, T) x(p, U) x(p, V) x(p, W) x(p, X)       \
  x(p, Y) x(p, Z) x(p, Zero) x(p, One) x(p, Two) x(p, Three) x(p, Four) \
  x(p, Five) x(p, Six) x(p, Seven) x(p, Eight) x(p, Nine)               \
  x(p, BackQuote) x(p, F1) x(p, F2) x(p, F3) x(p, F4) x(p, F5) x(p, F6) \
  x(p, F7) x(p, F8) x(p, F9) x(p, F10) x(p, F11) x(p, F12) x(p, Escape) \
  x(p, Tab) x(p, CapsLock) x(p, LeftShift) x(p, LeftCtrl)               \
  x(p, LeftMeta) x(p, LeftAlt) x(p, Space) x(p, RightAlt)               \
  x(p, RightMeta) x(p, RightCtrl) x(p, RightShift) x(p, Return)         \
  x(p, UpArrow) x(p, DownArrow) x(p, LeftArrow) x(p, RightArrow)
CBNDEF_ENUM(CBN_Win_KeyCode, CARBON_WIN_KEYCODES);
CBNDEF_AKA(cbn::win, KeyCode, CBN_Win_KeyCode);

/**
 * @brief ...
 */
#define CARBON_WIN_MOUSEBUTTONS(x, p)           \
  x(p, Left) x(p, Right) x(p, Middle)           \
  x(p, ScrollUp) x(p, ScrollDown)
CBNDEF_ENUM(CBN_Win_MouseButton, CARBON_WIN_MOUSEBUTTONS);
CBNDEF_AKA(cbn::win, MouseButton, CBN_Win_MouseButton);

/**
 * @brief ...
 * @param dc ...
 * @param title ...
 */
CBNDEF void carbon_win_open(const CBN_DrawCanvas *dc, const char *title);
CBNDEF_FN(cbn::win, Open, carbon_win_open);

/**
 * @brief ...
 */
CBNDEF void carbon_win_close(void);
CBNDEF_FN(cbn::win, Close, carbon_win_close);

/**
 * @brief ...
 * @param fps ...
 */
CBNDEF void carbon_win_set_max_fps(u32 fps);
CBNDEF_FN(cbn::win, SetMaxFPS, carbon_win_set_max_fps);

/**
 * @brief Set the window icon to a specific image object.
 *
 * The windowing module holds the ownership of the image object passed through.
 * This means you mustn't worry about managing its memory, it's the window's
 * responsability when it closes.
 *
 * @param img The image object of the icon.
 */
CBNDEF void carbon_win_set_icon(CBN_Image img);
CBNDEF_FN(cbn::win, SetIcon, carbon_win_set_icon);

/**
 * @brief ...
 * @return ...
 */
CBNDEF f64 carbon_win_get_deltatime(void);
CBNDEF_FN(cbn::win, GetDeltaTime, carbon_win_get_deltatime);

/**
 * @brief ...
 * @return ...
 */
CBNDEF u32 carbon_win_get_fps(void);
CBNDEF_FN(cbn::win, GetFPS, carbon_win_get_fps);

/**
 */
CBNDEF void carbon_win_update(const CBN_DrawCanvas *dc);
CBNDEF_FN(cbn::win, Update, carbon_win_update);

/**
 * @brief Checks if the window should close or not.
 * @return A boolean value representing whether the window should close or not.
 */
CBNDEF bool carbon_win_shouldclose(void);
CBNDEF_FN(cbn::win, ShouldClose, carbon_win_shouldclose);

/**
 * @brief Organizes the main window loop by calling back to the user's code for each frame.
 * @param callback The code to be executed in each frame, wrapped around to provide the dt (DeltaTime).
 */
#ifdef __cplusplus
namespace cbn::win {
  template <typename T>
  void ForFrame(T &&callback) {
    while (!ShouldClose()) {
      const auto dt = GetDeltaTime();
      callback(dt);
    }
  }
}
#endif

/**
 * @brief Requests the main window loop to be ended immediately, but gracefully nonetheless.
 */
CBNDEF void carbon_win_exit(void);
CBNDEF_FN(cbn::win, Exit, carbon_win_exit);

/**
 * @brief Returns true during the frame the user starts pressing down the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key was pressed or not.
 */
CBNDEF bool carbon_win_get_key_down(CBN_Win_KeyCode key);
CBNDEF_FN(cbn::win, GetKeyDown, carbon_win_get_key_down);

/**
 * @brief Returns true while the user holds down the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key is being pressed or not.
 */
CBNDEF bool carbon_win_get_key(CBN_Win_KeyCode key);
CBNDEF_FN(cbn::win, GetKey, carbon_win_get_key);

/**
 * @brief Returns true during the frame the user releases the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key was released or not.
 */
CBNDEF bool carbon_win_get_key_up(CBN_Win_KeyCode key);
CBNDEF_FN(cbn::win, GetKeyUp, carbon_win_get_key_up);

/**
 * @brief Returns true during the frame the user starts pressing down the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button was pressed or not.
 */
CBNDEF bool carbon_win_get_mouse_button_down(CBN_Win_MouseButton btn);
CBNDEF_FN(cbn::win, GetMouseButtonDown, carbon_win_get_mouse_button_down);

/**
 * @brief Returns true while the user holds down the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button is being pressed or not.
 */
CBNDEF bool carbon_win_get_mouse_button(CBN_Win_MouseButton btn);
CBNDEF_FN(cbn::win, GetMouseButton, carbon_win_get_mouse_button);

/**
 * @brief Returns true during the frame the user releases the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button was released or not.
 */
CBNDEF bool carbon_win_get_mouse_button_up(CBN_Win_MouseButton btn);
CBNDEF_FN(cbn::win, GetMouseButtonUp, carbon_win_get_mouse_button_up);

/**
 * @brief Returns the actual mouse position within the window.
 * @return A Vec2 representing the position.
 */
CBNDEF CBN_Vec2 carbon_win_get_mouse_position(void);
CBNDEF_FN(cbn::win, GetMousePosition, carbon_win_get_mouse_position);

/**
 * @brief Sets the visibility state of the mouse cursor, between shown or hidden.
 * @param visible A boolean value representing whether the mouse cursor must be visible or not.
 */
CBNDEF void carbon_win_set_mouse_visibility(bool visible);
CBNDEF_FN(cbn::win, SetMouseVisibility, carbon_win_set_mouse_visibility);

/**
 * @brief Sets the visibility state of the window border, between shown or hidden.
 * @param visible A boolean value representing whether the window border must be visible or not.
 */
CBNDEF void carbon_win_set_border_visibility(bool visible);
CBNDEF_FN(cbn::win, SetBorderVisibility, carbon_win_set_border_visibility);

/**
 * @brief Makes the window fullscreen or not.
 * @param yn A boolean value representing whether the window must be in fullscreen or not.
 */
CBNDEF void carbon_win_set_fullscreen(bool yn);
CBNDEF_FN(cbn::win, SetFullscreen, carbon_win_set_fullscreen);
