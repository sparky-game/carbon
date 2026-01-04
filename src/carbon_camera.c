// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

struct CBN_Camera {
  CBN_Vec3 position;
  f32 yaw;
  f32 pitch;
  CBN_Quat rotation;
  CBN_Mat4 view;
  f32 fov;
  f32 aspect;
  f32 near;
  f32 far;
  CBN_Mat4 proj;
};

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
  c->yaw = 0;
  c->pitch = 0;
  c->rotation = CARBON_QUAT_ID;
  c->view = carbon_math_mat4_view(c->position, c->rotation);
  c->fov = 60;
  c->aspect = (f32) dc.width / dc.height;
  c->near = 0.1;
  c->far = 100.0;
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

CBNINL void carbon_camera__update_view(CBN_Camera *c) {
  c->view = carbon_math_mat4_view(c->position, c->rotation);
}

CBNINL void carbon_camera__translate(CBN_Camera *c, CBN_Vec3 v, f32 amount) {
  if (!c || !amount) return;
  v = carbon_math_vec3_scale(carbon_math_vec3_norm(v), amount);
  c->position = carbon_math_vec3_add(c->position, v);
  carbon_camera__update_view(c);
}

CBNINL void carbon_camera__translate_xz(CBN_Camera *c, CBN_Vec3 v, f32 amount) {
  if (!c || !amount) return;
  v = carbon_math_vec3_rotate(v, c->rotation);
  v.y = 0;
  carbon_camera__translate(c, v, amount);
}

void carbon_camera_move_forward(CBN_Camera *c, f32 amount) {
  carbon_camera__translate_xz(c, CARBON_VEC3_FORWARD, amount);
}

void carbon_camera_move_backward(CBN_Camera *c, f32 amount) {
  carbon_camera__translate_xz(c, CARBON_VEC3_BACK, amount);
}

void carbon_camera_move_left(CBN_Camera *c, f32 amount) {
  carbon_camera__translate_xz(c, CARBON_VEC3_LEFT, amount);
}

void carbon_camera_move_right(CBN_Camera *c, f32 amount) {
  carbon_camera__translate_xz(c, CARBON_VEC3_RIGHT, amount);
}

void carbon_camera_move_up(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, CARBON_VEC3_UP, amount);
}

void carbon_camera_move_down(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, CARBON_VEC3_DOWN, amount);
}

CBNINL void carbon_camera__update_rotation(CBN_Camera *c) {
  const CBN_Quat q_yaw = carbon_math_quat_from_axis_angle(CARBON_VEC3_UP, c->yaw);
  const CBN_Vec3 v_pitch = carbon_math_vec3_rotate(CARBON_VEC3_RIGHT, q_yaw);
  const CBN_Quat q_pitch = carbon_math_quat_from_axis_angle(v_pitch, c->pitch);
  c->rotation = carbon_math_quat_mult(q_pitch, q_yaw);
  carbon_camera__update_view(c);
}

void carbon_camera_yaw(CBN_Camera *c, f32 amount) {
  if (!c || !amount) return;
  c->yaw += amount;
  carbon_camera__update_rotation(c);
}

void carbon_camera_pitch(CBN_Camera *c, f32 amount) {
  static const f32 max_pitch = 89;
  if (!c || !amount) return;
  c->pitch = CARBON_CLAMP(c->pitch + amount, -max_pitch, max_pitch);
  carbon_camera__update_rotation(c);
}
