// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_DRAWCANVAS__AA_RES                2
#define CARBON_DRAWCANVAS__BOX_OUTLINE_COLOR     0x000000ff
#define CARBON_DRAWCANVAS__BOX_TOPLEFT_COLOR     0xffffffff
#define CARBON_DRAWCANVAS__BOX_BOTTOMRIGHT_COLOR 0x555555ff
#define CARBON_DRAWCANVAS__BOX_INSIDE_COLOR      0xc6c6c6ff

#include "carbon_drawcanvas_font.inl"

CBN_DrawCanvas carbon_drawcanvas_create(usz width, usz height) {
  return (CBN_DrawCanvas) {
    .pixels  = (u32 *) carbon_memory_alloc(width * height * sizeof(u32)),
    .zbuffer = (f32 *) carbon_memory_alloc(width * height * sizeof(f32)),
    .width   = width,
    .height  = height
  };
}

void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc) {
  if (!dc) {
    CBN_WARN("`dc` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_memory_free(dc->pixels);
  carbon_memory_free(dc->zbuffer);
  carbon_memory_set(dc, 0, sizeof(*dc));
}

void carbon_drawcanvas_fill(CBN_DrawCanvas dc, u32 color) {
  for (usz i = 0; i < dc.width * dc.height; ++i) {
    dc.pixels[i] = color;
    dc.zbuffer[i] = 1;
  }
}

CBNINL void carbon_drawcanvas__alpha_blending(u32 *c1, u32 c2) {
  u32 a2 = c2 & 0xff;
  if (a2 == 0) return;
  if (a2 == 255) {
    *c1 = c2;
    return;
  }
  u32 r1 = (*c1 >> 24) & 0xff;
  u32 g1 = (*c1 >> 16) & 0xff;
  u32 b1 = (*c1 >> 8)  & 0xff;
  u32 a1 = (*c1 >> 0)  & 0xff;
  u32 r2 = (c2  >> 24) & 0xff;
  u32 g2 = (c2  >> 16) & 0xff;
  u32 b2 = (c2  >> 8)  & 0xff;
  u32 r = (r1 * (255 - a2) + r2 * a2) / 255;
  u32 g = (g1 * (255 - a2) + g2 * a2) / 255;
  u32 b = (b1 * (255 - a2) + b2 * a2) / 255;
  u32 a = a2 + (a1 * (255 - a2)) / 255;
  *c1 = (r << 24) | (g << 16) | (b << 8) | a;
}

void carbon_drawcanvas_line(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, u32 color) {
  i32 x1 = v1.x, y1 = v1.y, x2 = v2.x, y2 = v2.y;
  i32 dx =  carbon_math_abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
  i32 dy = -carbon_math_abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
  i32 err = dx + dy;
  for (;;) {
    if (0 <= x1 && x1 < (i32) dc.width && 0 <= y1 && y1 < (i32) dc.height) {
      carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, (usz) x1, (usz) y1), color);
    }
    if (x1 == x2 && y1 == y2) break;
    i32 e2 = 2 * err;
    if (e2 >= dy) {
      if (x1 == x2) break;
      err += dy;
      x1 += sx;
    }
    if (e2 <= dx) {
      if (y1 == y2) break;
      err += dx;
      y1 += sy;
    }
  }
}

CBNINL bool carbon_drawcanvas__triangle_norm(const CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, usz *lx, usz *hx, usz *ly, usz *hy) {
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
  *lx = CARBON_CLAMP(*lx, 0, dc.width - 1);
  *hx = CARBON_CLAMP(*hx, 0, dc.width - 1);
  *ly = CARBON_CLAMP(*ly, 0, dc.height - 1);
  *hy = CARBON_CLAMP(*hy, 0, dc.height - 1);
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

void carbon_drawcanvas_triangle(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) {
  usz lx, hx, ly, hy;
  if (!carbon_drawcanvas__triangle_norm(dc, v1, v2, v3, &lx, &hx, &ly, &hy)) return;
  for (usz j = ly; j <= hy; ++j) {
    for (usz i = lx; i <= hx; ++i) {
      i32 u1, u2, det;
      if (!carbon_drawcanvas__triangle_barycentric(v1, v2, v3, i, j, &u1, &u2, &det)) continue;
      carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, i, j), color);
    }
  }
}

void carbon_drawcanvas_triangle_3d(CBN_DrawCanvas dc, CBN_Vec3 v1, CBN_Vec3 v2, CBN_Vec3 v3, u32 color) {
  usz lx, hx, ly, hy;
  if (!carbon_drawcanvas__triangle_norm(dc, v1.xy, v2.xy, v3.xy, &lx, &hx, &ly, &hy)) return;
  for (usz j = ly; j <= hy; ++j) {
    for (usz i = lx; i <= hx; ++i) {
      i32 u1, u2, det;
      if (!carbon_drawcanvas__triangle_barycentric(v1.xy, v2.xy, v3.xy, i, j, &u1, &u2, &det)) continue;
      f32 w1 = (f32) u1/det, w2 = (f32) u2/det, w3 = 1 - w1 - w2;
      f32 z = v1.z*w1 + v2.z*w2 + v3.z*w3;
      usz idx = j * dc.width + i;
      if (z < dc.zbuffer[idx]) {
        dc.zbuffer[idx] = z;
        carbon_drawcanvas__alpha_blending(&dc.pixels[idx], color);
      }
    }
  }
}

CBNINL bool carbon_drawcanvas__rect_normalize(const CBN_DrawCanvas dc, const CBN_Rect r, i32 *x1, i32 *x2, i32 *y1, i32 *y2) {
  if (!r.w || !r.h) return false;
  i32 ox1 = r.x;
  i32 oy1 = r.y;
  i32 ox2 = ox1 + CARBON_SIGN(r.w) * (carbon_math_abs(r.w) - 1);
  if (ox1 > ox2) CARBON_SWAP(ox1, ox2);
  i32 oy2 = oy1 + CARBON_SIGN(r.h) * (carbon_math_abs(r.h) - 1);
  if (oy1 > oy2) CARBON_SWAP(oy1, oy2);
  if (ox1 >= (i32) dc.width || ox2 < 0 || oy1 >= (i32) dc.height || oy2 < 0) return false;
  *x1 = ox1;
  if (*x1 < 0) *x1 = 0;
  *x2 = ox2;
  if (*x2 >= (i32) dc.width) *x2 = dc.width - 1;
  *y1 = oy1;
  if (*y1 < 0) *y1 = 0;
  *y2 = oy2;
  if (*y2 >= (i32) dc.height) *y2 = dc.height - 1;
  return true;
}

void carbon_drawcanvas_rect(CBN_DrawCanvas dc, CBN_Rect r, u32 color) {
  i32 x1, x2, y1, y2;
  if (!carbon_drawcanvas__rect_normalize(dc, r, &x1, &x2, &y1, &y2)) return;
  u32 *p_dc = dc.pixels + (y1 * dc.width + x1);
  usz row_offset = x2 - x1 + 1;
  for (i32 j = y1; j <= y2; ++j) {
    for (i32 i = x1; i <= x2; ++i) {
      carbon_drawcanvas__alpha_blending(p_dc, color);
      ++p_dc;
    }
    p_dc += dc.width - row_offset;
  }
}

void carbon_drawcanvas_circle(CBN_DrawCanvas dc, CBN_Vec2 center, usz radius, u32 color) {
  i32 x1, x2, y1, y2;
  if (!carbon_drawcanvas__rect_normalize(dc, CARBON_RECT(center.x - radius, center.y - radius, 2*radius, 2*radius), &x1, &x2, &y1, &y2)) return;
  for (i32 j = y1; j <= y2; ++j) {
    for (i32 i = x1; i <= x2; ++i) {
      i64 n = 0;
      for (i64 sx = 0; sx < CARBON_DRAWCANVAS__AA_RES; ++sx) {
        for (i64 sy = 0; sy < CARBON_DRAWCANVAS__AA_RES; ++sy) {
          i64 res = CARBON_DRAWCANVAS__AA_RES + 1;
          i64 dx = (i * res * 2) + 2 + (sx * 2) - (res * center.x * 2) - res;
          i64 dy = (j * res * 2) + 2 + (sy * 2) - (res * center.y * 2) - res;
          if (dx*dx + dy*dy <= (i64) (res*res * radius*radius * 4)) ++n;
        }
      }
      u32 aa_alpha = ((color >> 0) & 0xff) * (n / CARBON_DRAWCANVAS__AA_RES / CARBON_DRAWCANVAS__AA_RES);
      u32 aa_color = (color & 0xffffff00) | aa_alpha;
      carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, i, j), aa_color);
    }
  }
}

void carbon_drawcanvas_sprite(CBN_DrawCanvas dc, const CBN_Sprite *s, CBN_Vec2 position) {
  const CBN_Rect r_dc = CARBON_RECT(0, 0, dc.width, dc.height);
  const CBN_Rect r_sp = CARBON_RECT(position.x, position.y, s->width, s->height);
  CBN_Rect xywh = carbon_math_rect_intersection(r_dc, r_sp);
  u32 *p_dc = dc.pixels + (usz)(xywh.y * r_dc.w + xywh.x);
  const u32 *p_sp = s->pixels + (usz)((xywh.y - r_sp.y) * r_sp.w + (xywh.x - r_sp.x));
  for (usz j = 0; j < xywh.h; ++j) {
    for (usz i = 0; i < xywh.w; ++i) {
      carbon_drawcanvas__alpha_blending(p_dc, *p_sp);
      ++p_dc, ++p_sp;
    }
    p_dc += (usz)(r_dc.w - xywh.w);
    p_sp += (usz)(r_sp.w - xywh.w);
  }
}

typedef struct {
  CBN_Vec3 world;
  CBN_Vec4 clip;
  CBN_Vec3 screen;
} Vertex3D;  // TODO: find a better name for this struct

CBNINL void carbon_drawcanvas__local_to_clip_space(const CBN_Camera *c, const CBN_Mesh *m, CBN_Transform t, Vertex3D *out_vs) {
  const CBN_Mat4 M = carbon_math_mat4_model(t.position, carbon_math_quat_from_euler(t.rotation), carbon_math_vec3_scale(t.scale, 0.5));
  const CBN_Mat4 V = carbon_camera_get_view(c);
  const CBN_Mat4 P = carbon_camera_get_proj(c);
  const CBN_Mat4 MVP = carbon_math_mat4_mult(P, carbon_math_mat4_mult(V, M));
  for (usz i = 0; i < m->metadata.vertices_count; ++i) {
    CBN_Vec4 v = CARBON_VEC4_3(m->vertices[i], 1);
    out_vs[i].world = CARBON_VEC3_V(carbon_math_mat4_mult_vec4(M, v));
    out_vs[i].clip = carbon_math_mat4_mult_vec4(MVP, v);
  }
}

CBNINL Vertex3D carbon_drawcanvas__clip_intersect(Vertex3D a, Vertex3D b) {
  // Intersects edge (a -> b) against plane (z + w = 0).
  f32 n = -(a.clip.z + a.clip.w);
  f32 d = (b.clip.z - a.clip.z) + (b.clip.w - a.clip.w);
  f32 t = 0;
  if (carbon_math_abs(d) > CARBON_EPS) t = n / d;
  t = CARBON_CLAMP(t, 0, 1);
  return (Vertex3D) {
    .world = carbon_math_vec3_lerp(a.world, b.world, t),
    .clip = carbon_math_vec4_lerp(a.clip, b.clip, t)
  };
}

CBNINL usz carbon_drawcanvas__near_plane_clipping(Vertex3D v1, Vertex3D v2, Vertex3D v3, Vertex3D *out_poly) {
  // Sutherland-Hodgman polygon clipping algorithm
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

CBNINL bool carbon_drawcanvas__clip_to_screen_space(const CBN_DrawCanvas dc, const CBN_Vec4 v, CBN_Vec3 *out_v) {
  if (v.w <= 0) return false;
  CBN_Vec3 ndc;
  if (!carbon_math_vec4_project_3d(v, &ndc)) return false;
  ndc.x = (ndc.x + 1)/2 * dc.width;
  ndc.y = (1 - (ndc.y + 1)/2) * dc.height;
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

CBNINL void carbon_drawcanvas__poly_triangulation(CBN_DrawCanvas dc, const Vertex3D *vs, usz vs_count, CBN_Vec3 light, u32 color) {
  if (vs_count < 3) return;
  for (usz i = 1; i + 1 < vs_count; ++i) {
    Vertex3D p1 = vs[0], p2 = vs[i], p3 = vs[i+1];
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p1.clip, &p1.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p2.clip, &p2.screen)) continue;
    if (!carbon_drawcanvas__clip_to_screen_space(dc, p3.clip, &p3.screen)) continue;
    u32 shade = carbon_drawcanvas__flat_shading(color, p1.world, p2.world, p3.world, light);
    carbon_drawcanvas_triangle_3d(dc, p1.screen, p2.screen, p3.screen, shade);
  }
}

