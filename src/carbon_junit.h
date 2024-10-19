// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_JUNIT_H_
#define CARBON_JUNIT_H_

#define CARBON_JUNIT_TESTCASE_NAME_MAX_LEN 512

typedef struct {
  double time;
  size_t tests;
  size_t failures;
} carbon_junit_testsuite;

typedef struct {
  char name[CARBON_JUNIT_TESTCASE_NAME_MAX_LEN];
  unsigned char has_failed;
} carbon_junit_testcase;

void carbon_junit_output(carbon_junit_testsuite *junit_ts, carbon_junit_testcase *junit_tcs, const char *out_filename);

#endif  // CARBON_JUNIT_H_

#ifdef CARBON_IMPLEMENTATION
#include "carbon_junit.c"
#endif  // CARBON_IMPLEMENTATION
