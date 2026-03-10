#include <metal_stdlib>

using namespace metal;

struct V2f {
  float4 pos [[position]];
  float2 uv;
};

vertex V2f vert(uint id [[vertex_id]]) {
  float2 pos[] = {{-1, -1}, {3, -1}, {-1, 3}};
  float2 uv[] = {{0, 1}, {2, 1}, {0, -1}};
  V2f o;
  o.pos = float4(pos[id], 0, 1);
  o.uv = uv[id];
  return o;
}

fragment float4 frag(V2f in [[stage_in]], texture2d<float> tex [[texture(0)]]) {
  constexpr sampler s(filter::nearest);
  return tex.sample(s, in.uv);
}