void carbon_drawcanvas_mesh(CBN_DrawCanvas dc, const CBN_Camera *c, const CBN_Mesh *m, CBN_Transform t, u32 color) {
  if (!c || !m || !m->vertices || !m->faces) return;
  Vertex3D vs[m->metadata.vertices_count];
  carbon_drawcanvas__local_to_clip_space(c, m, t, vs);
  const CBN_Vec3 light = carbon_math_vec3_norm(CARBON_VEC3_BACK);
  for (usz f = 0; f < m->metadata.faces_count; ++f) {
    const usz *i = m->faces[f][CARBON_MESH_FACE_COMP_VERTEX];
    const Vertex3D v1 = vs[i[0]], v2 = vs[i[1]], v3 = vs[i[2]];
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(v1, v2, v3, pvs);
    carbon_drawcanvas__poly_triangulation(dc, pvs, pvs_count, light, color);
  }
}

void carbon_drawcanvas_plane_xz(CBN_DrawCanvas dc, const CBN_Camera *c, CBN_Vec3 center, CBN_Vec2 size, u32 color) {
  if (!c) return;
  size = carbon_math_vec2_scale(size, 0.5);
  Vertex3D vs[4];
  vs[0].world = CARBON_VEC3(center.x - size.x, center.y, center.z - size.y);
  vs[1].world = CARBON_VEC3(center.x + size.x, center.y, center.z - size.y);
  vs[2].world = CARBON_VEC3(center.x + size.x, center.y, center.z + size.y);
  vs[3].world = CARBON_VEC3(center.x - size.x, center.y, center.z + size.y);
  const CBN_Mat4 V = carbon_camera_get_view(c);
  const CBN_Mat4 P = carbon_camera_get_proj(c);
  const CBN_Mat4 MVP = carbon_math_mat4_mult(P, V);
  for (usz i = 0; i < CARBON_ARRAY_LEN(vs); ++i) {
    CBN_Vec4 v = CARBON_VEC4_3(vs[i].world, 1);
    vs[i].clip = carbon_math_mat4_mult_vec4(MVP, v);
  }
  const CBN_Vec3 light = carbon_math_vec3_norm(CARBON_VEC3_ONE);
  {// First triangle
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(vs[0], vs[1], vs[2], pvs);
    carbon_drawcanvas__poly_triangulation(dc, pvs, pvs_count, light, color);
  }
  {// Second triangle
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(vs[0], vs[2], vs[3], pvs);
    carbon_drawcanvas__poly_triangulation(dc, pvs, pvs_count, light, color);
  }
}

