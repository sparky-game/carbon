// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define carbon_drawcanvas_at(dc, i, j) (dc)->pixels[(j) * (dc)->width + (i)]

#define carbon_drawcanvas__dispatch_backend(name, ...)  \
  dc->flags & CARBON_DRAWCANVAS_FLAG_MULTITHREAD        \
  ? carbon_drawcanvas__mt_ ## name(__VA_ARGS__)         \
  : carbon_drawcanvas__st_ ## name(__VA_ARGS__);

typedef struct {
  CBN_Vec3 world;
  CBN_Vec4 clip;
  CBN_Vec3 screen;
  CBN_Vec2 uv;
} Vertex3D;  // TODO: find a better name for this struct (e.g. `VertexInfo`).

CBNINL void carbon_drawcanvas__alpha_blending(u32 *dst, u32 src) {
  const u32 a = src & 0xff;
  if (a == 0) return;
  if (a == 0xff) {
    *dst = src;
    return;
  }
  const u32 d = *dst;
  const u32 ia = 0xff - a;
  u32 dst_r = (d   >> 24) & 0xff;
  u32 src_r = (src >> 24) & 0xff;
  u32 r = ((dst_r*ia + src_r*a) * 0x101) >> 16;
  u32 dst_g = (d   >> 16) & 0xff;
  u32 src_g = (src >> 16) & 0xff;
  u32 g = ((dst_g*ia + src_g*a) * 0x101) >> 16;
  u32 dst_b = (d   >> 8) & 0xff;
  u32 src_b = (src >> 8) & 0xff;
  u32 b = ((dst_b*ia + src_b*a) * 0x101) >> 16;
  *dst = (r << 24) | (g << 16) | (b << 8) | 0xff;
}

CBNINL bool carbon_drawcanvas__triangle_aabb(const CBN_DrawCanvas *dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, CBN_Vec2 *lo, CBN_Vec2 *hi) {
  lo->x = carbon_math_min_3(v1.x, v2.x, v3.x);
  lo->y = carbon_math_min_3(v1.y, v2.y, v3.y);
  hi->x = carbon_math_max_3(v1.x, v2.x, v3.x);
  hi->y = carbon_math_max_3(v1.y, v2.y, v3.y);
  const CBN_Vec2 scr = carbon_math_vec2(dc->width - 1, dc->height - 1);
  *lo = carbon_math_vec2_clamp(*lo, carbon_math_vec2_1(0), scr);
  *hi = carbon_math_vec2_clamp(*hi, carbon_math_vec2_1(0), scr);
  return lo->x <= hi->x && lo->y <= hi->y;
}

CBNINL bool carbon_drawcanvas__rect_normalize(const CBN_DrawCanvas *dc, const CBN_Rect r, i32 *x1, i32 *x2, i32 *y1, i32 *y2) {
  if (!r.w || !r.h) return false;
  i32 ox1 = r.x;
  i32 oy1 = r.y;
  i32 ox2 = ox1 + CARBON_SIGN(r.w) * (carbon_math_abs(r.w) - 1);
  if (ox1 > ox2) CARBON_SWAP(ox1, ox2);
  i32 oy2 = oy1 + CARBON_SIGN(r.h) * (carbon_math_abs(r.h) - 1);
  if (oy1 > oy2) CARBON_SWAP(oy1, oy2);
  if (ox1 >= (i32) dc->width || ox2 < 0 || oy1 >= (i32) dc->height || oy2 < 0) return false;
  *x1 = ox1;
  if (*x1 < 0) *x1 = 0;
  *x2 = ox2;
  if (*x2 >= (i32) dc->width) *x2 = dc->width - 1;
  *y1 = oy1;
  if (*y1 < 0) *y1 = 0;
  *y2 = oy2;
  if (*y2 >= (i32) dc->height) *y2 = dc->height - 1;
  return true;
}

CBNINL void carbon_drawcanvas__local_to_clip_space(const CBN_Camera *c, const CBN_Mesh *m, CBN_Transform t, Vertex3D *out_vs) {
  const CBN_Mat4 M = carbon_math_mat4_model(t.position,
                                            carbon_math_quat_from_euler(t.rotation),
                                            carbon_math_vec3_scale(t.scale, 0.5));
  const CBN_Mat4 V = carbon_camera_get_view(c);
  const CBN_Mat4 P = carbon_camera_get_proj(c);
  const CBN_Mat4 MVP = carbon_math_mat4_mult(P, carbon_math_mat4_mult(V, M));
  for (usz i = 0; i < m->metadata.vertices_count; ++i) {
    CBN_Vec4 v = carbon_math_vec4_3(m->vertices[i], 1);
    out_vs[i].world = carbon_math_mat4_mult_vec4(M, v).xyz;
    out_vs[i].clip = carbon_math_mat4_mult_vec4(MVP, v);
  }
}

