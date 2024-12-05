// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func) {
  CARBON_ERROR_ASS("[!] %s:%u (%s) :: Runtime error raised\n    Expression :: %s", file, line, func, expr);
  abort();
}
