// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

int main(void) {
  static constexpr auto size = 250;
  static constexpr auto width = 640, height = 360;
  static constexpr cbn::math::Vec2 center {width/2, height/2};
  static constexpr f32 delta = size/CARBON_SQRT3;
  static const cbn::math::Vec2 v1 {center.x, center.y - delta};
  static const cbn::math::Vec2 v2 {center.x - size/2, center.y + delta/2};
  static const cbn::math::Vec2 v3 {center.x + size/2, center.y + delta/2};
  auto canvas = cbn::DrawCanvas::New(width, height);
  canvas->OpenWindow("Rotate");
  cbn::win::SetMaxFPS(60);
  f32 angle = 0;
  cbn::win::ForFrame([&](const auto dt){
    canvas->Fill(0x181818ff);
    canvas->DrawTriangle(v1.RotatePivot(angle, center),
                         v2.RotatePivot(angle, center),
                         v3.RotatePivot(angle, center),
                         0xffdd33ff);
    angle += 50 * dt;
    canvas->UpdateWindow();
  });
  cbn::win::Close();
}
