// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_DRAWCANVAS__CIRCLE_AA          2
#define CARBON_DRAWCANVAS__MAX_LIGHTS         8

struct CBN_DrawCanvas {
  u32 *pixels;
  f32 *zbuffer;
  usz width;
  usz height;
  u32 flags;
  CBN_Light lights[CARBON_DRAWCANVAS__MAX_LIGHTS];
  usz lights_count;
};

#include "carbon_drawcanvas_monofont.inl"
#include "carbon_drawcanvas_helpers.inl"
#include "carbon_drawcanvas_st.inl"
#include "carbon_drawcanvas_mt.inl"

CBN_DrawCanvas *carbon_drawcanvas_create(usz width, usz height) {
  CBN_DrawCanvas *dc = carbon_memory_alloc(sizeof(CBN_DrawCanvas));
  dc->pixels       = carbon_memory_alloc(width * height * sizeof(u32));
  dc->zbuffer      = carbon_memory_alloc(width * height * sizeof(f32));
  dc->width        = width;
  dc->height       = height;
  dc->flags        = 0;
  dc->lights_count = 0;
  return dc;
}

void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc) {
  if (!dc) return;
  carbon_memory_free(dc->pixels);
  carbon_memory_free(dc->zbuffer);
  carbon_memory_set(dc, 0, sizeof(*dc));
}

u32 *carbon_drawcanvas_pixels(const CBN_DrawCanvas *dc) {
  return dc->pixels;
}

usz carbon_drawcanvas_width(const CBN_DrawCanvas *dc) {
  return dc->width;
}

usz carbon_drawcanvas_height(const CBN_DrawCanvas *dc) {
  return dc->height;
}

CBN_Rect carbon_drawcanvas_extent(const CBN_DrawCanvas *dc) {
  return carbon_math_rect(0, 0, dc->width, dc->height);
}

u32 carbon_drawcanvas_flags(const CBN_DrawCanvas *dc) {
  return dc->flags;
}

void carbon_drawcanvas_flags_enable(CBN_DrawCanvas *dc, u32 flags) {
  if (!dc) return;
  dc->flags |= flags;
}

void carbon_drawcanvas_flags_disable(CBN_DrawCanvas *dc, u32 flags) {
  if (!dc) return;
  dc->flags &= ~flags;
}

void carbon_drawcanvas_flags_toggle(CBN_DrawCanvas *dc, u32 flags) {
  if (!dc) return;
  dc->flags ^= flags;
}

isz carbon_drawcanvas_light_add(CBN_DrawCanvas *dc, CBN_Light l) {
  if (!dc || dc->lights_count >= CARBON_DRAWCANVAS__MAX_LIGHTS) return -1;
  l.active = true;
  dc->lights[dc->lights_count++] = l;
  return dc->lights_count;
}

void carbon_drawcanvas_light_enable(CBN_DrawCanvas *dc, isz idx) {
  if (!dc || 0 > idx || (usz)idx >= dc->lights_count) return;
  dc->lights[idx].active = true;
}

void carbon_drawcanvas_light_disable(CBN_DrawCanvas *dc, isz idx) {
  if (!dc || 0 > idx || (usz)idx >= dc->lights_count) return;
  dc->lights[idx].active = false;
}

void carbon_drawcanvas_light_toggle(CBN_DrawCanvas *dc, isz idx) {
  if (!dc || 0 > idx || (usz)idx >= dc->lights_count) return;
  dc->lights[idx].active ^= true;
}

void carbon_drawcanvas_light_set_intensity(CBN_DrawCanvas *dc, isz idx, f32 intensity) {
  if (!dc || 0 > idx || (usz)idx >= dc->lights_count) return;
  dc->lights[idx].intensity = intensity;
}

void carbon_drawcanvas_light_set_color(CBN_DrawCanvas *dc, isz idx, u32 color) {
  if (!dc || 0 > idx || (usz)idx >= dc->lights_count) return;
  dc->lights[idx].color = color;
}

void carbon_drawcanvas_clear(CBN_DrawCanvas *dc, u32 color) {
  const usz n = dc->width * dc->height;
  for (usz i = 0; i < n; ++i) {
    dc->pixels[i] = color;
    dc->zbuffer[i] = 1;
  }
}

void carbon_drawcanvas_fill(CBN_DrawCanvas *dc, u32 color) {
  const usz n = dc->width * dc->height;
  for (usz i = 0; i < n; ++i) {
    carbon_drawcanvas__alpha_blending(&dc->pixels[i], color);
    dc->zbuffer[i] = 1;
  }
}

