// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_Vec4 CBN_Vec4::Lerp(const CBN_Vec4 &v, f32 t) const {
  return carbon_math_vec4_lerp(*this, v, t);
}

const char *CBN_Vec4::ToString(void) const {
  return carbon_math_vec4_to_cstr(*this);
}
