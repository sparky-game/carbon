/*
**  $$====================$$
**  ||       Assert       ||
**  $$====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CBN_ASSERT(x) ((void) ((x) || (carbon_assert_abort(#x, __FILE__, __LINE__, __func__), 0)))

CARBON_API void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func);
