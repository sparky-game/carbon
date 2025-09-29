/*
**  $$===================$$
**  ||       JUnit       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_JUNIT_XML_OUT_FILENAME "carbon_results.xml"

typedef struct {
  char *name;
  u8 has_failed;
} CBN_JUnitTestcase;

CBNDEF void carbon_junit_output(const CBN_List junit_tcs, const char *out_filename, const usz failed, const f64 runtime);
