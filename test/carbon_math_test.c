// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_math, abs) {
  carbon_should_be(77, carbon_math_abs(-77));
  carbon_should_be(77, carbon_math_abs(77));
  carbon_should_be_f(6.9, carbon_math_abs(-6.9));
  carbon_should_be_f(6.9, carbon_math_abs(6.9));
  return CARBON_OK;
}

CARBON_TEST(carbon_math, exp) {
  // CARBON_INFO("e^0 :: %.2f", carbon_math_exp(0));
  // CARBON_INFO("e^1 :: %.2f", carbon_math_exp(1));
  // CARBON_INFO("e^2 :: %.2f", carbon_math_exp(2));
  return CARBON_KO;
}

CARBON_TEST(carbon_math, sigmoid) {
  carbon_should_be_f(0, carbon_math_sigmoid(-7));
  carbon_should_be_f(0.006692, carbon_math_sigmoid(-5));
  carbon_should_be_f(0.047425, carbon_math_sigmoid(-3));
  carbon_should_be_f(0.268941, carbon_math_sigmoid(-1));
  carbon_should_be_f(0.5, carbon_math_sigmoid(0));
  carbon_should_be_f(0.731058, carbon_math_sigmoid(1));
  carbon_should_be_f(0.952574, carbon_math_sigmoid(3));
  carbon_should_be_f(0.993307, carbon_math_sigmoid(5));
  carbon_should_be_f(1, carbon_math_sigmoid(7));
  return CARBON_OK;
}

CARBON_TEST(carbon_math, tanh) {
  return CARBON_KO;
}

CARBON_TEST(carbon_math, smoothstep) {
  return CARBON_KO;
}

CARBON_TEST(carbon_math, vec2_add) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  CBN_Vec2 r = carbon_math_vec2_add(u, v);
  carbon_should_be_f(4, r.x);
  carbon_should_be_f(6, r.y);
  return CARBON_OK;
}

CARBON_TEST(carbon_math, vec3_add) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  CBN_Vec3 r = carbon_math_vec3_add(u, v);
  carbon_should_be_f(5, r.x);
  carbon_should_be_f(7, r.y);
  carbon_should_be_f(9, r.z);
  return CARBON_OK;
}

CARBON_TEST(carbon_math, vec2_sub) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  CBN_Vec2 r = carbon_math_vec2_sub(u, v);
  carbon_should_be_f(-2, r.x);
  carbon_should_be_f(-2, r.y);
  return CARBON_OK;
}

CARBON_TEST(carbon_math, vec3_sub) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  CBN_Vec3 r = carbon_math_vec3_sub(u, v);
  carbon_should_be_f(-3, r.x);
  carbon_should_be_f(-3, r.y);
  carbon_should_be_f(-3, r.z);
  return CARBON_OK;
}

CARBON_TEST(carbon_math, vec2_dot) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  carbon_should_be_f(11, carbon_math_vec2_dot(u, v));
  return CARBON_OK;
}

CARBON_TEST(carbon_math, vec3_dot) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  carbon_should_be_f(32, carbon_math_vec3_dot(u, v));
  return CARBON_OK;
}
