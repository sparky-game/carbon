// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_Quat : CBN_Quat_t {
  using CBN_Quat_t::CBN_Quat_t;
  constexpr CBN_Quat(void) : CBN_Quat_t(0, 0, 0, 1) {}
  explicit CBN_Quat(CBN_Vec3 v) : CBN_Quat_t{carbon_math_quat_from_euler(v)} {}
  explicit CBN_Quat(CBN_Vec3 axis, f32 angle) : CBN_Quat_t{carbon_math_quat_from_axis_angle(axis, angle)} {}

  CBN_Quat operator*(CBN_Quat q) const { return carbon_math_quat_mult(*this, q); }
};

#endif
