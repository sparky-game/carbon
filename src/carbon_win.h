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

#define CARBON_WIN_MOUSEBUTTONS                         \
  x(Left) x(Right) x(Middle) x(ScrollUp) x(ScrollDown)

enum CBN_KeyCode_t {
#define x(name) CARBON_KEY_CODE_ ## name,
  CARBON_WIN_KEYCODES
#undef x
};

enum CBN_MouseButton_t {
#define x(name) CARBON_MOUSE_BUTTON_ ## name,
  CARBON_WIN_MOUSEBUTTONS
#undef x
};

#ifdef __cplusplus
enum struct CBN_KeyCode {
#define x(name) name = CARBON_KEY_CODE_ ## name,
  CARBON_WIN_KEYCODES
#undef x
};
enum struct CBN_MouseButton {
#define x(name) CARBON_MOUSE_BUTTON_ ## name,
  CARBON_WIN_MOUSEBUTTONS
#undef x
};
#else
typedef enum CBN_KeyCode_t CBN_KeyCode;
typedef enum CBN_MouseButton_t CBN_MouseButton;
#endif

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

CARBON_API u32 carbon_win_get_fps(void);

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

/**
 * @brief Returns true during the frame the user starts pressing down the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key was pressed or not.
 */
CARBON_API u8 carbon_win_get_key_down(const CBN_KeyCode key);

/**
 * @brief Returns true while the user holds down the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key is being pressed or not.
 */
CARBON_API u8 carbon_win_get_key(const CBN_KeyCode key);

/**
 * @brief Returns true during the frame the user releases the specified key.
 * @param key The KeyCode to evaluate.
 * @return A boolean value representing whether the key was released or not.
 */
CARBON_API u8 carbon_win_get_key_up(const CBN_KeyCode key);

/**
 * @brief Returns true during the frame the user starts pressing down the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button was pressed or not.
 */
CARBON_API u8 carbon_win_get_mouse_button_down(const CBN_MouseButton btn);

/**
 * @brief Returns true while the user holds down the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button is being pressed or not.
 */
CARBON_API u8 carbon_win_get_mouse_button(const CBN_MouseButton btn);

/**
 * @brief Returns true during the frame the user releases the specified mouse button.
 * @param btn The MouseButton to evaluate.
 * @return A boolean value representing whether the mouse button was released or not.
 */
CARBON_API u8 carbon_win_get_mouse_button_up(const CBN_MouseButton btn);

/**
 * @brief Returns the actual mouse position within the window.
 * @return A Vec2 representing the position.
 */
CARBON_API CBN_Vec2 carbon_win_get_mouse_position(void);

// Local Variables:
// mode: c++
// End:
