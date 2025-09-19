// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Camera *CBN_Camera::make(const CBN_DrawCanvas &dc) {
  return carbon_camera_create(dc);
}

void CBN_Camera::Free(void) {
  carbon_camera_destroy(this);
}

void CBN_Camera::Reset(const CBN_DrawCanvas &dc) {
  carbon_camera_reset(this, dc);
}

CBN_Vec3 CBN_Camera::GetPosition(void) const {
  return carbon_camera_get_position(this);
}

CBN_Quat CBN_Camera::GetRotation(void) const {
  return carbon_camera_get_rotation(this);
}

CBN_Mat4 CBN_Camera::GetView(void) const {
  return carbon_camera_get_view(this);
}

CBN_Mat4 CBN_Camera::GetProj(void) const {
  return carbon_camera_get_proj(this);
}

void CBN_Camera::MoveForward(f32 amount) {
  carbon_camera_move_forward(this, amount);
}

void CBN_Camera::MoveBackward(f32 amount) {
  carbon_camera_move_backward(this, amount);
}

void CBN_Camera::MoveLeft(f32 amount) {
  carbon_camera_move_left(this, amount);
}

void CBN_Camera::MoveRight(f32 amount) {
  carbon_camera_move_right(this, amount);
}

void CBN_Camera::Yaw(f32 amount) {
  carbon_camera_yaw(this, amount);
}

void CBN_Camera::Pitch(f32 amount) {
  carbon_camera_pitch(this, amount);
}
