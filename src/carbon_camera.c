// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_CAMERA__PITCH_MAX 89

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
  c->position   = carbon_math_vec3_1(0);
  c->yaw        = 0;
  c->pitch      = 0;
  c->rotation   = carbon_math_quat_id();
  c->view       = carbon_math_mat4_view(c->position, c->rotation);
  c->fov        = 60;
  c->aspect     = (f32) carbon_drawcanvas_width(dc) / carbon_drawcanvas_height(dc);
  c->near       = 0.1;
  c->far        = 100.0;
  c->type       = CBN_Camera_Type_Perspective;
  c->proj       = carbon_math_mat4_perspective(c->fov, c->aspect, c->near, c->far);
  c->ortho_size = 1;
}

CBNINL void carbon_camera__update_view(CBN_Camera *c) {
  c->view = carbon_math_mat4_view(c->position, c->rotation);
}

CBNINL void carbon_camera__update_proj(CBN_Camera *c) {
  switch (c->type) {
  case CBN_Camera_Type_Perspective:
    c->proj = carbon_math_mat4_perspective(c->fov, c->aspect, c->near, c->far);
    break;
  case CBN_Camera_Type_Orthographic: {
    f32 hh = c->ortho_size;
    f32 hw = hh * c->aspect;
    c->proj = carbon_math_mat4_orthographic(-hw, hw, -hh, hh, c->near, c->far);
  } break;
  case CBN_Camera_Type_Count:
  default: CARBON_UNREACHABLE;
  }
}

void carbon_camera_set_type(CBN_Camera *c, const CBN_Camera_Type t) {
  if (!c) return;
  c->type = t;
  carbon_camera__update_proj(c);
}

void carbon_camera_ortho_zoom(CBN_Camera *c, f32 amount) {
  static const f32 max_zoom = 0.1;
  if (!c) return;
  c->ortho_size = carbon_math_max(c->ortho_size + amount, max_zoom);
  if (c->type == CBN_Camera_Type_Orthographic) carbon_camera__update_proj(c);
}

CBN_Vec3 carbon_camera_get_position(const CBN_Camera *c) {
  if (!c) return carbon_math_vec3_1(0);
  return c->position;
}

void carbon_camera_set_position(CBN_Camera *c, CBN_Vec3 p) {
  if (!c) return;
  c->position = p;
  carbon_camera__update_view(c);
}

CBN_Quat carbon_camera_get_rotation(const CBN_Camera *c) {
  if (!c) return carbon_math_quat_id();
  return c->rotation;
}

CBNINL void carbon_camera__update_rotation(CBN_Camera *c) {
  const CBN_Quat q_yaw = carbon_math_quat_from_axis_angle(carbon_math_vec3(0, 1, 0), c->yaw);
  const CBN_Vec3 v_pitch = carbon_math_vec3_rotate(carbon_math_vec3(1, 0, 0), q_yaw);
  const CBN_Quat q_pitch = carbon_math_quat_from_axis_angle(v_pitch, c->pitch);
  c->rotation = carbon_math_quat_mult(q_pitch, q_yaw);
  carbon_camera__update_view(c);
}

void carbon_camera_set_rotation(CBN_Camera *c, CBN_Quat r) {
  if (!c) return;
  CBN_Vec3 fwd = carbon_math_vec3_rotate(carbon_math_vec3(0, 0, -1), r);
  c->yaw = CARBON_FROM_RADIANS(carbon_math_atan2(fwd.x, -fwd.z));
  c->pitch = carbon_math_clamp(CARBON_FROM_RADIANS(carbon_math_asin(fwd.y)),
                               -CARBON_CAMERA__PITCH_MAX, CARBON_CAMERA__PITCH_MAX);
  carbon_camera__update_rotation(c);
}

CBN_Mat4 carbon_camera_get_view(const CBN_Camera *c) {
  if (!c) return carbon_math_mat4_id();
  return c->view;
}

CBN_Mat4 carbon_camera_get_proj(const CBN_Camera *c) {
  if (!c) return carbon_math_mat4_id();
  return c->proj;
}

CBNINL CBN_Vec3 carbon_camera__ndc_to_world_space(CBN_Mat4 VP_inv, CBN_Vec3 ndc) {
  CBN_Vec4 clip = carbon_math_vec4_3(ndc, 1);
  CBN_Vec4 world = carbon_math_mat4_mult_vec4(VP_inv, clip);
  CBN_Vec3 v;
  if (!carbon_math_vec4_project_3d(world, &v)) return carbon_math_vec3_1(0);
  return v;
}

CBN_Ray carbon_camera_screen_to_world(const CBN_Camera *c, const CBN_DrawCanvas *dc, CBN_Vec2 p) {
  CBN_Ray ray = {0};
  if (!c || !dc) return ray;
  p.x = (2*p.x / carbon_drawcanvas_width(dc)) - 1;
  p.y = 1 - (2*p.y / carbon_drawcanvas_height(dc));
  const CBN_Mat4 VP_inv = carbon_math_mat4_inv(carbon_math_mat4_mult(c->proj, c->view));
  switch (c->type) {
  case CBN_Camera_Type_Perspective: {
    CBN_Vec3 near = carbon_camera__ndc_to_world_space(VP_inv, carbon_math_vec3_2(p, -1));
    CBN_Vec3 far = carbon_camera__ndc_to_world_space(VP_inv, carbon_math_vec3_2(p, 1));
    ray.origin = near;
    ray.direction = carbon_math_vec3_norm(carbon_math_vec3_sub(far, near));
  } break;
  case CBN_Camera_Type_Orthographic: {
    ray.origin = carbon_camera__ndc_to_world_space(VP_inv, carbon_math_vec3_2(p, -1));
    ray.direction = carbon_math_vec3_norm(carbon_math_vec3_rotate(carbon_math_vec3(0, 0, -1), c->rotation));
  } break;
  case CBN_Camera_Type_Count:
  default: CARBON_UNREACHABLE;
  }
  return ray;
}

CBNINL CBN_Vec3 carbon_camera__world_space_to_ndc(CBN_Mat4 VP, CBN_Vec3 w) {
  CBN_Vec4 world = carbon_math_vec4_3(w, 1);
  CBN_Vec4 clip = carbon_math_mat4_mult_vec4(VP, world);
  CBN_Vec3 ndc;
  if (!carbon_math_vec4_project_3d(clip, &ndc)) return carbon_math_vec3_1(0);
  return ndc;
}

CBN_Vec2 carbon_camera_world_to_screen(const CBN_Camera *c, const CBN_DrawCanvas *dc, CBN_Vec3 p) {
  if (!c || !dc) return carbon_math_vec2_1(0);
  const CBN_Mat4 VP = carbon_math_mat4_mult(c->proj, c->view);
  const CBN_Vec3 ndc = carbon_camera__world_space_to_ndc(VP, p);
  return carbon_math_vec2((ndc.x + 1) * carbon_drawcanvas_width(dc)/2,
                          (1 - ndc.y) * carbon_drawcanvas_height(dc)/2);
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

void carbon_camera_zoom(CBN_Camera *c, f32 amount) {
  if (!c || !amount) return;
  CBN_Vec3 fwd = carbon_math_vec3_rotate(carbon_math_vec3(0, 0, -1), c->rotation);
  carbon_camera__translate(c, fwd, amount);
}

void carbon_camera_yaw(CBN_Camera *c, f32 amount) {
  if (!c || !amount) return;
  c->yaw += amount;
  carbon_camera__update_rotation(c);
}

void carbon_camera_pitch(CBN_Camera *c, f32 amount) {
  if (!c || !amount) return;
  c->pitch = carbon_math_clamp(c->pitch + amount, -CARBON_CAMERA__PITCH_MAX, CARBON_CAMERA__PITCH_MAX);
  carbon_camera__update_rotation(c);
}
