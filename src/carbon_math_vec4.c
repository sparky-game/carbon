// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

char *carbon_math_vec4_to_cstr(CBN_Vec4 v) {
  return carbon_string_fmt("(%.3f, %.3f, %.3f, %.3f)", v.x, v.y, v.z, v.w);
}

CBN_Vec4 carbon_math_vec4_lerp(CBN_Vec4 u, CBN_Vec4 v, f32 t) {
  return CARBON_VEC4(CARBON_LERP(u.x, v.x, t),
                     CARBON_LERP(u.y, v.y, t),
                     CARBON_LERP(u.z, v.z, t),
                     CARBON_LERP(u.w, v.w, t));
}

bool carbon_math_vec4_project_3d(CBN_Vec4 v, CBN_Vec3 *out_v) {
  if (!out_v || !v.w) return false;
  *out_v = CARBON_VEC3(v.x/v.w, v.y/v.w, v.z/v.w);
  return true;
}
