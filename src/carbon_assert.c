// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func) {
  carbon_ceprintln(CARBON_COLOR_RED, "[!] %s:%u (%s) :: Runtime error raised\n    Expression :: %s", file, line, func, expr);
  __builtin_abort();
}
