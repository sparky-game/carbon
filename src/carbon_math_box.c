// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

bool carbon_math_box_ray_intersects(CBN_Box b, CBN_Ray r, f32 *t) {
  CBN_Vec3 min = b.xyz;
  CBN_Vec3 max = carbon_math_vec3_add(b.xyz, b.whd);
  f32 t1, t2, tmin, tmax;
  if (!r.direction.x) {
    if (r.origin.x < min.x || r.origin.x > max.x) return false;
    tmin = 0; tmax = 1;
  }
  else {
    t1 = (min.x - r.origin.x)/r.direction.x;
    t2 = (max.x - r.origin.x)/r.direction.x;
    tmin = carbon_math_min(t1, t2);
    tmax = carbon_math_max(t1, t2);
  }
  if (!r.direction.y) {
    if (r.origin.y < min.y || r.origin.y > max.y) return false;
  }
  else {
    t1 = (min.y - r.origin.y)/r.direction.y;
    t2 = (max.y - r.origin.y)/r.direction.y;
    tmin = carbon_math_max(tmin, carbon_math_min(t1, t2));
    tmax = carbon_math_min(tmax, carbon_math_max(t1, t2));
  }
  if (!r.direction.z) {
    if (r.origin.z < min.z || r.origin.z > max.z) return false;
  }
  else {
    t1 = (min.z - r.origin.z)/r.direction.z;
    t2 = (max.z - r.origin.z)/r.direction.z;
    tmin = carbon_math_max(tmin, carbon_math_min(t1, t2));
    tmax = carbon_math_min(tmax, carbon_math_max(t1, t2));
  }
  *t = tmin;
  return tmin <= tmax;
}
