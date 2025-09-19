// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

typedef struct CBN_Camera {
  CBN_Vec3 position;
  CBN_Quat rotation;
  CBN_Mat4 view;
  f32 fov;
  f32 aspect;
  f32 near;
  f32 far;
  CBN_Mat4 proj;
} CBN_Camera;

CBN_Camera *carbon_camera_create(const CBN_DrawCanvas dc) {
  CBN_Camera *c = (CBN_Camera *) carbon_memory_alloc(sizeof(CBN_Camera));
  carbon_camera_reset(c, dc);
  return c;
}

void carbon_camera_destroy(CBN_Camera *c) {
  if (!c) {
    CBN_WARN("`c` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(c);
}

void carbon_camera_reset(CBN_Camera *c, const CBN_DrawCanvas dc) {
  if (!c) {
    CBN_WARN("`c` is not a valid pointer, skipping reset");
    return;
  }
  c->position = CARBON_VEC3_ZERO;
  c->rotation = CARBON_QUAT_ID;
  c->view = carbon_math_mat4_view(c->position, c->rotation);
  c->fov = 60;
  c->aspect = (f32) dc.width / dc.height;
  c->near = 0.1;
  c->far = 5.0;
  c->proj = carbon_math_mat4_perspective(c->fov, c->aspect, c->near, c->far);
}

CBN_Vec3 carbon_camera_get_position(const CBN_Camera *c) {
  if (!c) return CARBON_VEC3_ZERO;
  return c->position;
}

CBN_Quat carbon_camera_get_rotation(const CBN_Camera *c) {
  if (!c) return CARBON_QUAT_ID;
  return c->rotation;
}

CBN_Mat4 carbon_camera_get_view(const CBN_Camera *c) {
  if (!c) return carbon_math_mat4_id();
  return c->view;
}

CBN_Mat4 carbon_camera_get_proj(const CBN_Camera *c) {
  if (!c) return carbon_math_mat4_id();
  return c->proj;
}

CARBON_INLINE void carbon_camera__update_view(CBN_Camera *c) {
  if (!c) return;
  c->view = carbon_math_mat4_view(c->position, c->rotation);
}

CARBON_INLINE void carbon_camera__translate(CBN_Camera *c, CBN_Vec3 axis, f32 amount) {
  if (!c || !amount) return;
  CBN_Vec3 r = carbon_math_vec3_rotate(axis, c->rotation);
  CBN_Vec3 v = carbon_math_vec3_scale(r, amount);
  c->position = carbon_math_vec3_add(c->position, v);
  carbon_camera__update_view(c);
}

void carbon_camera_move_forward(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, CARBON_VEC3_FORWARD, amount);
}

void carbon_camera_move_backward(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, CARBON_VEC3_BACK, amount);
}

void carbon_camera_move_left(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, CARBON_VEC3_LEFT, amount);
}

void carbon_camera_move_right(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, CARBON_VEC3_RIGHT, amount);
}

CARBON_INLINE void carbon_camera__rotate(CBN_Camera *c, CBN_Vec3 axis, f32 amount) {
  if (!c || !amount) return;
  CBN_Quat r = carbon_math_quat_from_axis_angle(axis, amount);
  c->rotation = carbon_math_quat_mult(r, c->rotation);
  carbon_camera__update_view(c);
}

void carbon_camera_yaw(CBN_Camera *c, f32 amount) {
  carbon_camera__rotate(c, CARBON_VEC3_UP, amount);
}

void carbon_camera_pitch(CBN_Camera *c, f32 amount) {
  carbon_camera__rotate(c, CARBON_VEC3_RIGHT, amount);
}
