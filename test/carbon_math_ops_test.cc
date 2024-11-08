// SPDX-License-Identifier: BSD-3-Clause OR GPL-3.0-only
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

CARBON_TEST(carbon_math_ops, vec3_add) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  CBN_Vec3 r = u + v;
  carbon_should_be_f(5, r.x);
  carbon_should_be_f(7, r.y);
  carbon_should_be_f(9, r.z);
  return CARBON_OK;
}

CARBON_TEST(carbon_math_ops, vec2_sub) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  CBN_Vec2 r = u - v;
  carbon_should_be_f(-2, r.x);
  carbon_should_be_f(-2, r.y);
  return CARBON_OK;
}

CARBON_TEST(carbon_math_ops, vec3_sub) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  CBN_Vec3 r = u - v;
  carbon_should_be_f(-3, r.x);
  carbon_should_be_f(-3, r.y);
  carbon_should_be_f(-3, r.z);
  return CARBON_OK;
}

CARBON_TEST(carbon_math_ops, vec2_dot) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  carbon_should_be_f(11, u * v);
  return CARBON_OK;
}

CARBON_TEST(carbon_math_ops, vec3_dot) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  carbon_should_be_f(32, u * v);
  return CARBON_OK;
}
