// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

void carbon_log_print(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stream, fmt, args);
  /* CBN_StrList fmt_splitted = carbon_strlist_from_splitted_cstr(fmt, "%$"); */
  /* carbon_strlist_foreach(fmt_splitted) { */
  /*   vfprintf(stream, carbon_strview_to_cstr(it.sv), args); */
  /*   if ((it.i != fmt_splitted.size - 1) || (it.i == fmt_splitted.size - 1 && carbon_string_ends_with_substr(fmt, "%$"))) { */
  /*     CBN_Log_BoxedVar arg = va_arg(args, CBN_Log_BoxedVar); */
  /*     // TODO: check that arg is a valid `CBN_Log_BoxedVar` */
  /*     carbon_log__print_boxed_var(stream, arg); */
  /*   } */
  /* } */
  /* fflush(stream); */
  /* carbon_strlist_destroy(&fmt_splitted); */
  va_end(args);
}
