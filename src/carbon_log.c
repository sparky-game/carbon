// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_LOG__MAX_LEN 1024

void carbon_log_write(i32 fd, CBN_Span msg) {
  write(fd, msg.data, msg.size);
}

void carbon_log_print(i32 fd, const char *s, ...) {
  char buf[CARBON_LOG__MAX_LEN] = {0};
  va_list args;
  va_start(args, s);
  carbon_string_vsfmt(buf, sizeof(buf), s, args);
  va_end(args);
  carbon_log_write(fd, carbon_strview_from_cstr(buf));
}
