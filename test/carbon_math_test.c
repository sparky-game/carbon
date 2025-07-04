// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#define TEST(name) CARBON_TEST(carbon_math, name)

TEST(abs) {
  carbon_should_be(77, carbon_math_abs(-77));
  carbon_should_be(77, carbon_math_abs(77));
  carbon_should_be_f(6.9, carbon_math_abs(-6.9));
  carbon_should_be_f(6.9, carbon_math_abs(6.9));
  return CARBON_OK;
}

TEST(round) {
  carbon_should_be_f(-5, carbon_math_round(-4.5));
  carbon_should_be_f(-4, carbon_math_round(-4.4));
  carbon_should_be_f(-1, carbon_math_round(-0.5));
  carbon_should_be_f(0, carbon_math_round(0));
  carbon_should_be_f(1, carbon_math_round(0.5));
  carbon_should_be_f(4, carbon_math_round(4.4));
  carbon_should_be_f(5, carbon_math_round(4.5));
  return CARBON_OK;
}

TEST(floor) {
  carbon_should_be_f(-5, carbon_math_floor(-4.5));
  carbon_should_be_f(-5, carbon_math_floor(-4.4));
  carbon_should_be_f(-1, carbon_math_floor(-0.5));
  carbon_should_be_f(0, carbon_math_floor(0));
  carbon_should_be_f(0, carbon_math_floor(0.5));
  carbon_should_be_f(4, carbon_math_floor(4.4));
  carbon_should_be_f(4, carbon_math_floor(4.5));
  return CARBON_OK;
}

TEST(ceil) {
  carbon_should_be_f(-4, carbon_math_ceil(-4.5));
  carbon_should_be_f(-4, carbon_math_ceil(-4.4));
  carbon_should_be_f(0, carbon_math_ceil(-0.5));
  carbon_should_be_f(0, carbon_math_ceil(0));
  carbon_should_be_f(1, carbon_math_ceil(0.5));
  carbon_should_be_f(5, carbon_math_ceil(4.4));
  carbon_should_be_f(5, carbon_math_ceil(4.5));
  return CARBON_OK;
}

TEST(sqrt) {
  carbon_should_be_f(CARBON_SQRT2, carbon_math_sqrt(2));
  carbon_should_be_f(CARBON_SQRT3, carbon_math_sqrt(3));
  return CARBON_OK;
}

TEST(fmod) {
  carbon_should_be_f(0, carbon_math_fmod(1, 1));
  carbon_should_be_f(0, carbon_math_fmod(2, 1));
  carbon_should_be_f(0, carbon_math_fmod(4, 1));
  carbon_should_be_f(0, carbon_math_fmod(8, 1));
  carbon_should_be_f(1, carbon_math_fmod(1, 2));
  carbon_should_be_f(1, carbon_math_fmod(1, 4));
  carbon_should_be_f(1, carbon_math_fmod(1, 8));
  carbon_should_be_f(1, carbon_math_fmod(1, 8));
  carbon_should_be_f(99, carbon_math_fmod(-1, 100));
  carbon_should_be_f(-1.4, carbon_math_fmod(6.9, -8.3));
  carbon_should_be_f(-0.7, carbon_math_fmod(6.9, -3.8));
  return CARBON_OK;
}

TEST(pow) {
  carbon_should_be_f(0, carbon_math_pow(0, 1));
  carbon_should_be_f(0, carbon_math_pow(0, 69));
  carbon_should_be_f(1, carbon_math_pow(-69, 0));
  carbon_should_be_f(1, carbon_math_pow(-1, 0));
  carbon_should_be_f(1, carbon_math_pow(0, 0));
  carbon_should_be_f(1, carbon_math_pow(1, 0));
  carbon_should_be_f(1, carbon_math_pow(69, 0));
  carbon_should_be_f(16384, carbon_math_pow(4, 7));
  carbon_should_be_f(CARBON_SQRT2, carbon_math_pow(2, 0.5));
  carbon_should_be_f(19.350426, carbon_math_pow(1.456, 7.886));
  return CARBON_OK;
}

