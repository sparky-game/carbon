// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

const char *CBN_StrView::c_str(void) const {
  return carbon_strview_to_cstr(*this);
}

#endif  // __cplusplus