void carbon_drawcanvas_line(CBN_DrawCanvas *dc, CBN_Vec2 v1, CBN_Vec2 v2, u32 color) {
  i32 x1 = v1.x, y1 = v1.y, x2 = v2.x, y2 = v2.y;
  i32 dx =  carbon_math_abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
  i32 dy = -carbon_math_abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
  i32 err = dx + dy;
  for (;;) {
    if (0 <= x1 && x1 < (i32) dc->width && 0 <= y1 && y1 < (i32) dc->height) {
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

void carbon_drawcanvas_triangle(CBN_DrawCanvas *dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color) {
  carbon_drawcanvas_triangle_3(dc, v1, v2, v3, color, color, color);
}

void carbon_drawcanvas_triangle_3(CBN_DrawCanvas *dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 c1, u32 c2, u32 c3) {
  CBN_Vec2 lo, hi;
  if (!carbon_drawcanvas__triangle_aabb(dc, v1, v2, v3, &lo, &hi)) return;
  for (usz j = lo.y; j <= hi.y; ++j) {
    for (usz i = lo.x; i <= hi.x; ++i) {
      CBN_Vec3 u;
      if (!carbon_math_vec2_barycentric(v1, v2, v3, carbon_math_vec2(i, j), &u)) continue;
      carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, i, j), carbon_color_lerp_3(c1, c2, c3, u));
    }
  }
}

void carbon_drawcanvas_rect(CBN_DrawCanvas *dc, CBN_Rect r, u32 color) {
  i32 x1, x2, y1, y2;
  if (!carbon_drawcanvas__rect_normalize(dc, r, &x1, &x2, &y1, &y2)) return;
  u32 *p_dc = dc->pixels + (y1 * dc->width + x1);
  usz row_offset = x2 - x1 + 1;
  for (i32 j = y1; j <= y2; ++j) {
    for (i32 i = x1; i <= x2; ++i) {
      carbon_drawcanvas__alpha_blending(p_dc, color);
      ++p_dc;
    }
    p_dc += dc->width - row_offset;
  }
}

void carbon_drawcanvas_rect_outline(CBN_DrawCanvas *dc, CBN_Rect r, usz thick, u32 color) {
  if (!thick) return;
  if (2*thick >= r.w || 2*thick >= r.h) {
    carbon_drawcanvas_rect(dc, r, color);
    return;
  }
  carbon_drawcanvas_rect(dc, carbon_math_rect(r.x, r.y, r.w, thick), color);
  carbon_drawcanvas_rect(dc, carbon_math_rect(r.x, r.y + r.h - thick, r.w, thick), color);
  carbon_drawcanvas_rect(dc, carbon_math_rect(r.x, r.y + thick, thick, r.h - 2*thick), color);
  carbon_drawcanvas_rect(dc, carbon_math_rect(r.x + r.w - thick, r.y + thick, thick, r.h - 2*thick), color);
}

void carbon_drawcanvas_circle(CBN_DrawCanvas *dc, CBN_Vec2 center, usz radius, u32 color) {
  i32 x1, x2, y1, y2;
  CBN_Rect xywh = carbon_math_rect_sq(center.x - radius, center.y - radius, 2*radius);
  if (!carbon_drawcanvas__rect_normalize(dc, xywh, &x1, &x2, &y1, &y2)) return;
  for (i32 j = y1; j <= y2; ++j) {
    for (i32 i = x1; i <= x2; ++i) {
      i64 n = 0;
      for (i64 sx = 0; sx < CARBON_DRAWCANVAS__CIRCLE_AA; ++sx) {
        for (i64 sy = 0; sy < CARBON_DRAWCANVAS__CIRCLE_AA; ++sy) {
          i64 res = CARBON_DRAWCANVAS__CIRCLE_AA;
          i64 dx = 2*i*res + 2*sx + 1 - 2*res*center.x;
          i64 dy = 2*j*res + 2*sy + 1 - 2*res*center.y;
          if (dx*dx + dy*dy <= (i64)(4 * res*res * radius*radius)) ++n;
        }
      }
      u32 aa_alpha = (color & 0xff) * n / (CARBON_DRAWCANVAS__CIRCLE_AA*CARBON_DRAWCANVAS__CIRCLE_AA);
      u32 aa_color = (color & 0xffffff00) | aa_alpha;
      carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, i, j), aa_color);
    }
  }
}

void carbon_drawcanvas_annulus(CBN_DrawCanvas *dc, CBN_Vec2 center, usz radius_out, usz radius_in, u32 color) {
  CBN_ASSERT(radius_in < radius_out);
  i32 x1, x2, y1, y2;
  CBN_Rect xywh = carbon_math_rect_sq(center.x - radius_out, center.y - radius_out, 2*radius_out);
  if (!carbon_drawcanvas__rect_normalize(dc, xywh, &x1, &x2, &y1, &y2)) return;
  for (i32 j = y1; j <= y2; ++j) {
    for (i32 i = x1; i <= x2; ++i) {
      i64 n = 0;
      for (i64 sx = 0; sx < CARBON_DRAWCANVAS__CIRCLE_AA; ++sx) {
        for (i64 sy = 0; sy < CARBON_DRAWCANVAS__CIRCLE_AA; ++sy) {
          i64 res = CARBON_DRAWCANVAS__CIRCLE_AA;
          i64 dx = 2*i*res + 2*sx + 1 - 2*res*center.x;
          i64 dy = 2*j*res + 2*sy + 1 - 2*res*center.y;
          i64 d_sq = dx*dx + dy*dy;
          if (d_sq <= (i64)(4 * res*res * radius_out*radius_out) &&
              d_sq >= (i64)(4 * res*res * radius_in*radius_in)) ++n;
        }
      }
      u32 aa_alpha = (color & 0xff) * n / (CARBON_DRAWCANVAS__CIRCLE_AA*CARBON_DRAWCANVAS__CIRCLE_AA);
      u32 aa_color = (color & 0xffffff00) | aa_alpha;
      carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, i, j), aa_color);
    }
  }
}

void carbon_drawcanvas_sprite(CBN_DrawCanvas *dc, const CBN_Sprite *s, CBN_Vec2 position, CBN_Vec2 scale, u32 tint) {
  const f32 sw = s->width * scale.x, sh = s->height * scale.y;
  const CBN_Rect r_dc = carbon_math_rect(0, 0, dc->width, dc->height);
  const CBN_Rect r_sp = carbon_math_rect(position.x, position.y, sw, sh);
  CBN_Rect xywh = carbon_math_rect_intersection(r_dc, r_sp);
  if (xywh.w <= 0 || xywh.h <= 0) return;
  u32 *p_dc = dc->pixels + (usz)(xywh.y * r_dc.w + xywh.x);
  const f32 inv_sx = 1/scale.x, inv_sy = 1/scale.y;
  const f32 start_x = (xywh.x - r_sp.x) * inv_sx;
  f32 src_y = (xywh.y - r_sp.y) * inv_sy;
  for (usz j = 0; j < xywh.h; ++j) {
    const usz y0 = (usz)src_y;
    const usz y1 = y0 + (y0 + 1 < s->height ? 1 : 0);
    const u32 *r0 = s->pixels + y0*s->width, *r1 = s->pixels + y1*s->width;
    f32 src_x = start_x;
    for (usz i = 0; i < xywh.w; ++i) {
      const usz x0 = (usz)src_x;
      const usz x1 = x0 + (x0 + 1 < s->width ? 1 : 0);
      const CBN_Vec2 t = carbon_math_vec2(src_x - x0, src_y - y0);
      u32 c = carbon_color_bilerp(r0[x0], r0[x1], r1[x0], r1[x1], t);
      c = carbon_color_mult(c, tint);
      carbon_drawcanvas__alpha_blending(p_dc, c);
      src_x += inv_sx;
      ++p_dc;
    }
    src_y += inv_sy;
    p_dc += (usz)(r_dc.w - xywh.w);
  }
}

void carbon_drawcanvas_mesh(CBN_DrawCanvas *dc, const CBN_Camera *c, const CBN_Mesh *m, CBN_Transform t, u32 color) {
  if (!c || !m || !m->vertices || !m->faces) return;
  Vertex3D vs[m->metadata.vertices_count];
  carbon_drawcanvas__local_to_clip_space(c, m, t, vs);
  const CBN_Vec3 cam_pos = carbon_camera_get_position(c);
  for (usz f = 0; f < m->metadata.faces_count; ++f) {
    const usz *i = m->faces[f][CARBON_MESH_FACE_COMP_VERTEX];
    const Vertex3D v1 = vs[i[0]], v2 = vs[i[1]], v3 = vs[i[2]];
    if (dc->flags & CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING) {
      if (carbon_drawcanvas__is_back_face(dc, cam_pos, v1.world, v2.world, v3.world)) continue;
    }
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(v1, v2, v3, pvs);
    carbon_drawcanvas__poly_triangulation(dc, pvs, pvs_count, color);
  }
}

