// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define ISO_8601_FMT "%Y-%m-%dT%H:%M:%S%z"

void carbon_junit_output(const CBN_List junit_tcs, const char *out_filename, const usz failed, const f64 runtime) {
  if (!out_filename) out_filename = CARBON_JUNIT_XML_OUT_FILENAME;
  FILE *fd = fopen(out_filename, "w");
  if (!fd) {
    CBN_ERROR("unable to open file (`%s`)", out_filename);
    return;
  }
  const char *timestamp = carbon_time_get_iso8601();
  fprintf(fd, "<testsuite timestamp=\"%s\" time=\"%.6f\" tests=\"%zu\" failures=\"%zu\">\n", timestamp, runtime, junit_tcs.size, failed);
  carbon_list_foreach(CBN_JUnitTestcase, junit_tcs) {
    if (it.var.has_failed) {
      fprintf(fd, "  <testcase name=\"%s\">\n", it.var.name);
      fprintf(fd, "    <failure />\n");
      fprintf(fd, "  </testcase>\n");
    }
    else fprintf(fd, "  <testcase name=\"%s\" />\n", it.var.name);
  }
  fprintf(fd, "</testsuite>");
  fclose(fd);
}
