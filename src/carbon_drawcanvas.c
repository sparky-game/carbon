// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_DRAWCANVAS__AA_RES             2
#define CARBON_DRAWCANVAS__NEAR_PLANE_EPSILON 0.2

struct CBN_DrawCanvas {
  u32 *pixels;
  f32 *zbuffer;
  usz width;
  usz height;
  u32 flags;
};

#include "carbon_drawcanvas_monofont.inl"
#include "carbon_drawcanvas_helpers.inl"
#include "carbon_drawcanvas_st.inl"
#include "carbon_drawcanvas_mt.inl"

CBN_DrawCanvas *carbon_drawcanvas_create(usz width, usz height) {
  CBN_DrawCanvas *dc = (CBN_DrawCanvas *) carbon_memory_alloc(sizeof(CBN_DrawCanvas));
  dc->pixels  = (u32 *) carbon_memory_alloc(width * height * sizeof(u32));
  dc->zbuffer = (f32 *) carbon_memory_alloc(width * height * sizeof(f32));
  dc->width   = width;
  dc->height  = height;
  dc->flags   = 0;
  return dc;

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

u32 *carbon_drawcanvas_pixels(const CBN_DrawCanvas *dc) {
  return dc->pixels;
}

usz carbon_drawcanvas_width(const CBN_DrawCanvas *dc) {
  return dc->width;
}

usz carbon_drawcanvas_height(const CBN_DrawCanvas *dc) {
  return dc->height;
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

void carbon_drawcanvas_fill(CBN_DrawCanvas *dc, u32 color) {
  carbon_drawcanvas__dispatch_backend(fill, dc, color);
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

void carbon_drawcanvas_circle(CBN_DrawCanvas *dc, CBN_Vec2 center, usz radius, u32 color) {
  i32 x1, x2, y1, y2;
  CBN_Rect xywh = carbon_math_rect_sq(center.x - radius, center.y - radius, 2*radius);
  if (!carbon_drawcanvas__rect_normalize(dc, xywh, &x1, &x2, &y1, &y2)) return;
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

void carbon_drawcanvas_sprite(CBN_DrawCanvas *dc, const CBN_Sprite *s, CBN_Vec2 position, CBN_Vec2 scale) {
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
    const usz y = (usz) src_y;
    const u32 *row = s->pixels + y*s->width;
    f32 src_x = start_x;
    for (usz i = 0; i < xywh.w; ++i) {
      const usz x = (usz) src_x;
      carbon_drawcanvas__alpha_blending(p_dc, row[x]);
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
  const CBN_Vec3 light = carbon_math_vec3_norm(carbon_math_vec3(0, 0, 1));
  const CBN_Vec3 cam_pos = carbon_camera_get_position(c);
  for (usz f = 0; f < m->metadata.faces_count; ++f) {
    const usz *i = m->faces[f][CARBON_MESH_FACE_COMP_VERTEX];
    const Vertex3D v1 = vs[i[0]], v2 = vs[i[1]], v3 = vs[i[2]];
    if (dc->flags & CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING) {
      if (carbon_drawcanvas__is_back_face(dc, cam_pos, v1.world, v2.world, v3.world)) continue;
    }
    Vertex3D pvs[4];
    usz pvs_count = carbon_drawcanvas__near_plane_clipping(v1, v2, v3, pvs);
    carbon_drawcanvas__poly_triangulation(dc, pvs, pvs_count, light, color);
  }
}

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
  const CBN_Mat4 MVP = carbon_math_mat4_mult(P, V);
  for (usz i = 0; i < CARBON_ARRAY_LEN(vs); ++i) {
    CBN_Vec4 v = carbon_math_vec4_3(vs[i].world, 1);
    vs[i].clip = carbon_math_mat4_mult_vec4(MVP, v);
  }
  const CBN_Vec3 light = carbon_math_vec3_norm(carbon_math_vec3_1(1));
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

usz carbon_drawcanvas_get_text_width(const char *txt, usz size) {
  return CARBON_DRAWCANVAS__MONOFONT_WIDTH * size * carbon_string_len(txt);
}

usz carbon_drawcanvas_get_text_height(usz size) {
  return CARBON_DRAWCANVAS__MONOFONT_HEIGHT * size;
}
