// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#undef TEST
#define TEST(x) CARBON_TEST(carbon_math, x)

TEST(abs) {
  carbon_should_be(77, cbn::math::Abs(-77));
  carbon_should_be(77, cbn::math::Abs(77));
  carbon_should_be_f(6.9, cbn::math::Abs(-6.9));
  carbon_should_be_f(6.9, cbn::math::Abs(6.9));
  return true;
}

TEST(round) {
  carbon_should_be_f(-5, cbn::math::Round(-4.5));
  carbon_should_be_f(-4, cbn::math::Round(-4.4));
  carbon_should_be_f(-1, cbn::math::Round(-0.5));
  carbon_should_be_f(0, cbn::math::Round(0));
  carbon_should_be_f(1, cbn::math::Round(0.5));
  carbon_should_be_f(4, cbn::math::Round(4.4));
  carbon_should_be_f(5, cbn::math::Round(4.5));
  return true;
}

TEST(floor) {
  carbon_should_be_f(-5, cbn::math::Floor(-4.5));
  carbon_should_be_f(-5, cbn::math::Floor(-4.4));
  carbon_should_be_f(-1, cbn::math::Floor(-0.5));
  carbon_should_be_f(0, cbn::math::Floor(0));
  carbon_should_be_f(0, cbn::math::Floor(0.5));
  carbon_should_be_f(4, cbn::math::Floor(4.4));
  carbon_should_be_f(4, cbn::math::Floor(4.5));
  return true;
}

TEST(ceil) {
  carbon_should_be_f(-4, cbn::math::Ceil(-4.5));
  carbon_should_be_f(-4, cbn::math::Ceil(-4.4));
  carbon_should_be_f(0, cbn::math::Ceil(-0.5));
  carbon_should_be_f(0, cbn::math::Ceil(0));
  carbon_should_be_f(1, cbn::math::Ceil(0.5));
  carbon_should_be_f(5, cbn::math::Ceil(4.4));
  carbon_should_be_f(5, cbn::math::Ceil(4.5));
  return true;
}

TEST(sqrt) {
  carbon_should_be_f(0, cbn::math::Sqrt(0));
  carbon_should_be_f(1, cbn::math::Sqrt(1));
  carbon_should_be_f(CARBON_SQRT2, cbn::math::Sqrt(2));
  carbon_should_be_f(CARBON_SQRT3, cbn::math::Sqrt(3));
  carbon_should_be_f(8.306623, cbn::math::Sqrt(69));
  carbon_should_be_f(27.876934, cbn::math::Sqrt(777.1234));
  carbon_should_be_f(0.5, cbn::math::Sqrt(0.25));
  carbon_should_be_f(CARBON_SQRT3_2, cbn::math::Sqrt(0.75));
  carbon_should_be_f(CARBON_1_SQRT2, cbn::math::Sqrt(0.5));
  carbon_should_be_f(0.1, cbn::math::Sqrt(1e-2));
  carbon_should_be_f(0.031622, cbn::math::Sqrt(1e-3));
  carbon_should_be_f(1e-3, cbn::math::Sqrt(1e-6));
  carbon_should_be_f(1e-5, cbn::math::Sqrt(1e-10));
  return true;
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
  return true;
}

TEST(pow) {
  carbon_should_be_f(0, cbn::math::Pow(0, 1));
  carbon_should_be_f(0, cbn::math::Pow(0, 69));
  carbon_should_be_f(1, cbn::math::Pow(-69, 0));
  carbon_should_be_f(1, cbn::math::Pow(-1, 0));
  carbon_should_be_f(1, cbn::math::Pow(0, 0));
  carbon_should_be_f(1, cbn::math::Pow(1, 0));
  carbon_should_be_f(1, cbn::math::Pow(69, 0));
  carbon_should_be_f(16384, cbn::math::Pow(4, 7));
  carbon_should_be_f(CARBON_SQRT2, cbn::math::Pow(2, 0.5));
  carbon_should_be_f(13.797444, cbn::math::Pow(1.4, 7.8));
  return true;
}

TEST(exp2) {
  carbon_should_be_f(0.125, cbn::math::Exp2(-3));
  carbon_should_be_f(0.25, cbn::math::Exp2(-2));
  carbon_should_be_f(0.5, cbn::math::Exp2(-1));
  carbon_should_be_f(1, cbn::math::Exp2(0));
  carbon_should_be_f(2, cbn::math::Exp2(1));
  carbon_should_be_f(4, cbn::math::Exp2(2));
  carbon_should_be_f(8, cbn::math::Exp2(3));
  return true;
}

TEST(exp) {
  carbon_should_be_f(1, cbn::math::Exp(0));
  carbon_should_be_f(CARBON_E, cbn::math::Exp(1));
  carbon_should_be_f(CARBON_E_0_5, cbn::math::Exp(0.5));
  carbon_should_be_f(CARBON_E_0_25, cbn::math::Exp(0.25));
  carbon_should_be_f(CARBON_E_0_125, cbn::math::Exp(0.125));
  return true;
}

TEST(exp10) {
  carbon_should_be_f(0.001, cbn::math::Exp10(-3));
  carbon_should_be_f(0.01, cbn::math::Exp10(-2));
  carbon_should_be_f(0.1, cbn::math::Exp10(-1));
  carbon_should_be_f(1, cbn::math::Exp10(0));
  carbon_should_be_f(10, cbn::math::Exp10(1));
  carbon_should_be_f(12.589254, cbn::math::Exp10(1.1));
  carbon_should_be_f(15.848933, cbn::math::Exp10(1.2));
  return true;
}

TEST(frexp) {
  i32 exp;
  carbon_should_be_f(0.625, cbn::math::Frexp(2560, &exp));
  carbon_should_be(12, exp);
  carbon_should_be_f(-0.5, cbn::math::Frexp(-4, &exp));
  carbon_should_be(3, exp);
  return true;
}

TEST(sigmoid) {
  carbon_should_be_f(0, cbn::math::Sigmoid(-14));
  carbon_should_be_f(0.006692, cbn::math::Sigmoid(-5));
  carbon_should_be_f(0.047425, cbn::math::Sigmoid(-3));
  carbon_should_be_f(0.268941, cbn::math::Sigmoid(-1));
  carbon_should_be_f(0.5, cbn::math::Sigmoid(0));
  carbon_should_be_f(0.731058, cbn::math::Sigmoid(1));
  carbon_should_be_f(0.952574, cbn::math::Sigmoid(3));
  carbon_should_be_f(0.993307, cbn::math::Sigmoid(5));
  carbon_should_be_f(1, cbn::math::Sigmoid(14));
  return true;
}

TEST(tanh) {
  carbon_should_be_f(-1, cbn::math::Tanh(-14));
  carbon_should_be_f(-0.995054, cbn::math::Tanh(-3));
  carbon_should_be_f(-0.761594, cbn::math::Tanh(-1));
  carbon_should_be_f(0, cbn::math::Tanh(0));
  carbon_should_be_f(0.761594, cbn::math::Tanh(1));
  carbon_should_be_f(0.995054, cbn::math::Tanh(3));
  carbon_should_be_f(1, cbn::math::Tanh(14));
  return true;
}

TEST(smoothstep) {
  carbon_should_be_f(0, cbn::math::SmoothStep(0, 1, 0));
  carbon_should_be_f(0.028, cbn::math::SmoothStep(0, 1, 0.1));
  carbon_should_be_f(0.5, cbn::math::SmoothStep(0, 1, 0.5));
  carbon_should_be_f(0.784, cbn::math::SmoothStep(0, 1, 0.7));
  carbon_should_be_f(1, cbn::math::SmoothStep(0, 1, 1));
  return true;
}

TEST(concat) {
  carbon_should_be_u64(1069, cbn::math::Concat(10, 69));
  carbon_should_be_u64(20250511, cbn::math::Concat(cbn::math::Concat(2025*10, 5), 11));
  return true;
}

TEST(egcd) {
  carbon_should_be(60, cbn::math::eGCD(960, 540));
  carbon_should_be(80, cbn::math::eGCD(1280, 720));
  carbon_should_be(100, cbn::math::eGCD(1600, 900));
  carbon_should_be(120, cbn::math::eGCD(1920, 1080));
  return true;
}

TEST(sin) {
  carbon_should_be_f(0, cbn::math::Sin(0));
  carbon_should_be_f(0.5, cbn::math::Sin(CARBON_PI_6));
  carbon_should_be_f(CARBON_1_SQRT2, cbn::math::Sin(CARBON_PI_4));
  carbon_should_be_f(CARBON_SQRT3_2, cbn::math::Sin(CARBON_PI_3));
  carbon_should_be_f(1, cbn::math::Sin(CARBON_PI_2));
  return true;
}

TEST(cos) {
  carbon_should_be_f(1, cbn::math::Cos(0));
  carbon_should_be_f(CARBON_SQRT3_2, cbn::math::Cos(CARBON_PI_6));
  carbon_should_be_f(CARBON_1_SQRT2, cbn::math::Cos(CARBON_PI_4));
  carbon_should_be_f(0.5, cbn::math::Cos(CARBON_PI_3));
  carbon_should_be_f(0, cbn::math::Cos(CARBON_PI_2));
  return true;
}

TEST(tan) {
  carbon_should_be_f(0, cbn::math::Tan(0));
  carbon_should_be_f(CARBON_1_SQRT3, cbn::math::Tan(CARBON_PI_6));
  carbon_should_be_f(1, cbn::math::Tan(CARBON_PI_4));
  carbon_should_be_f(CARBON_SQRT3, cbn::math::Tan(CARBON_PI_3));
  return true;
}

TEST(asin) {
  carbon_should_be_f(-1.119769, cbn::math::Asin(-0.90));
  carbon_should_be_f(-0.848062, cbn::math::Asin(-0.75));
  carbon_should_be_f(-0.523599, cbn::math::Asin(-0.5));
  carbon_should_be_f(-0.252680, cbn::math::Asin(-0.25));
  carbon_should_be_f(0, cbn::math::Asin(0));
  carbon_should_be_f(0.252680, cbn::math::Asin(0.25));
  carbon_should_be_f(0.523599, cbn::math::Asin(0.5));
  carbon_should_be_f(0.848062, cbn::math::Asin(0.75));
  carbon_should_be_f(1.119769, cbn::math::Asin(0.90));
  return true;
}

TEST(vec2_add) {
  cbn::math::Vec2 u {1, 2};
  cbn::math::Vec2 v {3, 4};
  auto r = u + v;
  carbon_should_be_f(4, r.x);
  carbon_should_be_f(6, r.y);
  return true;
}

TEST(vec3_add) {
  cbn::math::Vec3 u {1, 2, 3};
  cbn::math::Vec3 v {4, 5, 6};
  auto r = u + v;
  carbon_should_be_f(5, r.x);
  carbon_should_be_f(7, r.y);
  carbon_should_be_f(9, r.z);
  return true;
}

TEST(vec2_sub) {
  cbn::math::Vec2 u {1, 2};
  cbn::math::Vec2 v {3, 4};
  auto r = u - v;
  carbon_should_be_f(-2, r.x);
  carbon_should_be_f(-2, r.y);
  return true;
}

TEST(vec3_sub) {
  cbn::math::Vec3 u {1, 2, 3};
  cbn::math::Vec3 v {4, 5, 6};
  auto r = u - v;
  carbon_should_be_f(-3, r.x);
  carbon_should_be_f(-3, r.y);
  carbon_should_be_f(-3, r.z);
  return true;
}

TEST(vec2_dot) {
  cbn::math::Vec2 u {1, 2};
  cbn::math::Vec2 v {3, 4};
  carbon_should_be_f(11, u.Dot(v));
  return true;
}

TEST(vec3_dot) {
  cbn::math::Vec3 u {1, 2, 3};
  cbn::math::Vec3 v {4, 5, 6};
  carbon_should_be_f(32, u * v);
  return true;
}

TEST(vec3_cross) {
  cbn::math::Vec3 u {3, -3, 1};
  cbn::math::Vec3 v {4, 9, 2};
  auto r = u.Cross(v);
  carbon_should_be_f(-15, r.x);
  carbon_should_be_f(-2, r.y);
  carbon_should_be_f(39, r.z);
  return true;
}

TEST(rect_contains) {
  cbn::math::Rect r {0, 0, 10, 5};
  cbn::math::Vec2 p1 {8, 3};
  cbn::math::Vec2 p2 {3, 8};  // p2 = p1.yx;
  carbon_should_be_true(r.Contains(p1));
  carbon_should_be_false(r.Contains(p2));
  return true;
}

TEST(rect_overlaps) {
  cbn::math::Rect r1 {0, 5, 5}, r2 {4, 11, 5};
  carbon_should_be_false(r1.Overlaps(r2));
  carbon_should_be_false(r2.Overlaps(r1));
  --r2.y;
  carbon_should_be_true(r1.Overlaps(r2));
  carbon_should_be_true(r2.Overlaps(r1));
  --r2.y;
  carbon_should_be_true(r1.Overlaps(r2));
  carbon_should_be_true(r2.Overlaps(r1));
  return true;
}
