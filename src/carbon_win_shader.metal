#include <metal_stdlib>
using namespace metal;

struct V2f {
  float4 pos [[position]];
  float2 uv;
};

struct PostFX {
  float4 barrel_distortion;
  float4 chromatic_aberration;
  float4 scanlines;
  float4 vignette;
};

vertex V2f vert(uint id [[vertex_id]])
{
  float2 pos[] = {{-1, -1}, {3, -1}, {-1, 3}};
  float2 uv[] = {{0, 1}, {2, 1}, {0, -1}};
  V2f o;
  o.pos = float4(pos[id], 0, 1);
  o.uv = uv[id];
  return o;
}

float2 barrel_distortion(float2 uv, constant PostFX &fx) {
#define barrel_distortion_intensity fx.barrel_distortion[1]
  float2 d=uv*2.-1.;
  return uv+.05*barrel_distortion_intensity*d*dot(d,d);
}

float3 chromatic_aberration(float2 uv, texture2d<float> tex, sampler s, constant PostFX &fx) {
#define chromatic_aberration_intensity fx.chromatic_aberration[1]
#define chromatic_aberration_edge_fade fx.chromatic_aberration[2]
  float2 o=(uv-.5)*.012*chromatic_aberration_intensity;
  float w=mix(.001,.03,chromatic_aberration_edge_fade);
  float2 e=smoothstep(0.,w,uv)*smoothstep(0.,w,1.-uv);
  return float3(tex.sample(s,uv-o).r,
                tex.sample(s,uv).g,
                tex.sample(s,uv+o).b)*e.x*e.y;
}

float3 scanlines(float3 c, float4 p, constant PostFX &fx) {
#define scanlines_density fx.scanlines[1]
#define scanlines_opacity fx.scanlines[2]
  float s=1.-(abs(sin(p.y*mix(.5,2.5,scanlines_density)))*scanlines_opacity);
  return c*s;
}

float3 vignette(float3 c, float2 uv, constant PostFX &fx) {
#define vignette_radius     fx.vignette[1]
#define vignette_smoothness fx.vignette[2]
#define vignette_intensity  fx.vignette[3]
  float e0=vignette_radius+vignette_smoothness;
  float e1=vignette_radius;
  return mix(c,c*smoothstep(e0,e1,length(uv-.5)),vignette_intensity);
}

fragment float4 frag(V2f in [[stage_in]],
                     texture2d<float> tex [[texture(0)]],
                     constant PostFX &fx [[buffer(0)]])
{
  constexpr sampler s(min_filter::linear, mag_filter::linear, address::clamp_to_zero);
  float2 uv = in.uv;
  if (fx.barrel_distortion[0] > .5) uv = barrel_distortion(uv, fx);
  float3 c = fx.chromatic_aberration[0] > .5 ? chromatic_aberration(uv, tex, s, fx) : tex.sample(s, uv).rgb;
  if (fx.scanlines[0] > .5) c = scanlines(c, in.pos, fx);
  if (fx.vignette[0] > .5) c = vignette(c, uv, fx);
  return float4(c, 1.);
}
