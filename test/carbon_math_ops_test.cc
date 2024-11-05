// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_math_ops, vec2_add) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  CBN_Vec2 r = u + v;
  carbon_should_be_f(4, r.x);
  carbon_should_be_f(6, r.y);
  return CARBON_OK;
}
