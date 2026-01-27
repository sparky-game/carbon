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
  static vec4 Run(vec4 &o, vec4 FC, vec2 r, f64 t) {
    vec2 p=(FC.xy*2.-r)/r.y,l,i,v=p*(l+=4.-4.*abs(.7-dot(p,p)));
    for(;i.y++<8.;o+=(sin(v.xyyx())+1.)*abs(v.x-v.y))v+=cos(v.yx()*i.y+i+t)/i.y+.7;
    o=tanh(5.*exp(l.x-4.-p.y*vec4(-1,1,2,0))/o);
    return o;
  }

  void Render(cbn::DrawCanvas &dc, const f64 dt) {
    static auto t = dt;
    const auto &w = dc.width, &h = dc.height;
    for (usz y = 0; y < h; ++y) {
      for (usz x = 0; x < w; ++x) {
        vec4 o;
        Run(o, vec4(x, y, 0, 0), vec2(w, h), t);
        dc(x, y) = cbn::color::FromNormalized(o);
      }
    }
    t += dt;
  }
}

void hud_render(cbn::DrawCanvas &dc) {
  static constexpr auto color = 0x737373ff;
  static constexpr auto text_size = 1;
  static constexpr auto text_padding = 10;
  static const auto text_height = dc.TextHeight(text_size);
  static const std::string render_res = cbn::str::fmt("Render res: %zux%zu", dc.width, dc.height);
  const char *text[] = {
    cbn::str::fmt(CARBON_NAME " %s", cbn::Version(0, 0, 0)),
    cbn::str::fmt("%u fps", cbn::win::GetFPS()),
    render_res.c_str()
  };
  for (usz i = 0; i < CARBON_ARRAY_LEN(text); ++i) {
    dc.DrawText(text[i], cbn::math::Vec2(text_padding, text_padding + i*text_height), text_size, color);
  }
}

void render(cbn::DrawCanvas &dc, const f64 dt) {
  shader::Render(dc, dt);
  hud_render(dc);
}

int main(void) {
  auto canvas = cbn::DrawCanvas::make(640, 360);
  canvas.OpenWindow("Plasma Animation");
  cbn::win::SetMaxFPS(60);
  cbn::win::ForFrame([&](const auto dt){
    render(canvas, dt);
    canvas.UpdateWindow();
  });
  cbn::win::Close();
  canvas.Free();
}