TEST(exp2) {
  carbon_should_be_f(0.125, carbon_math_exp2(-3));
  carbon_should_be_f(0.25, carbon_math_exp2(-2));
  carbon_should_be_f(0.5, carbon_math_exp2(-1));
  carbon_should_be_f(1, carbon_math_exp2(0));
  carbon_should_be_f(2, carbon_math_exp2(1));
  carbon_should_be_f(4, carbon_math_exp2(2));
  carbon_should_be_f(8, carbon_math_exp2(3));
  return CARBON_OK;
}

TEST(exp) {
  carbon_should_be_f(1, carbon_math_exp(0));
  carbon_should_be_f(CARBON_E, carbon_math_exp(1));
  carbon_should_be_f(CARBON_E_0_5, carbon_math_exp(0.5));
  carbon_should_be_f(CARBON_E_0_25, carbon_math_exp(0.25));
  carbon_should_be_f(CARBON_E_0_125, carbon_math_exp(0.125));
  return CARBON_OK;
}

TEST(exp10) {
  carbon_should_be_f(0.001, carbon_math_exp10(-3));
  carbon_should_be_f(0.01, carbon_math_exp10(-2));
  carbon_should_be_f(0.1, carbon_math_exp10(-1));
  carbon_should_be_f(1, carbon_math_exp10(0));
  carbon_should_be_f(10, carbon_math_exp10(1));
  carbon_should_be_f(12.589254, carbon_math_exp10(1.1));
  carbon_should_be_f(15.848933, carbon_math_exp10(1.2));
  return CARBON_OK;
}

TEST(frexp) {
  i32 exp;
  carbon_should_be_f(0.625, carbon_math_frexp(2560, &exp));
  carbon_should_be(12, exp);
  carbon_should_be_f(-0.5, carbon_math_frexp(-4, &exp));
  carbon_should_be(3, exp);
  return CARBON_OK;
}

TEST(sigmoid) {
  carbon_should_be_f(0, carbon_math_sigmoid(-14));
  carbon_should_be_f(0.006692, carbon_math_sigmoid(-5));
  carbon_should_be_f(0.047425, carbon_math_sigmoid(-3));
  carbon_should_be_f(0.268941, carbon_math_sigmoid(-1));
  carbon_should_be_f(0.5, carbon_math_sigmoid(0));
  carbon_should_be_f(0.731058, carbon_math_sigmoid(1));
  carbon_should_be_f(0.952574, carbon_math_sigmoid(3));
  carbon_should_be_f(0.993307, carbon_math_sigmoid(5));
  carbon_should_be_f(1, carbon_math_sigmoid(14));
  return CARBON_OK;
}

TEST(tanh) {
  carbon_should_be_f(-1, carbon_math_tanh(-14));
  carbon_should_be_f(-0.995054, carbon_math_tanh(-3));
  carbon_should_be_f(-0.761594, carbon_math_tanh(-1));
  carbon_should_be_f(0, carbon_math_tanh(0));
  carbon_should_be_f(0.761594, carbon_math_tanh(1));
  carbon_should_be_f(0.995054, carbon_math_tanh(3));
  carbon_should_be_f(1, carbon_math_tanh(14));
  return CARBON_OK;
}

TEST(smoothstep) {
  carbon_should_be_f(0, carbon_math_smoothstep(0, 1, 0));
  carbon_should_be_f(0.028, carbon_math_smoothstep(0, 1, 0.1));
  carbon_should_be_f(0.5, carbon_math_smoothstep(0, 1, 0.5));
  carbon_should_be_f(0.784, carbon_math_smoothstep(0, 1, 0.7));
  carbon_should_be_f(1, carbon_math_smoothstep(0, 1, 1));
  return CARBON_OK;
}

TEST(concat) {
  carbon_should_be_u64(1069, carbon_math_concat(10, 69));
  carbon_should_be_u64(20250511, carbon_math_concat(carbon_math_concat(2025*10, 5), 11));
  return CARBON_OK;
}

