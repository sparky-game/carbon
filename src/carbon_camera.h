/*
**  $$====================$$
**  ||       Camera       ||
**  $$====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a 3D camera for rendering.
 *
 * It's defined as an opaque type, because the member data is ought to be
 * retrieved through the provided getter functions.
 * Thanks to this mechanism, it's possible to cache important information
 * up until they need to be changed, thus avoiding unnecessary computations.
 */
typedef struct CBN_Camera CBN_Camera;
#ifdef __cplusplus
struct CBN_Camera {
  /**
   * @see carbon_camera_create
   */
  static CBN_Camera *make(const CBN_DrawCanvas &dc);
  /**
   * @see carbon_camera_destroy
   */
  void Free(void);
  /**
   * @see carbon_camera_reset
   */
  void Reset(const CBN_DrawCanvas &dc);
  /**
   * @see carbon_camera_get_position
   */
  CBN_Vec3 GetPosition(void) const;
  /**
   * @see carbon_camera_get_rotation
   */
  CBN_Quat GetRotation(void) const;
  /**
   * @see carbon_camera_get_view
   */
  CBN_Mat4 GetView(void) const;
  /**
   * @see carbon_camera_get_proj
   */
  CBN_Mat4 GetProj(void) const;
  /**
   * @see carbon_camera_move_forward
   */
  void MoveForward(f32 amount);
  /**
   * @see carbon_camera_move_backward
   */
  void MoveBackward(f32 amount);
  /**
   * @see carbon_camera_move_left
   */
  void MoveLeft(f32 amount);
  /**
   * @see carbon_camera_move_right
   */
  void MoveRight(f32 amount);
  /**
   * @see carbon_camera_yaw
   */
  void Yaw(f32 amount);
  /**
   * @see carbon_camera_pitch
   */
  void Pitch(f32 amount);
};
#endif

/**
 * @brief Creates a new Camera object.
 * @param dc The DrawCanvas to get the needed info from.
 * @return The newly created Camera object.
 */
CBNDEF CBN_Camera *carbon_camera_create(const CBN_DrawCanvas dc);

/**
 * @brief Destroys a Camera object.
 * @param c The Camera to destroy.
 */
CBNDEF void carbon_camera_destroy(CBN_Camera *c);

/**
 * @brief Re-creates the Camera, resetting it as if it was newly created.
 * @param c The Camera object.
 * @param dc The DrawCanvas to get the needed info from.
 */
CBNDEF void carbon_camera_reset(CBN_Camera *c, const CBN_DrawCanvas dc);

/**
 * @brief Obtains the current position of the Camera as 3D vector.
 * @param c The Camera object.
 * @return The Camera position 3D vector.
 */
CBNDEF CBN_Vec3 carbon_camera_get_position(const CBN_Camera *c);

/**
 * @brief Obtains the current rotation of the Camera as quaternion.
 * @param c The Camera object.
 * @return The Camera rotation quaternion.
 */
CBNDEF CBN_Quat carbon_camera_get_rotation(const CBN_Camera *c);

/**
 * @brief Obtains the current view 4x4 matrix of the Camera.
 * @param c The Camera object.
 * @return The Camera view 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_camera_get_view(const CBN_Camera *c);

/**
 * @brief Obtains the current projection 4x4 matrix of the Camera.
 * @param c The Camera object.
 * @return The Camera projection 4x4 matrix.
 */
CBNDEF CBN_Mat4 carbon_camera_get_proj(const CBN_Camera *c);

/**
 * @brief Moves the Camera forward (-Z) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_move_forward(CBN_Camera *c, f32 amount);

/**
 * @brief Moves the Camera backward (+Z) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_move_backward(CBN_Camera *c, f32 amount);

/**
 * @brief Moves the Camera left (-X) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_move_left(CBN_Camera *c, f32 amount);

/**
 * @brief Moves the Camera right (+X) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_move_right(CBN_Camera *c, f32 amount);

/**
 * @brief Adjusts the Camera's yaw (Y-axis) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_yaw(CBN_Camera *c, f32 amount);

/**
 * @brief Adjusts the Camera's pitch (X-axis) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_pitch(CBN_Camera *c, f32 amount);

// Local Variables:
// mode: c++
// End:
