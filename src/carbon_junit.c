/*
 * BSD Carbon --- A simple C/C++ unit testing framework
 * Copyright (C) 2024 Wasym A. Alonso
 *
 * This file is part of Carbon.
 *
 * Carbon is free software: you can redistribute it and/or modify
 * it under the terms of the BSD 3-Clause "New" or "Revised" License
 * as published by The Regents of the University of California.
 *
 * Carbon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * BSD 3-Clause "New" or "Revised" License for more details.
 *
 * You should have received a copy of the BSD 3-Clause "New" or
 * "Revised" License along with Carbon.
 * If not, see <https://opensource.org/license/BSD-3-Clause>.
 */


#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#include <time.h>

#define XML_OUT_FILENAME "carbon_results.xml"
#define ISO_8601_FMT     "%Y-%m-%dT%H:%M:%S%z"

void carbon_junit_output(carbon_junit_testsuite *junit_ts, carbon_junit_testcase *junit_tcs, const char *out_filename) {
  if (!out_filename) out_filename = XML_OUT_FILENAME;
  FILE *fd = fopen(out_filename, "w");
  if (!fd) {
    CARBON_ERROR("[ERROR]: carbon_junit_output :: unable to open file (`%s`)\n", out_filename);
    return;
  }
  time_t t = time(0);
  struct tm *tm_info = localtime(&t);
  char timestamp[30];
  strftime(timestamp, sizeof(timestamp), ISO_8601_FMT, tm_info);
  fprintf(fd, "<testsuite timestamp=\"%s\" time=\"%.6f\" tests=\"%zu\" failures=\"%zu\">\n", timestamp, junit_ts->time, junit_ts->tests, junit_ts->failures);
  for (size_t i = 0; i < junit_ts->tests; ++i) {
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
