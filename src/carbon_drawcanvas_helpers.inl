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
} Vertex3D;  // TODO: find a better name for this struct

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

CBNINL bool carbon_drawcanvas__triangle_norm(const CBN_DrawCanvas *dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, usz *lx, usz *hx, usz *ly, usz *hy) {
  *lx = *hx = v1.x;
  *ly = *hy = v1.y;
  if (*lx > v2.x) *lx = v2.x;
  if (*hx < v2.x) *hx = v2.x;
  if (*ly > v2.y) *ly = v2.y;
  if (*hy < v2.y) *hy = v2.y;
  if (*lx > v3.x) *lx = v3.x;
  if (*hx < v3.x) *hx = v3.x;
  if (*ly > v3.y) *ly = v3.y;
  if (*hy < v3.y) *hy = v3.y;
  *lx = carbon_math_clamp(*lx, 0, dc->width - 1);
  *hx = carbon_math_clamp(*hx, 0, dc->width - 1);
  *ly = carbon_math_clamp(*ly, 0, dc->height - 1);
  *hy = carbon_math_clamp(*hy, 0, dc->height - 1);
  if (*lx > *hx || *ly > *hy) return false;
  return true;
}

CBNINL bool carbon_drawcanvas__triangle_barycentric(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, usz x, usz y, i32 *u1, i32 *u2, i32 *det) {
  *det = (v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y);
  *u1  = (v2.y - v3.y) * (x - v3.x) + (v3.x - v2.x) * (y - v3.y);
  *u2  = (v3.y - v1.y) * (x - v3.x) + (v1.x - v3.x) * (y - v3.y);
  i32 u3 = *det - *u1 - *u2;
  i8 det_sign = CARBON_SIGN(*det);
  return (CARBON_SIGN(*u1) == det_sign || !*u1) && (CARBON_SIGN(*u2) == det_sign || !*u2) && (CARBON_SIGN(u3) == det_sign || !u3);
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
  const CBN_Mat4 M = carbon_math_mat4_model(t.position, carbon_math_quat_from_euler(t.rotation), carbon_math_vec3_scale(t.scale, 0.5));
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
  // Intersects edge (a -> b) against plane (z + w = ε)
  f32 n = -(a.clip.z + a.clip.w - CARBON_DRAWCANVAS__NEAR_PLANE_EPSILON);
  f32 d = (b.clip.z - a.clip.z) + (b.clip.w - a.clip.w);
  f32 t = 0;
  if (carbon_math_abs(d) > CARBON_EPS) t = n / d;
  t = carbon_math_clamp(t, 0, 1);
  return (Vertex3D) {
    .world = carbon_math_vec3_lerp(a.world, b.world, t),
    .clip = carbon_math_vec4_lerp(a.clip, b.clip, t)
  };
}

CBNINL usz carbon_drawcanvas__near_plane_clipping(Vertex3D v1, Vertex3D v2, Vertex3D v3, Vertex3D *out_poly) {
  // Sutherland-Hodgman polygon clipping algorithm
  // Clips against plane (z + w >= ε)
  Vertex3D in[] = {v1, v2, v3};
  Vertex3D out[4];
  usz out_count = 0;
  for (usz i = 0; i < 3; ++i) {
    Vertex3D a = in[i];
    Vertex3D b = in[(i + 1) % 3];
    bool is_in_a = a.clip.z + a.clip.w >= CARBON_DRAWCANVAS__NEAR_PLANE_EPSILON;
    bool is_in_b = b.clip.z + b.clip.w >= CARBON_DRAWCANVAS__NEAR_PLANE_EPSILON;
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
  // if (v.w < CARBON_EPS) return false;
  CBN_Vec3 ndc;
  if (!carbon_math_vec4_project_3d(v, &ndc)) return false;
  ndc.x = (ndc.x + 1)/2 * dc->width;
  ndc.y = (1 - (ndc.y + 1)/2) * dc->height;
  *out_v = ndc;
  return true;
}

CBNINL u32 carbon_drawcanvas__flat_shading(u32 color, CBN_Vec3 v1, CBN_Vec3 v2, CBN_Vec3 v3, CBN_Vec3 L) {
  static const f32 n_a = 0.2;
  const u32 k_a = carbon_color_scale(color, n_a);
  const CBN_Vec3 N = carbon_math_vec3_norm(carbon_math_vec3_cross(carbon_math_vec3_sub(v2, v1), carbon_math_vec3_sub(v3, v1)));
  const f32 n_d = CARBON_MAX(0, carbon_math_vec3_dot(N, L));
  const u32 k_d = carbon_color_scale(color, n_d);
  return carbon_color_add(k_a, k_d);
}

CBNINL void carbon_drawcanvas__triangle_3d(CBN_DrawCanvas *dc, CBN_Vec3 v1, CBN_Vec3 v2, CBN_Vec3 v3, u32 color) {
  // Barycentric-based 3D triangle rasterization algorithm
  usz lx, hx, ly, hy;
  if (!carbon_drawcanvas__triangle_norm(dc, v1.xy, v2.xy, v3.xy, &lx, &hx, &ly, &hy)) return;
  for (usz j = ly; j <= hy; ++j) {
    for (usz i = lx; i <= hx; ++i) {
      i32 u1, u2, det;
      if (!carbon_drawcanvas__triangle_barycentric(v1.xy, v2.xy, v3.xy, i, j, &u1, &u2, &det)) continue;
      f32 w1 = (f32) u1/det, w2 = (f32) u2/det, w3 = 1 - w1 - w2;
      f32 z = v1.z*w1 + v2.z*w2 + v3.z*w3;
      usz idx = j * dc->width + i;
      if (z < dc->zbuffer[idx]) {
        dc->zbuffer[idx] = z;
        carbon_drawcanvas__alpha_blending(&dc->pixels[idx], color);
      }
    }
  }
}

CBNINL void carbon_drawcanvas__poly_triangulation(CBN_DrawCanvas *dc, const Vertex3D *vs, usz vs_count, CBN_Vec3 light, u32 color) {
  if (vs_count < 3) return;
  for (usz i = 1; i + 1 < vs_count; ++i) {
    Vertex3D p1 = vs[0], p2 = vs[i], p3 = vs[i+1];
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p1.clip, &p1.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p2.clip, &p2.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p3.clip, &p3.screen)) continue;
    u32 shade = carbon_drawcanvas__flat_shading(color, p1.world, p2.world, p3.world, light);
    carbon_drawcanvas__triangle_3d(dc, p1.screen, p2.screen, p3.screen, shade);
  }
}

// Local Variables:
// mode: c
// End:
