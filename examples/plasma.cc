// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

namespace shader {
  using vec2 = cbn::math::Vec2;
  using vec4 = cbn::math::Vec4;
  static constexpr auto abs  = cbn::math::Abs;
  static constexpr auto exp  = [](const auto &v){ return v.Exp(); };
  static constexpr auto sin  = [](const auto &v){ return v.Sin(); };
  static constexpr auto cos  = [](const auto &v){ return v.Cos(); };
  static constexpr auto tanh = [](const auto &v){ return v.Tanh(); };
  static constexpr auto dot  = [](const auto &u, const auto &v){ return u.Dot(v); };
  // Original GLSL shader code (<https://xcancel.com/XorDev/status/1894494786368745715>):
  /*
    vec2 p=(FC.xy*2.-r)/r.y,l,i,v=p*(l+=4.-4.*abs(.7-dot(p,p)));
    for(;i.y++<8.;o+=(sin(v.xyyx)+1.)*abs(v.x-v.y))v+=cos(v.yx*i.y+i+t)/i.y+.7;
    o=tanh(5.*exp(l.x-4.-p.y*vec4(-1,1,2,0))/o);
  */
  static vec4 Run(vec2 FC, vec2 r, f64 t) {
    vec4 o{};
    vec2 p=(FC.xy()*2.-r)/r.y,l{},i{},v=p*(l+=4.-4.*abs(.7-dot(p,p)));
    for(;i.y++<8.;o+=(sin(v.xyyx())+1.)*abs(v.x-v.y))v+=cos(v.yx()*i.y+i+t)/i.y+.7;
    o=tanh(5.*exp(l.x-4.-p.y*CARBON_VEC4(-1,1,2,0))/o);
    return o;
  }
}

void Render(cbn::DrawCanvas &dc, const f64 dt) {
  static auto t = dt;
  const auto &w = dc.width, &h = dc.height;
  for (usz y = 0; y < h; ++y) {
    for (usz x = 0; x < w; ++x) {
      const auto o = shader::Run(cbn::math::Vec2(x, y), cbn::math::Vec2(w, h), t);
      dc(x, y) = cbn::color::FromNormalized(o);
    }
  }
  t += dt;
}

int main(void) {
  auto canvas = cbn::DrawCanvas::make(640, 360);
  cbn::win::Open(canvas, "Plasma Animation");
  cbn::win::SetMaxFPS(60);
  cbn::win::ForFrame([&](const auto dt){
    Render(canvas, dt);
    cbn::win::Update(canvas);
  });
  cbn::win::Close();
  canvas.Free();
}
