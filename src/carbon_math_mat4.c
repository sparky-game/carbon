// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Mat4 carbon_math_mat4_zero(void) {
  return (CBN_Mat4) {
    .m = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  };
}

CBN_Mat4 carbon_math_mat4_id(void) {
  return (CBN_Mat4) {
    .m = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };
}

CBN_Mat4 carbon_math_mat4_mult(CBN_Mat4 a, CBN_Mat4 b) {
  CBN_Mat4 m = carbon_math_mat4_zero();
  for (usz i = 0; i < 4; ++i) {
    for (usz k = 0; k < 4; ++k) {
      for (usz j = 0; j < 4; ++j) {
        m.m[i][j] += a.m[i][k] * b.m[k][j];
      }
    }
  }
  return m;
}

CBN_Vec3 carbon_math_mat4_mult_vec3(CBN_Mat4 m, CBN_Vec3 v) {
  return carbon_math_mat4_mult_vec4(m, carbon_math_vec4_3(v, 1)).xyz;
}

CBN_Vec4 carbon_math_mat4_mult_vec4(CBN_Mat4 m, CBN_Vec4 v) {
  return (CBN_Vec4) {
    .x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3],
    .y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3],
    .z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3],
    .w = v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3]
  };
}

CBN_Mat4 carbon_math_mat4_from_quat(CBN_Quat q) {
  CBN_Mat4 m = carbon_math_mat4_id();
  CBN_Quat n = carbon_math_quat_norm(q);
  m.m[0][0] = 1 - 2 * (n.y * n.y + n.z * n.z);
  m.m[0][1] = 2 * (n.x * n.y - n.z * n.w);
  m.m[0][2] = 2 * (n.x * n.z + n.y * n.w);
  m.m[1][0] = 2 * (n.x * n.y + n.z * n.w);
  m.m[1][1] = 1 - 2 * (n.x * n.x + n.z * n.z);
  m.m[1][2] = 2 * (n.y * n.z - n.x * n.w);
  m.m[2][0] = 2 * (n.x * n.z - n.y * n.w);
  m.m[2][1] = 2 * (n.y * n.z + n.x * n.w);
  m.m[2][2] = 1 - 2 * (n.x * n.x + n.y * n.y);
  return m;
}

CBN_Mat4 carbon_math_mat4_translation(CBN_Vec3 position) {
  CBN_Mat4 m = carbon_math_mat4_id();
  m.m[0][3] = position.x;
  m.m[1][3] = position.y;
  m.m[2][3] = position.z;
  return m;
}

CBN_Mat4 carbon_math_mat4_scale(CBN_Vec3 scale) {
  CBN_Mat4 m = carbon_math_mat4_id();
  m.m[0][0] = scale.x;
  m.m[1][1] = scale.y;
  m.m[2][2] = scale.z;
  return m;
}

CBN_Mat4 carbon_math_mat4_model(CBN_Vec3 t, CBN_Quat r, CBN_Vec3 s) {
  CBN_Mat4 tm = carbon_math_mat4_translation(t);
  CBN_Mat4 rm = carbon_math_mat4_from_quat(r);
  CBN_Mat4 sm = carbon_math_mat4_scale(s);
  return carbon_math_mat4_mult(tm, carbon_math_mat4_mult(rm, sm));
}

CBN_Mat4 carbon_math_mat4_view(CBN_Vec3 position, CBN_Quat rotation) {
  CBN_Mat4 r = carbon_math_mat4_from_quat(carbon_math_quat_inv(rotation));
  CBN_Mat4 t = carbon_math_mat4_translation(carbon_math_vec3_neg(position));
  return carbon_math_mat4_mult(r, t);
}

CBN_Mat4 carbon_math_mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far) {
  f32 f = 1 / carbon_math_tan(CARBON_TO_RADIANS(fov) / 2);
  CBN_Mat4 m = carbon_math_mat4_zero();
  m.m[0][0] = f / aspect;
  m.m[1][1] = f;
  m.m[2][2] = (near + far) / (near - far);
  m.m[2][3] = (2 * near * far) / (near - far);
  m.m[3][2] = -1;
  return m;
}

CBN_Mat4 carbon_math_mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
  CBN_Mat4 m = carbon_math_mat4_zero();
  m.m[0][0] =  2 / (right - left);
  m.m[1][1] =  2 / (top - bottom);
  m.m[2][2] = -2 / (far - near);
  m.m[3][0] = - (right + left) / (right - left);
  m.m[3][1] = - (top + bottom) / (top - bottom);
  m.m[3][2] = - (far + near)   / (far - near);
  m.m[3][3] = 1;
  return m;
}