TEST(asin) {
  carbon_should_be_f(-1.119769, carbon_math_asin(-0.90));
  carbon_should_be_f(-0.848062, carbon_math_asin(-0.75));
  carbon_should_be_f(-0.523599, carbon_math_asin(-0.5));
  carbon_should_be_f(-0.252680, carbon_math_asin(-0.25));
  carbon_should_be_f(0, carbon_math_asin(0));
  carbon_should_be_f(0.252680, carbon_math_asin(0.25));
  carbon_should_be_f(0.523599, carbon_math_asin(0.5));
  carbon_should_be_f(0.848062, carbon_math_asin(0.75));
  carbon_should_be_f(1.119769, carbon_math_asin(0.90));
  return CARBON_OK;
}

TEST(vec2_add) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  CBN_Vec2 r = carbon_math_vec2_add(u, v);
  carbon_should_be_f(4, r.x);
  carbon_should_be_f(6, r.y);
  return CARBON_OK;
}

TEST(vec3_add) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  CBN_Vec3 r = carbon_math_vec3_add(u, v);
  carbon_should_be_f(5, r.x);
  carbon_should_be_f(7, r.y);
  carbon_should_be_f(9, r.z);
  return CARBON_OK;
}

TEST(vec2_sub) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  CBN_Vec2 r = carbon_math_vec2_sub(u, v);
  carbon_should_be_f(-2, r.x);
  carbon_should_be_f(-2, r.y);
  return CARBON_OK;
}

TEST(vec3_sub) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  CBN_Vec3 r = carbon_math_vec3_sub(u, v);
  carbon_should_be_f(-3, r.x);
  carbon_should_be_f(-3, r.y);
  carbon_should_be_f(-3, r.z);
  return CARBON_OK;
}

TEST(vec2_dot) {
  CBN_Vec2 u = {{1, 2}};
  CBN_Vec2 v = {{3, 4}};
  carbon_should_be_f(11, carbon_math_vec2_dot(u, v));
  return CARBON_OK;
}

TEST(vec3_dot) {
  CBN_Vec3 u = {{1, 2, 3}};
  CBN_Vec3 v = {{4, 5, 6}};
  carbon_should_be_f(32, carbon_math_vec3_dot(u, v));
  return CARBON_OK;
}

TEST(vec3_cross) {
  CBN_Vec3 u = {{3, -3, 1}};
  CBN_Vec3 v = {{4, 9, 2}};
  CBN_Vec3 r = carbon_math_vec3_cross(u, v);
  carbon_should_be_f(-15, r.x);
  carbon_should_be_f(-2, r.y);
  carbon_should_be_f(39, r.z);
  return CARBON_OK;
}

TEST(rect_contains_point) {
  CBN_Rect r = {0, 0, 10, 5};
  CBN_Vec2 p1 = {{8, 3}};
  CBN_Vec2 p2 = {{3, 8}};
  carbon_should_be_true(carbon_math_rect_contains_point(r, p1));
  carbon_should_be_false(carbon_math_rect_contains_point(r, p2));
  return CARBON_OK;
}

TEST(rect_detect_collision) {
  CBN_Rect r1 = {0, 5, 5, 5}, r2 = {4, 11, 5, 5};
  carbon_should_be_false(carbon_math_rect_detect_collision(r1, r2));
  carbon_should_be_false(carbon_math_rect_detect_collision(r2, r1));
  CBN_Rect r3 = {0, 5, 5, 5}, r4 = {4, 10, 5, 5};
  carbon_should_be_true(carbon_math_rect_detect_collision(r3, r4));
  carbon_should_be_true(carbon_math_rect_detect_collision(r4, r3));
  CBN_Rect r5 = {0, 5, 5, 5}, r6 = {4, 9, 5, 5};
  carbon_should_be_true(carbon_math_rect_detect_collision(r5, r6));
  carbon_should_be_true(carbon_math_rect_detect_collision(r6, r5));
  return CARBON_OK;
}

#undef TEST
