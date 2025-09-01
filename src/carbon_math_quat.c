// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Quat carbon_math_quat_mult(CBN_Quat p, CBN_Quat q) {
  return (CBN_Quat) {
    .x =  p.x * q.w + p.y * q.z - p.z * q.y + p.w * q.x,
    .y = -p.x * q.z + p.y * q.w + p.z * q.x + p.w * q.y,
    .z =  p.x * q.y - p.y * q.x + p.z * q.w + p.w * q.z,
    .w = -p.x * q.x - p.y * q.y - p.z * q.z + p.w * q.w
  };
}

f32 carbon_math_quat_dot(CBN_Quat p, CBN_Quat q) {
  return p.x * q.x + p.y * q.y + p.z * q.z + p.w * q.w;
}

CBN_Quat carbon_math_quat_from_euler(CBN_Vec3 v) {
  const f32 r = CARBON_TO_RADIANS(0.5);
  f32 hx = r * v.x, hy = r * v.y, hz = r * v.z;
  f32 sx = carbon_math_sin(hx), sy = carbon_math_sin(hy), sz = carbon_math_sin(hz);
  f32 cx = carbon_math_cos(hx), cy = carbon_math_cos(hy), cz = carbon_math_cos(hz);
  CBN_Quat qx = {{sx, 0, 0, cx}}, qy = {{0, sy, 0, cy}}, qz = {{0, 0, sz, cz}};
  return carbon_math_quat_mult(qz, carbon_math_quat_mult(qy, qx));
}
