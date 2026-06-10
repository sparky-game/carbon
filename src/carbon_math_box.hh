// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Box : CBN_Box_t {
  constexpr CBN_Box(f32 _x, f32 _y, f32 _z, f32 _w, f32 _h, f32 _d) : CBN_Box_t{{{_x, _y, _z}}, {{_w, _h, _d}}} {}
  constexpr CBN_Box(void) : CBN_Box(0, 0, 0, 0, 0, 0) {}
  constexpr CBN_Box(CBN_Vec3 p, CBN_Vec3 sz) : CBN_Box(p.x, p.y, p.z, sz.x, sz.y, sz.z) {}

  cbn::Opt<f32> RayIntersects(const CBN_Ray &r) const {
    f32 t;
    if (!carbon_math_box_ray_intersects(*this, r, &t)) return {};
    return t;
  }
};

#endif
