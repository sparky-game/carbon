// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

static constexpr auto l = 2;
static constexpr auto g = 9.81;
static constexpr auto tau = 1/50.;
static constexpr auto pixels_per_meter = 140;

cbn::Pair<f64> update(f64 dt) {
  using namespace cbn::math;
  static auto t = 0.0;
  static auto theta1 = pi/2;
  static auto theta2 = pi/2;
  static auto omega1 = 0.0;
  static auto omega2 = 0.0;
  t += dt;
  while (t >= tau) {
    {// Physics
      auto f = [](f64 t1, f64 t2, f64 w1, f64 w2) -> cbn::Pair<f64> {
        auto delta = t1 - t2;
        auto d = l * (3 - Cos(2*delta));
        auto a1 = (-3*g*Sin(t1) - g*Sin(t1 - 2*t2) - 2*l*Sin(delta)*(w2*w2 + w1*w1*Cos(delta)))/d;
        auto a2 = (2*Sin(delta)*(2*l*w1*w1 + 2*g*Cos(t1) + l*w2*w2*Cos(delta)))/d;
        return {a1, a2};
      };
      auto k1_t1 = omega1;
      auto k1_t2 = omega2;
      auto [k1_w1, k1_w2] = f(theta1,
                              theta2,
                              omega1,
                              omega2);
      auto k2_t1 = omega1 + k1_w1*(tau/2);
      auto k2_t2 = omega2 + k1_w2*(tau/2);
      auto [k2_w1, k2_w2] = f(theta1 + k1_t1*(tau/2),
                              theta2 + k1_t2*(tau/2),
                              omega1 + k1_w1*(tau/2),
                              omega2 + k1_w2*(tau/2));
      auto k3_t1 = omega1 + k2_w1*(tau/2);
      auto k3_t2 = omega2 + k2_w2*(tau/2);
      auto [k3_w1, k3_w2] = f(theta1 + k2_t1*(tau/2),
                              theta2 + k2_t2*(tau/2),
                              omega1 + k2_w1*(tau/2),
                              omega2 + k2_w2*(tau/2));
      auto k4_t1 = omega1 + k3_w1*tau;
      auto k4_t2 = omega2 + k3_w2*tau;
      auto [k4_w1, k4_w2] = f(theta1 + k3_t1*tau,
                              theta2 + k3_t2*tau,
                              omega1 + k3_w1*tau,
                              omega2 + k3_w2*tau);
      theta1 += (tau/6)*(k1_t1 + 2*k2_t1 + 2*k3_t1 + k4_t1);
      theta2 += (tau/6)*(k1_t2 + 2*k2_t2 + 2*k3_t2 + k4_t2);
      omega1 += (tau/6)*(k1_w1 + 2*k2_w1 + 2*k3_w1 + k4_w1);
      omega2 += (tau/6)*(k1_w2 + 2*k2_w2 + 2*k3_w2 + k4_w2);
    }
    t -= tau;
  }
  return {theta1, theta2};
}

void render(cbn::DrawCanvas &dc, f64 theta1, f64 theta2) {
  using namespace cbn::math;
  static const Vec2 pivot {dc.Width()/2.f, 100};
  Vec2 v1 {Sin(theta1), Cos(theta1)}, v2 {Sin(theta2), Cos(theta2)};
  auto bob1 = pivot + l*v1*pixels_per_meter, bob2 = bob1 + l*v2*pixels_per_meter;
  dc.Fill(0x181818ff);
  dc.DrawLine(pivot, bob1, 0xdcdcdcff);
  dc.DrawLine(bob1, bob2, 0xdcdcdcff);
  dc.DrawCircle(bob1, 20, 0xffdd33ff);
  dc.DrawCircle(bob2, 20, 0xffdd33ff);
  dc.UpdateWindow();
}

int main(void) {
  auto canvas = cbn::DrawCanvas::New(1280, 720);
  canvas->OpenWindow("Double Pendulum");
  cbn::win::SetMaxFPS(60);
  cbn::win::ForFrame([&](const auto dt){
    auto [theta1, theta2] = update(dt);
    render(*canvas, theta1, theta2);
  });
  cbn::win::Close();
}
