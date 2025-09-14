// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

const char *CBN_Vec4::ToString(void) const {
  return carbon_math_vec4_to_cstr(*this);
}
