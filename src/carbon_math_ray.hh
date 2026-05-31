// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Ray : CBN_Ray_t {
  constexpr CBN_Ray(const CBN_Vec3 &ori, const CBN_Vec3 &dir) : CBN_Ray_t{ori, dir} {}

  CBN_Vec3 GetPoint(f32 distance) const { return carbon_math_ray_get_point(*this, distance); }

  const char *ToString(void) const { return carbon_math_ray_to_cstr(*this); }
};

#endif
