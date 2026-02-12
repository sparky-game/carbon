/*
  $$====================$$
  ||       Camera       ||
  $$====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a 3D camera for rendering.
 */
typedef struct CBN_Camera CBN_Camera;
CBNDEF_AKA(cbn, Camera, CBN_Camera);

/**
 * @brief Types of 3D projections used to display the camera's frustum on a 2D canvas.
 */
typedef enum {
  CARBON_CAMERA_TYPE_PERSPECTIVE,
  CARBON_CAMERA_TYPE_ORTHOGRAPHIC
} CBN_Camera_Type;

typedef struct CBN_DrawCanvas CBN_DrawCanvas;  // Forward declaration

/**
 * @brief Creates a new Camera object.
 * @param dc The DrawCanvas to get the needed info from.
 * @return The newly created Camera object.
 */
CBNDEF CBN_Camera *carbon_camera_create(const CBN_DrawCanvas *dc);

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
CBNDEF void carbon_camera_reset(CBN_Camera *c, const CBN_DrawCanvas *dc);

/**
 * @brief ...
 * @param c The Camera object.
 * @param t ...
 */
CBNDEF void carbon_camera_set_type(CBN_Camera *c, const CBN_Camera_Type t);

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
 * @brief Moves the Camera up (+Y) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_move_up(CBN_Camera *c, f32 amount);

/**
 * @brief Moves the Camera down (-Y) by the given amount.
 * @param c The Camera object.
 * @param amount The amount to move.
 */
CBNDEF void carbon_camera_move_down(CBN_Camera *c, f32 amount);

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

/**
 * @brief ...
 * @param c The Camera object.
 * @param amount ...
 */
CBNDEF void carbon_camera_ortho_zoom(CBN_Camera *c, f32 amount);

#ifdef __cplusplus
struct CBN_Camera {
  using Type = CBN_Camera_Type;
  
  static cbn::Scope<CBN_Camera> New(const CBN_DrawCanvas *dc) {
    return {
      carbon_camera_create(dc),
      [](CBN_Camera *c){ if (c) c->Free(); }
    };
  }
  
  void Free(void) {
    carbon_camera_destroy(this);
  }

  void Reset(const CBN_DrawCanvas &dc) {
    carbon_camera_reset(this, &dc);
  }

  void SetType(const Type t) {
    carbon_camera_set_type(this, t);
  }

  CBN_Vec3 GetPosition(void) const {
    return carbon_camera_get_position(this);
  }

  CBN_Quat GetRotation(void) const {
    return carbon_camera_get_rotation(this);
  }

  CBN_Mat4 GetView(void) const {
    return carbon_camera_get_view(this);
  }

  CBN_Mat4 GetProj(void) const {
    return carbon_camera_get_proj(this);
  }

  void MoveForward(f32 amount) {
    carbon_camera_move_forward(this, amount);
  }

  void MoveBackward(f32 amount) {
    carbon_camera_move_backward(this, amount);
  }

  void MoveLeft(f32 amount) {
    carbon_camera_move_left(this, amount);
  }
  
  void MoveRight(f32 amount) {
    carbon_camera_move_right(this, amount);
  }
  
  void MoveUp(f32 amount) {
    carbon_camera_move_up(this, amount);
  }
  
  void MoveDown(f32 amount) {
    carbon_camera_move_down(this, amount);
  }

  void Yaw(f32 amount) {
    carbon_camera_yaw(this, amount);
  }
  
  void Pitch(f32 amount) {
    carbon_camera_pitch(this, amount);
  }

  void OrthoZoom(f32 amount) {
    carbon_camera_ortho_zoom(this, amount);
  }
};
#endif
