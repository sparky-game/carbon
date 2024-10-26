// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define ISO_8601_FMT "%Y-%m-%dT%H:%M:%S%z"

void carbon_junit_output(JUnitTestsuite *junit_ts, JUnitTestcase *junit_tcs, const char *out_filename) {
  if (!out_filename) out_filename = CARBON_JUNIT_XML_OUT_FILENAME;
  FILE *fd = fopen(out_filename, "w");
  if (!fd) {
    CARBON_ERROR("carbon_junit_output :: unable to open file (`%s`)", out_filename);
    return;
  }
  time_t t = time(0);
  struct tm *tm_info = localtime(&t);
  char timestamp[30];
  strftime(timestamp, sizeof(timestamp), ISO_8601_FMT, tm_info);
  fprintf(fd, "<testsuite timestamp=\"%s\" time=\"%.6f\" tests=\"%zu\" failures=\"%zu\">\n", timestamp, junit_ts->time, junit_ts->tests, junit_ts->failures);
  for (usz i = 0; i < junit_ts->tests; ++i) {
    if (junit_tcs[i].has_failed) {
      fprintf(fd, "  <testcase name=\"%s\">\n", junit_tcs[i].name);
      fprintf(fd, "    <failure />\n");
      fprintf(fd, "  </testcase>\n");
    }
    else fprintf(fd, "  <testcase name=\"%s\" />\n", junit_tcs[i].name);
  }
  fprintf(fd, "</testsuite>");
  fclose(fd);
}