void carbon_drawcanvas_mesh_with_texture(CBN_DrawCanvas *dc, const CBN_Camera *c, const CBN_Mesh *m, CBN_Transform t, const CBN_Sprite *s) {
  if (!c || !m || !m->vertices || !m->texcoords || !m->faces || !s || !s->pixels) return;
  Vertex3D vs[m->metadata.vertices_count];
  carbon_drawcanvas__local_to_clip_space(c, m, t, vs);
  const CBN_Vec3 cam_pos = carbon_camera_get_position(c);
  for (usz f = 0; f < m->metadata.faces_count; ++f) {
    const usz *i    = m->faces[f][CARBON_MESH_FACE_COMP_VERTEX];
    const usz *i_vt = m->faces[f][CARBON_MESH_FACE_COMP_TEXCOORD];
    Vertex3D v1 = vs[i[0]], v2 = vs[i[1]], v3 = vs[i[2]];
    v1.uv = m->texcoords[i_vt[0]];
    v2.uv = m->texcoords[i_vt[1]];
    v3.uv = m->texcoords[i_vt[2]];
    if (dc->flags & CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING) {
      if (carbon_drawcanvas__is_back_face(dc, cam_pos, v1.world, v2.world, v3.world)) continue;
    }
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(v1, v2, v3, pvs);
    carbon_drawcanvas__poly_triangulation_with_texture(dc, pvs, pvs_count, s);
  }
}

