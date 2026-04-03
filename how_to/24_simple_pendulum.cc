// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

static constexpr auto l = 2;
static constexpr auto g = 9.81;
static constexpr auto tau = 1/50.;
static constexpr auto pixels_per_meter = 200;

f64 update(f64 dt) {
  using namespace cbn::math;
  static auto t = 0.0;
  static auto theta = pi/2;
  static auto omega = 0.0;
  t += dt;
  while (t >= tau) {
    {// Physics
      auto alpha = (-g/l) * Sin(theta);
      omega += alpha * tau;
      theta += omega * tau;
    }
    t -= tau;
  }
  return theta;
}

void render(cbn::DrawCanvas &dc, f64 theta) {
  using namespace cbn::math;
  static const auto pivot = Vec2(dc.Width()/2, 100);
  cbn::math::Vec2 v {Sin(theta), Cos(theta)};
  auto bob = pivot + l*v*pixels_per_meter;
  dc.Fill(0x181818ff);
  dc.DrawLine(pivot, bob, 0xdcdcdcff);
  dc.DrawCircle(bob, 20, 0xffdd33ff);
  dc.UpdateWindow();
}

int main(void) {
  auto canvas = cbn::DrawCanvas::New(1280, 720);
  canvas->OpenWindow("Simple Pendulum");
  cbn::win::SetMaxFPS(60);
  cbn::win::ForFrame([&](const auto dt){
    auto theta = update(dt);
    render(*canvas, theta);
  });
  cbn::win::Close();
}
