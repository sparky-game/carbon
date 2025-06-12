// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "../carbon.h.in"
#include "carbon_deps.h"
#include "carbon_defs.h"
#include "carbon_types.h"
#include "carbon_log.h"
#include "carbon_string.h"

void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func) {
  carbon_ceprintln(CARBON_COLOR_RED, "[!] %s:%u (%s) :: Runtime error raised\n    Expression :: %s", file, line, func, expr);
  abort();
}
