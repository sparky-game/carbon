// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

char *carbon_math_vec4_to_cstr(CBN_Vec4 v) {
  return carbon_string_fmt("(%.3f, %.3f, %.3f, %.3f)", v.x, v.y, v.z, v.w);
}

u8 carbon_math_vec4_project_3d(CBN_Vec4 v, CBN_Vec3 *out_v) {
  if (!out_v || !v.w) return false;
  *out_v = CARBON_VEC3(v.x/v.w, v.y/v.w, v.z/v.w);
  return true;
}
