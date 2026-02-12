// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

CBN_Quat carbon_math_quat_id(void) {
  return carbon_math_quat(0, 0, 0, 1);
}

CBN_Quat carbon_math_quat_conj(CBN_Quat q) {
  return carbon_math_quat(-q.x, -q.y, -q.z, q.w);
}

CBN_Quat carbon_math_quat_mult(CBN_Quat p, CBN_Quat q) {
  return carbon_math_quat(p.x * q.w + p.y * q.z - p.z * q.y + p.w * q.x,
                          -p.x * q.z + p.y * q.w + p.z * q.x + p.w * q.y,
                          p.x * q.y - p.y * q.x + p.z * q.w + p.w * q.z,
                          -p.x * q.x - p.y * q.y - p.z * q.z + p.w * q.w);
}

CBN_Quat carbon_math_quat_from_euler(CBN_Vec3 v) {
  f32 hx = CARBON_TO_RADIANS(v.x)/2, hy = CARBON_TO_RADIANS(v.y)/2, hz = CARBON_TO_RADIANS(v.z)/2;
  f32 sx = carbon_math_sin(hx), sy = carbon_math_sin(hy), sz = carbon_math_sin(hz);
  f32 cx = carbon_math_cos(hx), cy = carbon_math_cos(hy), cz = carbon_math_cos(hz);
  CBN_Quat qx = carbon_math_quat(sx, 0, 0, cx), qy = carbon_math_quat(0, sy, 0, cy), qz = carbon_math_quat(0, 0, sz, cz);
  return carbon_math_quat_mult(qz, carbon_math_quat_mult(qy, qx));
}

CBN_Quat carbon_math_quat_from_axis_angle(CBN_Vec3 axis, f32 angle) {
  const f32 ha = CARBON_TO_RADIANS(angle)/2;
  f32 s = carbon_math_sin(ha), c = carbon_math_cos(ha);
  return carbon_math_quat(s * axis.x, s * axis.y, s * axis.z, c);
}

f32 carbon_math_quat_len_squared(CBN_Quat q) {
  return carbon_math_quat_dot(q, q);
}

f32 carbon_math_quat_len(CBN_Quat q) {
  return carbon_math_sqrt(carbon_math_quat_len_squared(q));
}

CBN_Quat carbon_math_quat_norm(CBN_Quat q) {
  f32 sqlen = carbon_math_quat_len_squared(q);
  if (sqlen <= CARBON_EPS_SQ) return q;
  return carbon_math_quat_scale(q, carbon_math_rsqrt(sqlen));
}

CBN_Quat carbon_math_quat_inv(CBN_Quat q) {
  return carbon_math_quat_norm(carbon_math_quat_conj(q));
}
