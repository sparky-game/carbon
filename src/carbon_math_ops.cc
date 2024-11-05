// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_Vec2 operator+(const CBN_Vec2 &u, const CBN_Vec2 &v) {
  return carbon_math_vec2_add(u, v);
}

#endif  // __cplusplus