CBNINL bool carbon_drawcanvas__is_back_face(const CBN_DrawCanvas *dc, CBN_Vec3 cam_pos, CBN_Vec3 v1, CBN_Vec3 v2, CBN_Vec3 v3) {
  CBN_Vec3 N = carbon_math_vec3_cross(carbon_math_vec3_sub(v2, v1), carbon_math_vec3_sub(v3, v1));
  CBN_Vec3 V = carbon_math_vec3_sub(cam_pos, v1);
  f32 orientation = carbon_math_vec3_dot(N, V);
  return dc->flags & CARBON_DRAWCANVAS_FLAG_FRONTFACE_CW ? orientation >= 0 : orientation <= 0;
}

CBNINL Vertex3D carbon_drawcanvas__clip_intersect(Vertex3D a, Vertex3D b) {
  // Intersects edge (a -> b) against plane (z + w = 0)
  f32 n = -(a.clip.z + a.clip.w);
  f32 d = (b.clip.z - a.clip.z) + (b.clip.w - a.clip.w);
  f32 t = 0;
  if (carbon_math_abs(d) > CARBON_EPS) t = n/d;
  t = carbon_math_clamp(t, 0, 1);
  return (Vertex3D) {
    .world = carbon_math_vec3_lerp(a.world, b.world, t),
    .clip = carbon_math_vec4_lerp(a.clip, b.clip, t),
    .uv = carbon_math_vec2_lerp(a.uv, b.uv, t)
  };
}

CBNINL usz carbon_drawcanvas__near_plane_clipping(Vertex3D v1, Vertex3D v2, Vertex3D v3, Vertex3D *out_poly) {
  // Sutherland-Hodgman polygon clipping algorithm
  // Clips against plane (z + w >= 0)
  Vertex3D in[] = {v1, v2, v3};
  Vertex3D out[4];
  usz out_count = 0;
  for (usz i = 0; i < 3; ++i) {
    Vertex3D a = in[i];
    Vertex3D b = in[(i + 1) % 3];
    bool is_in_a = a.clip.z + a.clip.w >= 0;
    bool is_in_b = b.clip.z + b.clip.w >= 0;
    if (is_in_a && is_in_b)       out[out_count++] = b;
    else if (is_in_a && !is_in_b) out[out_count++] = carbon_drawcanvas__clip_intersect(a, b);
    else if (!is_in_a && is_in_b) {
      out[out_count++] = carbon_drawcanvas__clip_intersect(a, b);
      out[out_count++] = b;
    }
  }
  for (usz i = 0; i < out_count; ++i) out_poly[i] = out[i];
  return out_count;
}

CBNINL bool carbon_drawcanvas__clip_to_screen_space(const CBN_DrawCanvas *dc, const CBN_Vec4 v, CBN_Vec3 *out_v) {
  CBN_Vec3 ndc;
  if (!carbon_math_vec4_project_3d(v, &ndc)) return false;
  ndc.x = (ndc.x + 1)/2 * dc->width;
  ndc.y = (1 - (ndc.y + 1)/2) * dc->height;
  *out_v = ndc;
  return true;
}

CBNINL u32 carbon_drawcanvas__flat_shading(CBN_DrawCanvas *dc, CBN_Vec3 v1, CBN_Vec3 v2, CBN_Vec3 v3, u32 color) {
  static const f32 n_a = 0.2;
  const CBN_Vec3 N = carbon_math_vec3_norm(carbon_math_vec3_cross(carbon_math_vec3_sub(v2, v1), carbon_math_vec3_sub(v3, v1)));
  const CBN_Vec3 centroid = carbon_math_vec3_scale(carbon_math_vec3_add(carbon_math_vec3_add(v1, v2), v3), 1/3.f);
  u32 k = carbon_color_scale(color, n_a);
  for (usz i = 0; i < dc->lights_count; ++i) {
    const CBN_Light *l = &dc->lights[i];
    if (!l->active) continue;
    CBN_Vec3 L = carbon_math_vec3_1(0);
    f32 att = 1;
    switch (l->type) {
    case CBN_LightType_Directional:
      L = l->as_dir.direction;
      break;
    case CBN_LightType_Point: {
      CBN_Vec3 diff = carbon_math_vec3_sub(l->as_point.position, centroid);
      f32 d = carbon_math_vec3_len(diff);
      L = carbon_math_vec3_scale(diff, 1/d);
      att = carbon_math_max(0, 1 - (d/l->as_point.range));
      att *= att;
    } break;
    case CBN_LightType_Count:
    default: CARBON_UNREACHABLE;
    }
    const f32 n_d = carbon_math_max(0, carbon_math_vec3_dot(N, L)) * l->intensity * att;
    const u32 k_d = carbon_color_mult(carbon_color_scale(color, n_d), l->color);
    k = carbon_color_add(k, k_d);
  }
  return k;
}

#define PINEDA_ALGORITHM_SETUP(dc, v1, v2, v3)                          \
  static const i64 spxsc = 256;                                         \
  CBN_Vec2 lo, hi;                                                      \
  if (!carbon_drawcanvas__triangle_aabb((dc), (v1).xy, (v2).xy, (v3).xy, &lo, &hi)) return; \
  i64 x1 = (v1).x * spxsc, y1 = (v1).y * spxsc;                         \
  i64 x2 = (v2).x * spxsc, y2 = (v2).y * spxsc;                         \
  i64 x3 = (v3).x * spxsc, y3 = (v3).y * spxsc;                         \
  i64 A = (x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1);                    \
  if (A == 0) return;                                                   \
  i64 dw0_dx = -(y3 - y2) * spxsc, dw0_dy = (x3 - x2) * spxsc;          \
  i64 dw1_dx = -(y1 - y3) * spxsc, dw1_dy = (x1 - x3) * spxsc;          \
  i64 dw2_dx = -(y2 - y1) * spxsc, dw2_dy = (x2 - x1) * spxsc;          \
  i64 sx = (i64)lo.x * spxsc + (spxsc/2), sy = (i64)lo.y * spxsc + (spxsc/2); \
  i64 w0_row = (x3 - x2)*(sy - y2) - (y3 - y2)*(sx - x2);               \
  i64 w1_row = (x1 - x3)*(sy - y3) - (y1 - y3)*(sx - x3);               \
  i64 w2_row = (x2 - x1)*(sy - y1) - (y2 - y1)*(sx - x1);               \
  if (A < 0) {                                                          \
  A      = -A;                                                          \
  w0_row = -w0_row, w1_row = -w1_row, w2_row = -w2_row;                 \
  dw0_dx = -dw0_dx, dw1_dx = -dw1_dx, dw2_dx = -dw2_dx;                 \
  dw0_dy = -dw0_dy, dw1_dy = -dw1_dy, dw2_dy = -dw2_dy;                 \
  }                                                                     \
  w0_row += dw0_dx > 0 || (dw0_dx == 0 && dw0_dy > 0) ? 0 : -1;         \
  w1_row += dw1_dx > 0 || (dw1_dx == 0 && dw1_dy > 0) ? 0 : -1;         \
  w2_row += dw2_dx > 0 || (dw2_dx == 0 && dw2_dy > 0) ? 0 : -1;         \
  f32 iA = 1.f/A;                                                       \
  f32 dz_dx = ((v1).z*dw0_dx + (v2).z*dw1_dx + (v3).z*dw2_dx) * iA;     \
  f32 dz_dy = ((v1).z*dw0_dy + (v2).z*dw1_dy + (v3).z*dw2_dy) * iA;     \
  f32 z_row = ((v1).z*w0_row + (v2).z*w1_row + (v3).z*w2_row) * iA;

CBNINL void carbon_drawcanvas__triangle_3d(CBN_DrawCanvas *dc, CBN_Vec3 v1, CBN_Vec3 v2, CBN_Vec3 v3, u32 color) {
  // Pineda incremental edge function algorithm
  PINEDA_ALGORITHM_SETUP(dc, v1, v2, v3);
  for (usz j = lo.y; j <= hi.y; ++j) {
    i64 w0 = w0_row, w1 = w1_row, w2 = w2_row;
    f32 z = z_row;
    f32 *zbuf = dc->zbuffer + j*dc->width + (usz)lo.x;
    u32 *pxls = dc->pixels + j*dc->width + (usz)lo.x;
    for (usz i = lo.x; i <= hi.x; ++i, ++zbuf, ++pxls) {
      if (w0 >= 0 && w1 >= 0 && w2 >= 0 && z < *zbuf) {
        *zbuf = z;
        carbon_drawcanvas__alpha_blending(pxls, color);
      }
      w0 += dw0_dx, w1 += dw1_dx, w2 += dw2_dx, z += dz_dx;
    }
    w0_row += dw0_dy, w1_row += dw1_dy, w2_row += dw2_dy, z_row += dz_dy;
  }
}

CBNINL void carbon_drawcanvas__triangle_3d_with_texture(CBN_DrawCanvas *dc, Vertex3D v1, Vertex3D v2, Vertex3D v3, const CBN_Sprite *s, u32 shade) {
  // Pineda incremental edge function algorithm
  PINEDA_ALGORITHM_SETUP(dc, v1.screen, v2.screen, v3.screen);
  f32 iw1 = 1/v1.clip.w, iw2 = 1/v2.clip.w, iw3 = 1/v3.clip.w;
  f32 uw1 = v1.uv.x * iw1, vw1 = v1.uv.y * iw1;
  f32 uw2 = v2.uv.x * iw2, vw2 = v2.uv.y * iw2;
  f32 uw3 = v3.uv.x * iw3, vw3 = v3.uv.y * iw3;
  f32 diw_dx = (iw1*dw0_dx + iw2*dw1_dx + iw3*dw2_dx) * iA;
  f32 diw_dy = (iw1*dw0_dy + iw2*dw1_dy + iw3*dw2_dy) * iA;
  f32 iw_row = (iw1*w0_row + iw2*w1_row + iw3*w2_row) * iA;
  f32 duw_dx = (uw1*dw0_dx + uw2*dw1_dx + uw3*dw2_dx) * iA;
  f32 duw_dy = (uw1*dw0_dy + uw2*dw1_dy + uw3*dw2_dy) * iA;
  f32 uw_row = (uw1*w0_row + uw2*w1_row + uw3*w2_row) * iA;
  f32 dvw_dx = (vw1*dw0_dx + vw2*dw1_dx + vw3*dw2_dx) * iA;
  f32 dvw_dy = (vw1*dw0_dy + vw2*dw1_dy + vw3*dw2_dy) * iA;
  f32 vw_row = (vw1*w0_row + vw2*w1_row + vw3*w2_row) * iA;
  for (usz j = lo.y; j <= hi.y; ++j) {
    i64 w0 = w0_row, w1 = w1_row, w2 = w2_row;
    f32 z = z_row, iw = iw_row, uw = uw_row, vw = vw_row;
    f32 *zbuf = dc->zbuffer + j*dc->width + (usz)lo.x;
    u32 *pxls = dc->pixels + j*dc->width + (usz)lo.x;
    for (usz i = lo.x; i <= hi.x; ++i, ++zbuf, ++pxls) {
      if (w0 >= 0 && w1 >= 0 && w2 >= 0 && z < *zbuf) {
        *zbuf = z;
        f32 w = 1/iw;
        f32 u = carbon_math_clamp(uw * w, 0, 1), v = carbon_math_clamp(vw * w, 0, 1);
        usz x = u*(s->width - 1), y = (1 - v)*(s->height - 1);
        u32 color = carbon_color_mult(s->pixels[y*s->width + x], shade);
        carbon_drawcanvas__alpha_blending(pxls, color);
      }
      w0 += dw0_dx, w1 += dw1_dx, w2 += dw2_dx, z += dz_dx;
      iw += diw_dx, uw += duw_dx, vw += dvw_dx;
    }
    w0_row += dw0_dy, w1_row += dw1_dy, w2_row += dw2_dy, z_row += dz_dy;
    iw_row += diw_dy, uw_row += duw_dy, vw_row += dvw_dy;
  }
}

CBNINL void carbon_drawcanvas__poly_triangulation(CBN_DrawCanvas *dc, const Vertex3D *vs, usz vs_count, u32 color) {
  if (vs_count < 3) return;
  for (usz i = 1; i + 1 < vs_count; ++i) {
    Vertex3D p1 = vs[0], p2 = vs[i], p3 = vs[i+1];
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p1.clip, &p1.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p2.clip, &p2.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p3.clip, &p3.screen)) continue;
    u32 shade = carbon_drawcanvas__flat_shading(dc, p1.world, p2.world, p3.world, color);
    carbon_drawcanvas__triangle_3d(dc, p1.screen, p2.screen, p3.screen, shade);
  }
}

CBNINL void carbon_drawcanvas__poly_triangulation_with_texture(CBN_DrawCanvas *dc, const Vertex3D *vs, usz vs_count, const CBN_Sprite *s) {
  if (vs_count < 3) return;
  for (usz i = 1; i + 1 < vs_count; ++i) {
    Vertex3D p1 = vs[0], p2 = vs[i], p3 = vs[i+1];
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p1.clip, &p1.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p2.clip, &p2.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p3.clip, &p3.screen)) continue;
    u32 shade = carbon_drawcanvas__flat_shading(dc, p1.world, p2.world, p3.world, 0xffffffff);
    carbon_drawcanvas__triangle_3d_with_texture(dc, p1, p2, p3, s, shade);
  }
}

// Local Variables:
// mode: c
// End:
