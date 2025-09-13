// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Mat4 carbon_math_mat4_zero(void) {
  return (CBN_Mat4) {
    .items = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  };
}

CBN_Mat4 carbon_math_mat4_id(void) {
  return (CBN_Mat4) {
    .items = {
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
        m.items[i][j] += a.items[i][k] * b.items[k][j];
      }
    }
  }
  return m;
}

CBN_Vec3 carbon_math_mat4_mult_vec3(CBN_Mat4 m, CBN_Vec3 v) {
  return CARBON_VEC3_V(carbon_math_mat4_mult_vec4(m, CARBON_VEC4_3(v, 1)));
}

CBN_Vec4 carbon_math_mat4_mult_vec4(CBN_Mat4 m, CBN_Vec4 v) {
  return (CBN_Vec4) {
    .x = v.x * m.items[0][0] + v.y * m.items[0][1] + v.z * m.items[0][2] + v.w * m.items[0][3],
    .y = v.x * m.items[1][0] + v.y * m.items[1][1] + v.z * m.items[1][2] + v.w * m.items[1][3],
    .z = v.x * m.items[2][0] + v.y * m.items[2][1] + v.z * m.items[2][2] + v.w * m.items[2][3],
    .w = v.x * m.items[3][0] + v.y * m.items[3][1] + v.z * m.items[3][2] + v.w * m.items[3][3]
  };
}

CBN_Mat4 carbon_math_mat4_from_quat(CBN_Quat q) {
  CBN_Mat4 m = carbon_math_mat4_id();
  CBN_Quat n = carbon_math_quat_norm(q);
  m.items[0][0] = 1 - 2 * (n.y * n.y + n.z * n.z);
  m.items[0][1] = 2 * (n.x * n.y - n.z * n.w);
  m.items[0][2] = 2 * (n.x * n.z + n.y * n.w);
  m.items[1][0] = 2 * (n.x * n.y + n.z * n.w);
  m.items[1][1] = 1 - 2 * (n.x * n.x + n.z * n.z);
  m.items[1][2] = 2 * (n.y * n.z - n.x * n.w);
  m.items[2][0] = 2 * (n.x * n.z - n.y * n.w);
  m.items[2][1] = 2 * (n.y * n.z + n.x * n.w);
  m.items[2][2] = 1 - 2 * (n.x * n.x + n.y * n.y);
  return m;
}

CBN_Mat4 carbon_math_mat4_translation(CBN_Vec3 position) {
  CBN_Mat4 m = carbon_math_mat4_id();
  m.items[0][3] = position.x;
  m.items[1][3] = position.y;
  m.items[2][3] = position.z;
  return m;
}

CBN_Mat4 carbon_math_mat4_scale(CBN_Vec3 scale) {
  CBN_Mat4 m = carbon_math_mat4_id();
  m.items[0][0] = scale.x;
  m.items[1][1] = scale.y;
  m.items[2][2] = scale.z;
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
  CBN_Mat4 t = carbon_math_mat4_translation(CARBON_VEC3_N(position));
  return carbon_math_mat4_mult(r, t);
}

CBN_Mat4 carbon_math_mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far) {
  f32 f = 1 / carbon_math_tan(CARBON_TO_RADIANS(fov) / 2);
  CBN_Mat4 m = carbon_math_mat4_zero();
  m.items[0][0] = f / aspect;
  m.items[1][1] = f;
  m.items[2][2] = (near + far) / (near - far);
  m.items[2][3] = (2 * near * far) / (near - far);
  m.items[3][2] = -1;
  return m;
}
