// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_Vec2 CBN_Vec2::Rotate(f32 angle) const {
  return carbon_math_vec2_rotate(*this, angle);
}

const char *CBN_Vec2::ToString(void) const {
  return carbon_math_vec2_to_cstr(*this);
}

#endif  // __cplusplus
