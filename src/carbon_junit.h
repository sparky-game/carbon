// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_JUNIT_H_
#define CARBON_JUNIT_H_

#define CARBON_JUNIT_XML_OUT_FILENAME      "carbon_results.xml"
#define CARBON_JUNIT_TESTCASE_NAME_MAX_LEN 512

typedef struct {
  f64 time;
  usz tests;
  usz failures;
} JUnitTestsuite;

typedef struct {
  char name[CARBON_JUNIT_TESTCASE_NAME_MAX_LEN];
  u8 has_failed;
} JUnitTestcase;

void carbon_junit_output(JUnitTestsuite *junit_ts, JUnitTestcase *junit_tcs, const char *out_filename);

#endif  // CARBON_JUNIT_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_junit.c"
#endif  // CARBON_IMPLEMENTATION
