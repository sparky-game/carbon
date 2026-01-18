/*
**  $$=======================$$
**  ||       Windowing       ||
**  $$=======================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_WIN_KEYCODES                                             \
  x(A) x(B) x(C) x(D) x(E) x(F) x(G) x(H) x(I) x(J) x(K) x(L) x(M)      \
  x(N) x(O) x(P) x(Q) x(R) x(S) x(T) x(U) x(V) x(W) x(X) x(Y) x(Z)      \
  x(Zero) x(One) x(Two) x(Three) x(Four) x(Five) x(Six) x(Seven)        \
  x(Eight) x(Nine) x(BackQuote) x(F1) x(F2) x(F3) x(F4) x(F5) x(F6)     \
  x(F7) x(F8) x(F9) x(F10) x(F11) x(F12) x(Escape) x(Tab) x(CapsLock)   \
  x(LeftShift) x(LeftControl) x(LeftMeta) x(LeftAlt) x(Space)           \
  x(RightAlt) x(RightMeta) x(RightControl) x(RightShift) x(Return)      \
  x(UpArrow) x(DownArrow) x(LeftArrow) x(RightArrow)

#ifdef __cplusplus
enum struct CBN_KeyCode {
#define x(name) name,
  CARBON_WIN_KEYCODES
#undef x
};
#else
typedef enum {
#define x(name) CARBON_KEY_CODE_ ## name,
  CARBON_WIN_KEYCODES
#undef x
} CBN_KeyCode;
#endif
CBNDEF_T(cbn::win, KeyCode, CBN_KeyCode);

#define CARBON_WIN_MOUSEBUTTONS                         \
  x(Left) x(Right) x(Middle) x(ScrollUp) x(ScrollDown)

#ifdef __cplusplus
enum struct CBN_MouseButton {
#define x(name) name,
  CARBON_WIN_MOUSEBUTTONS
#undef x
};
#else
typedef enum {
#define x(name) CARBON_MOUSE_BUTTON_ ## name,
  CARBON_WIN_MOUSEBUTTONS
#undef x
} CBN_MouseButton;
#endif
CBNDEF_T(cbn::win, MouseButton, CBN_MouseButton);

/**
 */
CBNDEF void carbon_win_open(const CBN_DrawCanvas dc, const char *title);
CBNDEF_FN(cbn::win, Open, carbon_win_open);

/**
 */
CBNDEF void carbon_win_close(void);
CBNDEF_FN(cbn::win, Close, carbon_win_close);

/**
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

CBNDEF f64 carbon_win_get_deltatime(void);
CBNDEF_FN(cbn::win, GetDeltaTime, carbon_win_get_deltatime);

CBNDEF u32 carbon_win_get_fps(void);
CBNDEF_FN(cbn::win, GetFPS, carbon_win_get_fps);

CBNDEF void carbon_win_update(const CBN_DrawCanvas dc);
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
 */
CBNDEF void carbon_win_exit(void);
CBNDEF_FN(cbn::win, Exit, carbon_win_exit);

/**
 * @brief Returns true during the frame the user starts pressing down the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key was pressed or not.
 */
CBNDEF bool carbon_win_get_key_down(const CBN_KeyCode key);
CBNDEF_FN(cbn::win, GetKeyDown, carbon_win_get_key_down);

/**
 * @brief Returns true while the user holds down the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key is being pressed or not.
 */
CBNDEF bool carbon_win_get_key(const CBN_KeyCode key);
CBNDEF_FN(cbn::win, GetKey, carbon_win_get_key);

/**
 * @brief Returns true during the frame the user releases the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key was released or not.
 */
CBNDEF bool carbon_win_get_key_up(const CBN_KeyCode key);
CBNDEF_FN(cbn::win, GetKeyUp, carbon_win_get_key_up);

/**
 * @brief Returns true during the frame the user starts pressing down the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button was pressed or not.
 */
CBNDEF bool carbon_win_get_mouse_button_down(const CBN_MouseButton btn);
CBNDEF_FN(cbn::win, GetMouseButtonDown, carbon_win_get_mouse_button_down);

/**
 * @brief Returns true while the user holds down the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button is being pressed or not.
 */
CBNDEF bool carbon_win_get_mouse_button(const CBN_MouseButton btn);
CBNDEF_FN(cbn::win, GetMouseButton, carbon_win_get_mouse_button);

/**
 * @brief Returns true during the frame the user releases the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button was released or not.
 */
CBNDEF bool carbon_win_get_mouse_button_up(const CBN_MouseButton btn);
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

// Local Variables:
// mode: c++
// End:
