// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

using namespace cbn::math;

cbn::Camera *cbn::Camera::make(const cbn::DrawCanvas *dc) {
  return carbon_camera_create(dc);
}

cbn::Scope<cbn::Camera> cbn::Camera::make_unique(const cbn::DrawCanvas *dc) {
  return {
    make(dc),
    [](cbn::Camera *c){ if (c) c->Free(); }
  };
}

void cbn::Camera::Free(void) {
  carbon_camera_destroy(this);
}

void cbn::Camera::Reset(const cbn::DrawCanvas &dc) {
  carbon_camera_reset(this, &dc);
}

void cbn::Camera::SetType(const Type t) {
  carbon_camera_set_type(this, t);
}

Vec3 cbn::Camera::GetPosition(void) const {
  return carbon_camera_get_position(this);
}

Quat cbn::Camera::GetRotation(void) const {
  return carbon_camera_get_rotation(this);
}

Mat4 cbn::Camera::GetView(void) const {
  return carbon_camera_get_view(this);
}

Mat4 cbn::Camera::GetProj(void) const {
  return carbon_camera_get_proj(this);
}

void cbn::Camera::MoveForward(f32 amount) {
  carbon_camera_move_forward(this, amount);
}

void cbn::Camera::MoveBackward(f32 amount) {
  carbon_camera_move_backward(this, amount);
}

void cbn::Camera::MoveLeft(f32 amount) {
  carbon_camera_move_left(this, amount);
}

void cbn::Camera::MoveRight(f32 amount) {
  carbon_camera_move_right(this, amount);
}

void cbn::Camera::MoveUp(f32 amount) {
  carbon_camera_move_up(this, amount);
}

void cbn::Camera::MoveDown(f32 amount) {
  carbon_camera_move_down(this, amount);
}

void cbn::Camera::Yaw(f32 amount) {
  carbon_camera_yaw(this, amount);
}

void cbn::Camera::Pitch(f32 amount) {
  carbon_camera_pitch(this, amount);
}

void cbn::Camera::OrthoZoom(f32 amount) {
  carbon_camera_ortho_zoom(this, amount);
}
