// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_math_cxx, name)

TEST(vec2_add) {
  auto u = cbn::math::Vec2(1, 2);
  auto v = cbn::math::Vec2(3, 4);
  auto r = u + v;
  carbon_should_be_f(4, r.x);
  carbon_should_be_f(6, r.y);
  return CARBON_OK;
}

TEST(vec3_add) {
  auto u = CARBON_VEC3(1, 2, 3);
  auto v = CARBON_VEC3(4, 5, 6);
  auto r = u + v;
  carbon_should_be_f(5, r.x);
  carbon_should_be_f(7, r.y);
  carbon_should_be_f(9, r.z);
  return CARBON_OK;
}

TEST(vec2_sub) {
  auto u = cbn::math::Vec2(1, 2);
  auto v = cbn::math::Vec2(3, 4);
  auto r = u - v;
  carbon_should_be_f(-2, r.x);
  carbon_should_be_f(-2, r.y);
  return CARBON_OK;
}

TEST(vec3_sub) {
  auto u = CARBON_VEC3(1, 2, 3);
  auto v = CARBON_VEC3(4, 5, 6);
  auto r = u - v;
  carbon_should_be_f(-3, r.x);
  carbon_should_be_f(-3, r.y);
  carbon_should_be_f(-3, r.z);
  return CARBON_OK;
}

TEST(vec2_dot) {
  auto u = cbn::math::Vec2(1, 2);
  auto v = cbn::math::Vec2(3, 4);
  carbon_should_be_f(11, u.Dot(v));
  return CARBON_OK;
}

TEST(vec3_dot) {
  auto u = CARBON_VEC3(1, 2, 3);
  auto v = CARBON_VEC3(4, 5, 6);
  carbon_should_be_f(32, u * v);
  return CARBON_OK;
}

#undef TEST