const CBN_Mesh carbon_drawcanvas_tetrahedron = {
  .metadata = (CBN_Mesh_Metadata){
    .vertices_count  = 4,
    .texcoords_count = 4*3,  // 3 corners per face (triangle)
    .normals_count   = 4,
    .faces_count     = 4     // 4 triangular faces
  },
  .vertices = (CBN_Vec3[]){
    {{{ 1,  1,  1}}},  // [0]
    {{{-1, -1,  1}}},  // [1]
    {{{-1,  1, -1}}},  // [2]
    {{{ 1, -1, -1}}}   // [3]
  },
  .texcoords = (CBN_Vec2[]){
    {{{1/2.,    0}}}, {{{1/4., 1/4.}}}, {{{3/4., 1/4.}}},  // [0..2]  top
    {{{1/4., 1/4.}}}, {{{   0, 1/2.}}}, {{{1/2., 1/2.}}},  // [3..5]  left
    {{{3/4., 1/4.}}}, {{{1/2., 1/2.}}}, {{{   1, 1/2.}}},  // [6..8]  right
    {{{1/2., 1/2.}}}, {{{1/4., 3/4.}}}, {{{3/4., 3/4.}}}   // [9..11] bottom
  },
  .normals = (CBN_Vec3[]){
    {{{ CARBON_1_SQRT3, -CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [0] top
    {{{-CARBON_1_SQRT3,  CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [1] left
    {{{ CARBON_1_SQRT3,  CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [2] right
    {{{-CARBON_1_SQRT3, -CARBON_1_SQRT3, -CARBON_1_SQRT3}}}   // [3] bottom
  },
  .faces = (usz[][CARBON_MESH_FACE_COMPS][3]){
    {{0, 1, 3}, {0,  1,  2}, {0, 0, 0}},  // top
    {{0, 2, 1}, {3,  4,  5}, {1, 1, 1}},  // left
    {{0, 3, 2}, {6,  7,  8}, {2, 2, 2}},  // right
    {{1, 2, 3}, {9, 10, 11}, {3, 3, 3}}   // bottom
  }
};

const CBN_Mesh carbon_drawcanvas_cube = {
  .metadata = (CBN_Mesh_Metadata){
    .vertices_count  = 8,
    .texcoords_count = 6*4,  // 4 corners per face (square)
    .normals_count   = 6,
    .faces_count     = 6*2   // 2 triangles per face
  },
  .vertices = (CBN_Vec3[]){
    {{{-1, -1,  1}}},  // [0] front bottom-left
    {{{ 1, -1,  1}}},  // [1] front bottom-right
    {{{ 1,  1,  1}}},  // [2] front top-right
    {{{-1,  1,  1}}},  // [3] front top-left
    {{{-1, -1, -1}}},  // [4] back bottom-left
    {{{ 1, -1, -1}}},  // [5] back bottom-right
    {{{ 1,  1, -1}}},  // [6] back top-right
    {{{-1,  1, -1}}}   // [7] back top-left
  },
  .texcoords = (CBN_Vec2[]){
    {{{1/4., 1/3.}}}, {{{2/4., 1/3.}}}, {{{2/4., 2/3.}}}, {{{1/4., 2/3.}}},  // [0..3]   front
    {{{2/4., 1/3.}}}, {{{3/4., 1/3.}}}, {{{3/4., 2/3.}}}, {{{2/4., 2/3.}}},  // [4..7]   right
    {{{3/4., 1/3.}}}, {{{4/4., 1/3.}}}, {{{4/4., 2/3.}}}, {{{3/4., 2/3.}}},  // [8..11]  back
    {{{   0, 1/3.}}}, {{{1/4., 1/3.}}}, {{{1/4., 2/3.}}}, {{{   0, 2/3.}}},  // [12..15] left
    {{{1/4., 2/3.}}}, {{{2/4., 2/3.}}}, {{{2/4., 3/3.}}}, {{{1/4., 3/3.}}},  // [16..19] top
    {{{1/4.,    0}}}, {{{2/4.,    0}}}, {{{2/4., 1/3.}}}, {{{1/4., 1/3.}}}   // [20..23] bottom
  },
  .normals = (CBN_Vec3[]){
    {{{ 0,  0,  1}}},  // [0] front
    {{{ 1,  0,  0}}},  // [1] right
    {{{ 0,  0, -1}}},  // [2] back
    {{{-1,  0,  0}}},  // [3] left
    {{{ 0,  1,  0}}},  // [4] top
    {{{ 0, -1,  0}}}   // [5] bottom
  },
  .faces = (usz[][CARBON_MESH_FACE_COMPS][3]){
    {{0, 1, 2}, { 0,  1,  2}, {0, 0, 0}}, {{0, 2, 3}, { 0,  2,  3}, {0, 0, 0}},  // front
    {{1, 5, 6}, { 4,  5,  6}, {1, 1, 1}}, {{1, 6, 2}, { 4,  6,  7}, {1, 1, 1}},  // right
    {{5, 4, 7}, { 8,  9, 10}, {2, 2, 2}}, {{5, 7, 6}, { 8, 10, 11}, {2, 2, 2}},  // back
    {{4, 0, 3}, {12, 13, 14}, {3, 3, 3}}, {{4, 3, 7}, {12, 14, 15}, {3, 3, 3}},  // left
    {{3, 2, 6}, {16, 17, 18}, {4, 4, 4}}, {{3, 6, 7}, {16, 18, 19}, {4, 4, 4}},  // top
    {{4, 5, 1}, {20, 21, 22}, {5, 5, 5}}, {{4, 1, 0}, {20, 22, 23}, {5, 5, 5}}   // bottom
  }
};

const CBN_Mesh carbon_drawcanvas_octahedron = {
  .metadata = (CBN_Mesh_Metadata){
    .vertices_count  = 6,
    .texcoords_count = 8*3,  // 3 corners per face (triangle)
    .normals_count   = 8,
    .faces_count     = 8     // 8 triangular faces
  },
  .vertices = (CBN_Vec3[]){
    {{{ 0,  1,  0}}},  // [0] top
    {{{ 0, -1,  0}}},  // [1] bottom
    {{{ 1,  0,  0}}},  // [2] right
    {{{-1,  0,  0}}},  // [3] left
    {{{ 0,  0,  1}}},  // [4] front
    {{{ 0,  0, -1}}}   // [5] back
  },
  .texcoords = (CBN_Vec2[]){
    {{{1/8., 0}}}, {{{   0, 1/4.}}}, {{{1/4., 1/4.}}},  // [0..2]   top-front-right
    {{{3/8., 0}}}, {{{1/4., 1/4.}}}, {{{1/2., 1/4.}}},  // [3..5]   top-right-back
    {{{5/8., 0}}}, {{{1/2., 1/4.}}}, {{{3/4., 1/4.}}},  // [6..8]   top-back-left
    {{{7/8., 0}}}, {{{3/4., 1/4.}}}, {{{   1, 1/4.}}},  // [9..11]  top-left-front
    {{{1/8., 1}}}, {{{   0, 3/4.}}}, {{{1/4., 3/4.}}},  // [12..14] bottom-right-front
    {{{3/8., 1}}}, {{{1/4., 3/4.}}}, {{{1/2., 3/4.}}},  // [15..17] bottom-back-right
    {{{5/8., 1}}}, {{{1/2., 3/4.}}}, {{{3/4., 3/4.}}},  // [18..20] bottom-left-back
    {{{7/8., 1}}}, {{{3/4., 3/4.}}}, {{{   1, 3/4.}}}   // [21..23] bottom-front-left
  },
  .normals = (CBN_Vec3[]){
    {{{ CARBON_1_SQRT3,  CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [0]
    {{{ CARBON_1_SQRT3,  CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [1]
    {{{-CARBON_1_SQRT3,  CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [2]
    {{{-CARBON_1_SQRT3,  CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [3]
    {{{ CARBON_1_SQRT3, -CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [4]
    {{{ CARBON_1_SQRT3, -CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [5]
    {{{-CARBON_1_SQRT3, -CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [6]
    {{{-CARBON_1_SQRT3, -CARBON_1_SQRT3,  CARBON_1_SQRT3}}}   // [7]
  },
  .faces = (usz[][CARBON_MESH_FACE_COMPS][3]){
    {{0, 4, 2}, { 0,  1,  2}, {0, 0, 0}},  // top-front-right
    {{0, 2, 5}, { 3,  4,  5}, {1, 1, 1}},  // top-right-back
    {{0, 5, 3}, { 6,  7,  8}, {2, 2, 2}},  // top-back-left
    {{0, 3, 4}, { 9, 10, 11}, {3, 3, 3}},  // top-left-front
    {{1, 2, 4}, {12, 13, 14}, {4, 4, 4}},  // bottom-right-front
    {{1, 5, 2}, {15, 16, 17}, {5, 5, 5}},  // bottom-back-right
    {{1, 3, 5}, {18, 19, 20}, {6, 6, 6}},  // bottom-left-back
    {{1, 4, 3}, {21, 22, 23}, {7, 7, 7}}   // bottom-front-left
  }
};

#define PHI_C1  0.52573111211913360602  // `1/√(ϕ + 2)`
#define PHI_C2  0.85065080835203993218  // `ϕ/√(ϕ + 2)`
#define PHI_C3  0.27639320225002103035  // `1/(ϕ + 2)`
#define PHI_C4  0.72360679774997896964  // `(ϕ + 2)/5`
#define PHI_C5  1.10557280900008412143  // `4/(ϕ + 2)`
#define PHI_C6  0.89442719099991587856  // `2ϕ/(ϕ + 2)`
#define PHI_C7  0.34164078649987381784  // `2/(3ϕ + 1)`
#define PHI_C8  0.64983939246581265231  // `2/(ϕ√(ϕ + 2))`
#define PHI_C9  1.05146222423826721205  // `2/√(ϕ + 2)`
#define PHI_C10 0.95105651629515357211  // `√(ϕ + 2)/2`
#define PHI_C11 0.58778525229247312916  // `√(3 - ϕ)/2`
#define PHI_C12 0.35682208977308993194  // `1/(ϕ√3)`
#define PHI_C13 0.93417235896271569645  // `ϕ/√3`

#define D10_Z_SCALE 0.55
#define D10_H_OFF (0.21114561800016824287 * D10_Z_SCALE)  // `2 - 4/√5`
#define D10_C1 0.8272
#define D10_C2 0.67267

#define UVS_FACE(cx, cy, sx, sy)                \
  {{{(cx), (cy) + (sy)}}},                      \
  {{{(cx) + (sx), (cy)}}},                      \
  {{{(cx), (cy) - (sy)}}},                      \
  {{{(cx) - (sx), (cy)}}}

const CBN_Mesh carbon_drawcanvas_pentatrapezohedron = {
  .metadata = (CBN_Mesh_Metadata){
    .vertices_count  = 12,
    .texcoords_count = 10*4,  // 4 corners per face (quadrilateral)
    .normals_count   = 10,
    .faces_count     = 10*2   // 2 triangles per face
  },
  .vertices = (CBN_Vec3[]){
    {{{      0,       0,  2 * D10_Z_SCALE}}},  // [0]  top pole
    {{{      0,       0, -2 * D10_Z_SCALE}}},  // [1]  bottom pole
    {{{ PHI_C5,       0,        D10_H_OFF}}},  // [2]  belt 01/10
    {{{ PHI_C6,  PHI_C8,       -D10_H_OFF}}},  // [3]  belt 02/10
    {{{ PHI_C7,  PHI_C9,        D10_H_OFF}}},  // [4]  belt 03/10
    {{{-PHI_C7,  PHI_C9,       -D10_H_OFF}}},  // [5]  belt 04/10
    {{{-PHI_C6,  PHI_C8,        D10_H_OFF}}},  // [6]  belt 05/10
    {{{-PHI_C5,       0,       -D10_H_OFF}}},  // [7]  belt 06/10
    {{{-PHI_C6, -PHI_C8,        D10_H_OFF}}},  // [8]  belt 07/10
    {{{-PHI_C7, -PHI_C9,       -D10_H_OFF}}},  // [9]  belt 08/10
    {{{ PHI_C7, -PHI_C9,        D10_H_OFF}}},  // [10] belt 09/10
    {{{ PHI_C6, -PHI_C8,       -D10_H_OFF}}}   // [11] belt 10/10
  },
  .texcoords = (CBN_Vec2[]){
    UVS_FACE(1/10., 0.78, 0.075, 0.14),
    UVS_FACE(3/10., 0.78, 0.075, 0.14),
    UVS_FACE( 1/2., 0.78, 0.075, 0.14),
    UVS_FACE(7/10., 0.78, 0.075, 0.14),
    UVS_FACE(9/10., 0.78, 0.075, 0.14),
    UVS_FACE(1/10., 0.22, 0.075, 0.14),
    UVS_FACE(3/10., 0.22, 0.075, 0.14),
    UVS_FACE( 1/2., 0.22, 0.075, 0.14),
    UVS_FACE(7/10., 0.22, 0.075, 0.14),
    UVS_FACE(9/10., 0.22, 0.075, 0.14)
  },
  .normals = (CBN_Vec3[]){
    {{{ PHI_C4 * D10_C1,  PHI_C1 * D10_C1,  D10_C2}}},  // [0]
    {{{-PHI_C3 * D10_C1,  PHI_C2 * D10_C1,  D10_C2}}},  // [1]
    {{{-PHI_C6 * D10_C1,                0,  D10_C2}}},  // [2]
    {{{-PHI_C3 * D10_C1, -PHI_C2 * D10_C1,  D10_C2}}},  // [3]
    {{{ PHI_C4 * D10_C1, -PHI_C1 * D10_C1,  D10_C2}}},  // [4]
    {{{-PHI_C4 * D10_C1, -PHI_C1 * D10_C1, -D10_C2}}},  // [5]
    {{{ PHI_C3 * D10_C1, -PHI_C2 * D10_C1, -D10_C2}}},  // [6]
    {{{ PHI_C6 * D10_C1,                0, -D10_C2}}},  // [7]
    {{{ PHI_C3 * D10_C1,  PHI_C2 * D10_C1, -D10_C2}}},  // [8]
    {{{-PHI_C4 * D10_C1,  PHI_C1 * D10_C1, -D10_C2}}}   // [9]
  },
  .faces = (usz[][CARBON_MESH_FACE_COMPS][3]){
    {{0,  2,  3}, { 0,  1,  2}, {0, 0, 0}}, {{0,  3,  4}, { 0,  2,  3}, {0, 0, 0}},
    {{0,  4,  5}, { 4,  5,  6}, {1, 1, 1}}, {{0,  5,  6}, { 4,  6,  7}, {1, 1, 1}},
    {{0,  6,  7}, { 8,  9, 10}, {2, 2, 2}}, {{0,  7,  8}, { 8, 10, 11}, {2, 2, 2}},
    {{0,  8,  9}, {12, 13, 14}, {3, 3, 3}}, {{0,  9, 10}, {12, 14, 15}, {3, 3, 3}},
    {{0, 10, 11}, {16, 17, 18}, {4, 4, 4}}, {{0, 11,  2}, {16, 18, 19}, {4, 4, 4}},
    {{1,  8,  7}, {20, 22, 21}, {5, 5, 5}}, {{1,  9,  8}, {20, 23, 22}, {5, 5, 5}},
    {{1, 10,  9}, {24, 26, 25}, {6, 6, 6}}, {{1, 11, 10}, {24, 27, 26}, {6, 6, 6}},
    {{1,  2, 11}, {28, 30, 29}, {7, 7, 7}}, {{1,  3,  2}, {28, 31, 30}, {7, 7, 7}},
    {{1,  4,  3}, {32, 34, 33}, {8, 8, 8}}, {{1,  5,  4}, {32, 35, 34}, {8, 8, 8}},
    {{1,  6,  5}, {36, 38, 37}, {9, 9, 9}}, {{1,  7,  6}, {36, 39, 38}, {9, 9, 9}}
  }
};

#undef UVS_FACE
#define UVS_FACE(cx, cy, sx, sy)                            \
  {{{(cx), (cy) + (sy)}}},                                  \
  {{{(cx) - PHI_C10 * (sx), (cy) + CARBON_1_2PHI * (sy)}}}, \
  {{{(cx) - PHI_C11 * (sx), (cy) - CARBON_PHI_2 * (sy)}}},  \
  {{{(cx) + PHI_C11 * (sx), (cy) - CARBON_PHI_2 * (sy)}}},  \
  {{{(cx) + PHI_C10 * (sx), (cy) + CARBON_1_2PHI * (sy)}}}

const CBN_Mesh carbon_drawcanvas_dodecahedron = {
  .metadata = (CBN_Mesh_Metadata){
    .vertices_count  = 20,
    .texcoords_count = 12*5,  // 5 corners per face (pentagon)
    .normals_count   = 12,
    .faces_count     = 12*3   // 3 triangles per face
  },
  .vertices = (CBN_Vec3[]){
    {{{           -1,            -1,          -1}}},  // [0]
    {{{           -1,            -1,           1}}},  // [1]
    {{{           -1,             1,          -1}}},  // [2]
    {{{           -1,             1,           1}}},  // [3]
    {{{            1,            -1,          -1}}},  // [4]
    {{{            1,            -1,           1}}},  // [5]
    {{{            1,             1,          -1}}},  // [6]
    {{{            1,             1,           1}}},  // [7]
    {{{            0, -CARBON_1_PHI,  CARBON_PHI}}},  // [8]
    {{{            0, -CARBON_1_PHI, -CARBON_PHI}}},  // [9]
    {{{            0,  CARBON_1_PHI,  CARBON_PHI}}},  // [10]
    {{{            0,  CARBON_1_PHI, -CARBON_PHI}}},  // [11]
    {{{-CARBON_1_PHI,  CARBON_PHI,             0}}},  // [12]
    {{{ CARBON_1_PHI,  CARBON_PHI,             0}}},  // [13]
    {{{-CARBON_1_PHI, -CARBON_PHI,             0}}},  // [14]
    {{{ CARBON_1_PHI, -CARBON_PHI,             0}}},  // [15]
    {{{  -CARBON_PHI,           0, -CARBON_1_PHI}}},  // [16]
    {{{  -CARBON_PHI,           0,  CARBON_1_PHI}}},  // [17]
    {{{   CARBON_PHI,           0, -CARBON_1_PHI}}},  // [18]
    {{{   CARBON_PHI,           0,  CARBON_1_PHI}}}   // [19]
  },
  .texcoords = (CBN_Vec2[]){
    UVS_FACE(1/8., 5/6., 0.095, 0.115),
    UVS_FACE(3/8., 5/6., 0.095, 0.115),
    UVS_FACE(5/8., 5/6., 0.095, 0.115),
    UVS_FACE(7/8., 5/6., 0.095, 0.115),
    UVS_FACE(1/8., 1/2., 0.095, 0.115),
    UVS_FACE(3/8., 1/2., 0.095, 0.115),
    UVS_FACE(5/8., 1/2., 0.095, 0.115),
    UVS_FACE(7/8., 1/2., 0.095, 0.115),
    UVS_FACE(1/8., 1/6., 0.095, 0.115),
    UVS_FACE(3/8., 1/6., 0.095, 0.115),
    UVS_FACE(5/8., 1/6., 0.095, 0.115),
    UVS_FACE(7/8., 1/6., 0.095, 0.115)
  },
  .normals = (CBN_Vec3[]){
    {{{      0, -PHI_C2,  PHI_C1}}},  // [0]
    {{{      0, -PHI_C2, -PHI_C1}}},  // [1]
    {{{-PHI_C2, -PHI_C1,       0}}},  // [2]
    {{{-PHI_C1,       0, -PHI_C2}}},  // [3]
    {{{ PHI_C1,       0, -PHI_C2}}},  // [4]
    {{{ PHI_C2, -PHI_C1,       0}}},  // [5]
    {{{      0,  PHI_C2, -PHI_C1}}},  // [6]
    {{{-PHI_C2,  PHI_C1,       0}}},  // [7]
    {{{ PHI_C2,  PHI_C1,       0}}},  // [8]
    {{{-PHI_C1,       0,  PHI_C2}}},  // [9]
    {{{ PHI_C1,       0,  PHI_C2}}},  // [10]
    {{{      0,  PHI_C2,  PHI_C1}}}   // [11]
  },
  .faces = (usz[][CARBON_MESH_FACE_COMPS][3]){
    {{15,  5,  8}, { 0,  1,  2}, { 0,  0,  0}}, {{15,  8,  1}, { 0,  2,  3}, { 0,  0,  0}}, {{15,  1, 14}, { 0,  3,  4}, { 0,  0,  0}},
    {{ 4, 15, 14}, { 5,  6,  7}, { 1,  1,  1}}, {{ 4, 14,  0}, { 5,  7,  8}, { 1,  1,  1}}, {{ 4,  0,  9}, { 5,  8,  9}, { 1,  1,  1}},
    {{ 0, 14,  1}, {10, 11, 12}, { 2,  2,  2}}, {{ 0,  1, 17}, {10, 12, 13}, { 2,  2,  2}}, {{ 0, 17, 16}, {10, 13, 14}, { 2,  2,  2}},
    {{11,  9,  0}, {15, 16, 17}, { 3,  3,  3}}, {{11,  0, 16}, {15, 17, 18}, { 3,  3,  3}}, {{11, 16,  2}, {15, 18, 19}, { 3,  3,  3}},
    {{ 6, 18,  4}, {20, 21, 22}, { 4,  4,  4}}, {{ 6,  4,  9}, {20, 22, 23}, { 4,  4,  4}}, {{ 6,  9, 11}, {20, 23, 24}, { 4,  4,  4}},
    {{18, 19,  5}, {25, 26, 27}, { 5,  5,  5}}, {{18,  5, 15}, {25, 27, 28}, { 5,  5,  5}}, {{18, 15,  4}, {25, 28, 29}, { 5,  5,  5}},
    {{13,  6, 11}, {30, 31, 32}, { 6,  6,  6}}, {{13, 11,  2}, {30, 32, 33}, { 6,  6,  6}}, {{13,  2, 12}, {30, 33, 34}, { 6,  6,  6}},
    {{ 3, 12,  2}, {35, 36, 37}, { 7,  7,  7}}, {{ 3,  2, 16}, {35, 37, 38}, { 7,  7,  7}}, {{ 3, 16, 17}, {35, 38, 39}, { 7,  7,  7}},
    {{19, 18,  6}, {40, 41, 42}, { 8,  8,  8}}, {{19,  6, 13}, {40, 42, 43}, { 8,  8,  8}}, {{19, 13,  7}, {40, 43, 44}, { 8,  8,  8}},
    {{ 8, 10,  3}, {45, 46, 47}, { 9,  9,  9}}, {{ 8,  3, 17}, {45, 47, 48}, { 9,  9,  9}}, {{ 8, 17,  1}, {45, 48, 49}, { 9,  9,  9}},
    {{ 5, 19,  7}, {50, 51, 52}, {10, 10, 10}}, {{ 5,  7, 10}, {50, 52, 53}, {10, 10, 10}}, {{ 5, 10,  8}, {50, 53, 54}, {10, 10, 10}},
    {{10,  7, 13}, {55, 56, 57}, {11, 11, 11}}, {{10, 13, 12}, {55, 57, 58}, {11, 11, 11}}, {{10, 12,  3}, {55, 58, 59}, {11, 11, 11}}
  }
};

#undef UVS_FACE
#define UVS_FACE(cx, cy, sx, sy)                \
  {{{(cx), (cy) - (sy)}}},                      \
  {{{(cx) - (sx), (cy) + (sy)}}},               \
  {{{(cx) + (sx), (cy) + (sy)}}}

const CBN_Mesh carbon_drawcanvas_icosahedron = {
  .metadata = (CBN_Mesh_Metadata){
    .vertices_count  = 12,
    .texcoords_count = 20*3,  // 3 corners per face (triangle)
    .normals_count   = 20,
    .faces_count     = 20   // 20 triangular faces
  },
  .vertices = (CBN_Vec3[]){
    {{{          0,           1,  CARBON_PHI}}},  // [0]
    {{{          0,          -1,  CARBON_PHI}}},  // [1]
    {{{          0,           1, -CARBON_PHI}}},  // [2]
    {{{          0,          -1, -CARBON_PHI}}},  // [3]
    {{{          1,  CARBON_PHI,           0}}},  // [4]
    {{{         -1,  CARBON_PHI,           0}}},  // [5]
    {{{          1, -CARBON_PHI,           0}}},  // [6]
    {{{         -1, -CARBON_PHI,           0}}},  // [7]
    {{{ CARBON_PHI,           0,           1}}},  // [8]
    {{{-CARBON_PHI,           0,           1}}},  // [9]
    {{{ CARBON_PHI,           0,          -1}}},  // [10]
    {{{-CARBON_PHI,           0,          -1}}}   // [11]
  },
  .texcoords = (CBN_Vec2[]){
    UVS_FACE(1/10., 1/8., 0.08, 0.1),  // [0]
    UVS_FACE(3/10., 1/8., 0.08, 0.1),  // [1]
    UVS_FACE(5/10., 1/8., 0.08, 0.1),  // [2]
    UVS_FACE(7/10., 1/8., 0.08, 0.1),  // [3]
    UVS_FACE(9/10., 1/8., 0.08, 0.1),  // [4]
    UVS_FACE(1/10., 3/8., 0.08, 0.1),  // [5]
    UVS_FACE(3/10., 3/8., 0.08, 0.1),  // [6]
    UVS_FACE(5/10., 3/8., 0.08, 0.1),  // [7]
    UVS_FACE(7/10., 3/8., 0.08, 0.1),  // [8]
    UVS_FACE(9/10., 3/8., 0.08, 0.1),  // [9]
    UVS_FACE(1/10., 5/8., 0.08, 0.1),  // [10]
    UVS_FACE(3/10., 5/8., 0.08, 0.1),  // [11]
    UVS_FACE(5/10., 5/8., 0.08, 0.1),  // [12]
    UVS_FACE(7/10., 5/8., 0.08, 0.1),  // [13]
    UVS_FACE(9/10., 5/8., 0.08, 0.1),  // [14]
    UVS_FACE(1/10., 7/8., 0.08, 0.1),  // [15]
    UVS_FACE(3/10., 7/8., 0.08, 0.1),  // [16]
    UVS_FACE(5/10., 7/8., 0.08, 0.1),  // [17]
    UVS_FACE(7/10., 7/8., 0.08, 0.1),  // [18]
    UVS_FACE(9/10., 7/8., 0.08, 0.1)   // [19]
  },
  .normals = (CBN_Vec3[]){
    {{{ CARBON_1_SQRT3,  CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [0]
    {{{              0,         PHI_C13,         PHI_C12}}},  // [1]
    {{{-CARBON_1_SQRT3,  CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [2]
    {{{       -PHI_C12,               0,         PHI_C13}}},  // [3]
    {{{        PHI_C12,               0,         PHI_C13}}},  // [4]
    {{{ CARBON_1_SQRT3, -CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [5]
    {{{              0,        -PHI_C13,         PHI_C12}}},  // [6]
    {{{-CARBON_1_SQRT3, -CARBON_1_SQRT3,  CARBON_1_SQRT3}}},  // [7]
    {{{              0,         PHI_C13,        -PHI_C12}}},  // [8]
    {{{        PHI_C13,         PHI_C12,               0}}},  // [9]
    {{{ CARBON_1_SQRT3,  CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [10]
    {{{       -PHI_C13,         PHI_C12,               0}}},  // [11]
    {{{-CARBON_1_SQRT3,  CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [12]
    {{{        PHI_C13,        -PHI_C12,               0}}},  // [13]
    {{{       -PHI_C13,        -PHI_C12,               0}}},  // [14]
    {{{-CARBON_1_SQRT3, -CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [15]
    {{{              0,        -PHI_C13,        -PHI_C12}}},  // [16]
    {{{ CARBON_1_SQRT3, -CARBON_1_SQRT3, -CARBON_1_SQRT3}}},  // [17]
    {{{        PHI_C12,               0,        -PHI_C13}}},  // [18]
    {{{       -PHI_C12,               0,        -PHI_C13}}}   // [19]
  },
  .faces = (usz[][CARBON_MESH_FACE_COMPS][3]){
    {{ 0,  8,  4}, { 0,  1,  2}, { 0,  0,  0}},  // T1
    {{ 0,  4,  5}, { 3,  4,  5}, { 1,  1,  1}},  // T2
    {{ 0,  5,  9}, { 6,  7,  8}, { 2,  2,  2}},  // T3
    {{ 0,  9,  1}, { 9, 10, 11}, { 3,  3,  3}},  // T4
    {{ 0,  1,  8}, {12, 13, 14}, { 4,  4,  4}},  // T5
    {{ 1,  6,  8}, {15, 16, 17}, { 5,  5,  5}},  // M1
    {{ 1,  7,  6}, {18, 19, 20}, { 6,  6,  6}},  // M2
    {{ 1,  9,  7}, {21, 22, 23}, { 7,  7,  7}},  // M3
    {{ 4,  2,  5}, {24, 25, 26}, { 8,  8,  8}},  // M4
    {{ 4,  8, 10}, {27, 28, 29}, { 9,  9,  9}},  // M5
    {{ 4, 10,  2}, {30, 31, 32}, {10, 10, 10}},  // M6
    {{ 5, 11,  9}, {33, 34, 35}, {11, 11, 11}},  // M7
    {{ 5,  2, 11}, {36, 37, 38}, {12, 12, 12}},  // M8
    {{ 8,  6, 10}, {39, 40, 41}, {13, 13, 13}},  // M9
    {{ 9, 11,  7}, {42, 43, 44}, {14, 14, 14}},  // M10
    {{ 3,  7, 11}, {45, 46, 47}, {15, 15, 15}},  // B1
    {{ 3,  6,  7}, {48, 49, 50}, {16, 16, 16}},  // B2
    {{ 3, 10,  6}, {51, 52, 53}, {17, 17, 17}},  // B3
    {{ 3,  2, 10}, {54, 55, 56}, {18, 18, 18}},  // B4
    {{ 3, 11,  2}, {57, 58, 59}, {19, 19, 19}}   // B5
  }
};

#undef UVS_FACE

void carbon_drawcanvas_plane_xz(CBN_DrawCanvas *dc, const CBN_Camera *c, CBN_Vec3 center, CBN_Vec2 size, u32 color) {
  if (!c) return;
  size = carbon_math_vec2_scale(size, 0.5);
  Vertex3D vs[4];
  vs[0].world = carbon_math_vec3(center.x - size.x, center.y, center.z - size.y);
  vs[1].world = carbon_math_vec3(center.x + size.x, center.y, center.z - size.y);
  vs[2].world = carbon_math_vec3(center.x + size.x, center.y, center.z + size.y);
  vs[3].world = carbon_math_vec3(center.x - size.x, center.y, center.z + size.y);
  const CBN_Mat4 V = carbon_camera_get_view(c);
  const CBN_Mat4 P = carbon_camera_get_proj(c);
  const CBN_Mat4 VP = carbon_math_mat4_mult(P, V);
  for (usz i = 0; i < CARBON_ARRAY_LEN(vs); ++i) {
    CBN_Vec4 v = carbon_math_vec4_3(vs[i].world, 1);
    vs[i].clip = carbon_math_mat4_mult_vec4(VP, v);
  }
  {// First triangle
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(vs[0], vs[1], vs[2], pvs);
    carbon_drawcanvas__poly_triangulation(dc, pvs, pvs_count, color);
  }
  {// Second triangle
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(vs[0], vs[2], vs[3], pvs);
    carbon_drawcanvas__poly_triangulation(dc, pvs, pvs_count, color);
  }
}

void carbon_drawcanvas_text(CBN_DrawCanvas *dc, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  static const char *glyphs = &carbon_drawcanvas__monofont[0][0][0];
  for (usz i = 0; *txt; ++i, ++txt) {
    i32 gx = position.x + (i * CARBON_DRAWCANVAS__MONOFONT_WIDTH * size);
    i32 gy = position.y;
    const char *glyph = &glyphs[(*txt) * CARBON_DRAWCANVAS__MONOFONT_WIDTH * CARBON_DRAWCANVAS__MONOFONT_HEIGHT];
    for (usz dy = 0; dy < CARBON_DRAWCANVAS__MONOFONT_HEIGHT; ++dy) {
      for (usz dx = 0; dx < CARBON_DRAWCANVAS__MONOFONT_WIDTH; ++dx) {
        i32 px = gx + dx*size;
        i32 py = gy + dy*size;
        if (0 <= px && px < (i32)dc->width && 0 <= py && py < (i32)dc->height && glyph[dy*CARBON_DRAWCANVAS__MONOFONT_WIDTH + dx]) {
          carbon_drawcanvas_rect(dc, carbon_math_rect_sq(px, py, size), color);
        }
      }
    }
  }
}

void carbon_drawcanvas_text_with_shadow(CBN_DrawCanvas *dc, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  carbon_drawcanvas_text(dc, txt, carbon_math_vec2(position.x + 1 * size, position.y + 1 * size), size, 0x33333380);
  carbon_drawcanvas_text(dc, txt, position, size, color);
}

f32 carbon_drawcanvas_get_text_width(const char *txt, usz size) {
  return CARBON_DRAWCANVAS__MONOFONT_WIDTH * size * carbon_string_len(txt);
}

f32 carbon_drawcanvas_get_text_height(usz size) {
  return CARBON_DRAWCANVAS__MONOFONT_HEIGHT * size;
}

void carbon_drawcanvas_text_with_font(CBN_DrawCanvas *dc, const CBN_Font *f, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  const f32 sf = (0 < size && size <= f->metadata.size) ? (f32)size/(f32)f->metadata.size : 1;
  const u32 c_rgb = color & 0xffffff00, c_a = color & 0x000000ff;
  const f32 edge = (f32)f->metadata.sdf_onedge/0xff, aa = f->metadata.sdf_pxscale/0xff/2;
  for (; *txt; ++txt) {
    usz idx = *txt - CARBON_FONT_ASCII_START;
    CBN_Font_Chardata cdata = f->metadata.cdata[idx];
    f32 src_w = cdata.x1 - cdata.x0, src_h = cdata.y1 - cdata.y0;
    f32 dst_w = sf*src_w, dst_h = sf*src_h;
    for (f32 dy = 0; dy < dst_h; ++dy) {
      for (f32 dx = 0; dx < dst_w; ++dx) {
        i32 px = carbon_math_round(position.x + dx + sf*cdata.xoff);
        i32 py = carbon_math_round(position.y + dy + sf*cdata.yoff);
        if (0 <= px && px < (i32)dc->width && 0 <= py && py < (i32)dc->height) {
          usz sx = cdata.x0 + dx/sf, sy = cdata.y0 + dy/sf;
          f32 val = (f32)f->data[sy*CARBON_FONT_DATA_SIZE + sx]/0xff;
          f32 fill = carbon_math_clamp((val - (edge - aa))/(2*aa), 0, 1);
          if (fill <= 0) continue;
          carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, px, py), c_rgb | (u32)(fill * c_a));
        }
      }
    }
    position.x += sf * cdata.xadvance;
  }
}

void carbon_drawcanvas_text_with_font_with_outline(CBN_DrawCanvas *dc, const CBN_Font *f, const char *txt, CBN_Vec2 position, usz size, u32 color) {
  const f32 sf = (0 < size && size <= f->metadata.size) ? (f32)size/(f32)f->metadata.size : 1;
  const u32 c_rgb = color & 0xffffff00, c_a = color & 0x000000ff;
  const f32 edge = (f32)f->metadata.sdf_onedge/0xff, pxn = f->metadata.sdf_pxscale/0xff;
  const f32 aa = pxn/2, delta = (f32)f->metadata.sdf_padding * pxn;
  for (; *txt; ++txt) {
    usz idx = *txt - CARBON_FONT_ASCII_START;
    CBN_Font_Chardata cdata = f->metadata.cdata[idx];
    f32 src_w = cdata.x1 - cdata.x0, src_h = cdata.y1 - cdata.y0;
    f32 dst_w = sf*src_w, dst_h = sf*src_h;
    for (f32 dy = 0; dy < dst_h; ++dy) {
      for (f32 dx = 0; dx < dst_w; ++dx) {
        i32 px = carbon_math_round(position.x + dx + sf*cdata.xoff);
        i32 py = carbon_math_round(position.y + dy + sf*cdata.yoff);
        if (0 <= px && px < (i32)dc->width && 0 <= py && py < (i32)dc->height) {
          usz sx = cdata.x0 + dx/sf, sy = cdata.y0 + dy/sf;
          f32 val = (f32)f->data[sy*CARBON_FONT_DATA_SIZE + sx]/0xff;
          f32 fill = carbon_math_clamp((val - (edge - aa))/(2*aa), 0, 1);
          f32 outline = carbon_math_clamp((val - (edge - delta - aa))/(2*aa), 0, 1);
          if (outline > 0) carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, px, py), 0x33333300 | (u32)(outline * c_a));
          if (fill    > 0) carbon_drawcanvas__alpha_blending(&carbon_drawcanvas_at(dc, px, py), c_rgb | (u32)(fill * c_a));
        }
      }
    }
    position.x += sf * cdata.xadvance;
  }
}
