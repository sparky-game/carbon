#version 330 core

in vec2 i_uv;
out vec4 o_color;
uniform sampler2D u_tex;

layout(std140) uniform PostFX {
  vec4 u_barrel_distortion;
  vec4 u_chromatic_aberration;
  vec4 u_scanlines;
  vec4 u_vignette;
};

#define barrel_distortion_intensity u_barrel_distortion[1]
vec2 barrel_distortion(vec2 uv) {
  vec2 d=uv*2.-1.;
  return uv+.05*barrel_distortion_intensity*d*dot(d,d);
}

#define chromatic_aberration_intensity u_chromatic_aberration[1]
#define chromatic_aberration_edge_fade u_chromatic_aberration[2]
vec3 chromatic_aberration(vec2 uv) {
  vec2 o=(uv-.5)*.012*chromatic_aberration_intensity;
  float w=mix(.001,.03,chromatic_aberration_edge_fade);
  vec2 e=smoothstep(0.,w,uv)*smoothstep(0.,w,1.-uv);
  return vec3(
    texture(u_tex,uv-o).r,
    texture(u_tex,uv).g,
    texture(u_tex,uv+o).b
  )*e.x*e.y;
}

#define scanlines_density u_scanlines[1]
#define scanlines_opacity u_scanlines[2]
vec3 scanlines(vec3 c) {
  float s=1.-(abs(sin(gl_fragCoord.y*mix(.5,2.5,scanlines_density)))*scanlines_opacity);
  return c*s;
}

#define vignette_radius     u_vignette[1]
#define vignette_smoothness u_vignette[2]
#define vignette_intensity  u_vignette[3]
vec3 vignette(vec3 c, vec2 uv) {
  float e0=vignette_radius+vignette_smoothness;
  float e1=vignette_radius;
  return mix(c,c*smoothstep(e0,e1,length(uv-.5)),vignette_intensity);
}

void main() {
  vec2 uv = i_uv;
  if (u_barrel_distortion[0] > .5) uv = barrel_distortion(uv);
  vec3 c = u_chromatic_aberration[0] > .5 ? chromatic_aberration(uv) : texture(u_tex, uv).rgb;
  if (u_scanlines[0] > .5) c = scanlines(c);
  if (u_vignette[0] > .5) c = vignette(c, uv);
  o_color = vec4(c, 1.);
}