void carbon_drawcanvas_box(CBN_DrawCanvas dc, CBN_Rect r) {
#define PX(i, j, c) carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, (usz)(i), (usz)(j)), (c));
#define OUTLINE(i, j)     PX(i, j, CARBON_DRAWCANVAS__BOX_OUTLINE_COLOR)
#define TOPLEFT(i, j)     PX(i, j, CARBON_DRAWCANVAS__BOX_TOPLEFT_COLOR)
#define BOTTOMRIGHT(i, j) PX(i, j, CARBON_DRAWCANVAS__BOX_BOTTOMRIGHT_COLOR)
#define INSIDE(i, j)      PX(i, j, CARBON_DRAWCANVAS__BOX_INSIDE_COLOR)
  const usz x1 = r.x, y1 = r.y, x2 = x1 + r.w - 1, y2 = y1 + r.h - 1;
  // Header
  for (usz j = y1; j <= y1 + 1; ++j) {
    for (usz i = x1 + 4; i <= x2 - 6; ++i) {
      OUTLINE(i, j);
    }
  }
  for (usz j = y1 + 2; j <= y1 + 3; ++j) {
    OUTLINE(x1 + 2, j);
    OUTLINE(x1 + 3, j);
    for (usz i = x1 + 4; i <= x2 - 6; ++i) {
      TOPLEFT(i, j);
    }
    OUTLINE(x2 - 5, j);
    OUTLINE(x2 - 4, j);
  }
  for (usz j = y1 + 4; j <= y1 + 5; ++j) {
    OUTLINE(x1 + 0, j);
    OUTLINE(x1 + 1, j);
    for (usz i = x1 + 2; i <= x2 - 6; ++i) {
      TOPLEFT(i, j);
    }
    INSIDE(x2 - 5, j);
    INSIDE(x2 - 4, j);
    OUTLINE(x2 - 3, j);
    OUTLINE(x2 - 2, j);
  }
  for (usz j = y1 + 6; j <= y1 + 7; ++j) {
    OUTLINE(x1 + 0, j);
    OUTLINE(x1 + 1, j);
    for (usz i = x1 + 2; i <= x1 + 7; ++i) {
      TOPLEFT(i, j);
    }
    for (usz i = x1 + 8; i <= x2 - 6; ++i) {
      INSIDE(i, j);
    }
    for (usz i = x2 - 5; i <= x2 - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(x2 - 1, j);
    OUTLINE(x2 - 0, j);
  }
  // Content
  for (usz j = y1 + 8; j <= y2 - 8; ++j) {
    OUTLINE(x1 + 0, j);
    OUTLINE(x1 + 1, j);
    for (usz i = x1 + 2; i <= x1 + 5; ++i) {
      TOPLEFT(i, j);
    }
    for (usz i = x1 + 6; i <= x2 - 6; ++i) {
      INSIDE(i, j);
    }
    for (usz i = x2 - 5; i <= x2 - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(x2 - 1, j);
    OUTLINE(x2 - 0, j);
  }
  // Footer
  for (usz j = y2 - 7; j <= y2 - 6; ++j) {
    OUTLINE(x1 + 0, j);
    OUTLINE(x1 + 1, j);
    for (usz i = x1 + 2; i <= x1 + 5; ++i) {
      TOPLEFT(i, j);
    }
    for (usz i = x1 + 6; i <= x2 - 8; ++i) {
      INSIDE(i, j);
    }
    for (usz i = x2 - 7; i <= x2 - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(x2 - 1, j);
    OUTLINE(x2 - 0, j);
  }
  for (usz j = y2 - 5; j <= y2 - 4; ++j) {
    OUTLINE(x1 + 2, j);
    OUTLINE(x1 + 3, j);
    INSIDE(x1 + 4, j);
    INSIDE(x1 + 5, j);
    for (usz i = x1 + 6; i <= x2 - 2; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(x2 - 1, j);
    OUTLINE(x2 - 0, j);
  }
  for (usz j = y2 - 3; j <= y2 - 2; ++j) {
    OUTLINE(x1 + 4, j);
    OUTLINE(x1 + 5, j);
    for (usz i = x1 + 6; i <= x2 - 4; ++i) {
      BOTTOMRIGHT(i, j);
    }
    OUTLINE(x2 - 3, j);
    OUTLINE(x2 - 2, j);
  }
  for (usz j = y2 - 1; j <= y2; ++j) {
    for (usz i = x1 + 6; i <= x2 - 4; ++i) {
      OUTLINE(i, j);
    }
  }
#undef PX
#undef OUTLINE
#undef TOPLEFT
#undef BOTTOMRIGHT
#undef INSIDE
}

void carbon_drawcanvas_text(CBN_DrawCanvas dc, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  static const char *glyphs = &carbon_drawcanvas__font[0][0][0];
  for (usz i = 0; *txt; ++i, ++txt) {
    i32 gx = position.x + (i * CARBON_DRAWCANVAS__FONT_WIDTH * size);
    i32 gy = position.y;
    const char *glyph = &glyphs[(*txt) * CARBON_DRAWCANVAS__FONT_WIDTH * CARBON_DRAWCANVAS__FONT_HEIGHT];
    for (usz dy = 0; dy < CARBON_DRAWCANVAS__FONT_HEIGHT; ++dy) {
      for (usz dx = 0; dx < CARBON_DRAWCANVAS__FONT_WIDTH; ++dx) {
        i32 px = gx + dx*size;
        i32 py = gy + dy*size;
        if (0 <= px && px < (i32) dc.width && 0 <= py && py < (i32) dc.height && glyph[dy*CARBON_DRAWCANVAS__FONT_WIDTH + dx]) {
          carbon_drawcanvas_rect(dc, CARBON_RECT_SQUARE(px, py, size), color);
        }
      }
    }
  }
}

void carbon_drawcanvas_text_with_shadow(CBN_DrawCanvas dc, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  carbon_drawcanvas_text(dc, txt, CARBON_VEC2(position.x + 1 * size, position.y + 1 * size), size, 0x33333380);
  carbon_drawcanvas_text(dc, txt, position, size, color);
}

usz carbon_drawcanvas_get_text_width(const char *txt, usz size) {
  return CARBON_DRAWCANVAS__FONT_WIDTH * size * carbon_string_len(txt);
}

usz carbon_drawcanvas_get_text_height(usz size) {
  return CARBON_DRAWCANVAS__FONT_HEIGHT * size;
}
