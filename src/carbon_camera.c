// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

struct CBN_Camera {
  CBN_Camera_Type type;
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
  f32 ortho_size;
};

CBN_Camera *carbon_camera_create(const CBN_DrawCanvas *dc) {
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

void carbon_camera_reset(CBN_Camera *c, const CBN_DrawCanvas *dc) {
  if (!c) {
    CBN_WARN("`c` is not a valid pointer, skipping reset");
    return;
  }
  c->position = carbon_math_vec3_1(0);
  c->yaw = 0;
  c->pitch = 0;
  c->rotation = carbon_math_quat_id();
  c->view = carbon_math_mat4_view(c->position, c->rotation);
  c->fov = 60;
  c->aspect = (f32) dc->width / dc->height;
  c->near = 0.1;
  c->far = 100.0;
  c->type = CARBON_CAMERA_TYPE_PERSPECTIVE;
  c->proj = carbon_math_mat4_perspective(c->fov, c->aspect, c->near, c->far);
  c->ortho_size = 1;
}

CBNINL void carbon_camera__update_view(CBN_Camera *c) {
  c->view = carbon_math_mat4_view(c->position, c->rotation);
}

CBNINL void carbon_camera__update_proj(CBN_Camera *c) {
  switch (c->type) {
  case CARBON_CAMERA_TYPE_PERSPECTIVE:
    c->proj = carbon_math_mat4_perspective(c->fov, c->aspect, c->near, c->far);
    break;
  case CARBON_CAMERA_TYPE_ORTHOGRAPHIC: {
    f32 hh = c->ortho_size;
    f32 hw = hh * c->aspect;
    c->proj = carbon_math_mat4_orthographic(-hw, hw, -hh, hh, c->near, c->far);
  } break;
  default: CARBON_UNREACHABLE;
  }
}

void carbon_camera_set_type(CBN_Camera *c, const CBN_Camera_Type t) {
  if (!c) return;
  c->type = t;
  carbon_camera__update_proj(c);
}

CBN_Vec3 carbon_camera_get_position(const CBN_Camera *c) {
  if (!c) return carbon_math_vec3_1(0);
  return c->position;
}

CBN_Quat carbon_camera_get_rotation(const CBN_Camera *c) {
  if (!c) return carbon_math_quat_id();
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
  carbon_camera__translate_xz(c, carbon_math_vec3(0, 0, -1), amount);
}

void carbon_camera_move_backward(CBN_Camera *c, f32 amount) {
  carbon_camera__translate_xz(c, carbon_math_vec3(0, 0, 1), amount);
}

void carbon_camera_move_left(CBN_Camera *c, f32 amount) {
  carbon_camera__translate_xz(c, carbon_math_vec3(-1, 0, 0), amount);
}

void carbon_camera_move_right(CBN_Camera *c, f32 amount) {
  carbon_camera__translate_xz(c, carbon_math_vec3(1, 0, 0), amount);
}

void carbon_camera_move_up(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, carbon_math_vec3(0, 1, 0), amount);
}

void carbon_camera_move_down(CBN_Camera *c, f32 amount) {
  carbon_camera__translate(c, carbon_math_vec3(0, -1, 0), amount);
}

CBNINL void carbon_camera__update_rotation(CBN_Camera *c) {
  const CBN_Quat q_yaw = carbon_math_quat_from_axis_angle(carbon_math_vec3(0, 1, 0), c->yaw);
  const CBN_Vec3 v_pitch = carbon_math_vec3_rotate(carbon_math_vec3(1, 0, 0), q_yaw);
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
  c->pitch = carbon_math_clamp(c->pitch + amount, -max_pitch, max_pitch);
  carbon_camera__update_rotation(c);
}

void carbon_camera_ortho_zoom(CBN_Camera *c, f32 amount) {
  static const f32 max_zoom = 0.1;
  if (!c) return;
  c->ortho_size = CARBON_MAX(c->ortho_size + amount, max_zoom);
  if (c->type == CARBON_CAMERA_TYPE_ORTHOGRAPHIC) carbon_camera__update_proj(c);
}
