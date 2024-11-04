// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func) {
  CARBON_ERROR("%s:%d (%s) :: Runtime error raised\n    Expression :: %s", file, line, func, expr);
  abort();
}
