// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define carbon_log__printv(stream, x) fprintf(stream, carbon_log__var_to_spec(x), (x))

CARBON_INLINE void carbon_log__print_boxed_var(FILE *stream, CBN_Log_BoxedVar x) {
  if      (!carbon_string_cmp(x.spec, "%zu"))  carbon_log__printv(stream, *((usz *) x.var));
  else if (!carbon_string_cmp(x.spec, "%zd"))  carbon_log__printv(stream, *((isz *) x.var));
  else if (!carbon_string_cmp(x.spec, "%hhu")) carbon_log__printv(stream, *((u8 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%hhd")) carbon_log__printv(stream, *((i8 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%hu"))  carbon_log__printv(stream, *((u16 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%hd"))  carbon_log__printv(stream, *((i16 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%u"))   carbon_log__printv(stream, *((u32 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%u"))   carbon_log__printv(stream, *((u32 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%d"))   carbon_log__printv(stream, *((i32 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%llu")) carbon_log__printv(stream, *((u64 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%lld")) carbon_log__printv(stream, *((i64 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%f"))   carbon_log__printv(stream, *((f32 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%lf"))  carbon_log__printv(stream, *((f64 *) x.var));
  else if (!carbon_string_cmp(x.spec, "%c"))   carbon_log__printv(stream, *((char *) x.var));
  else if (!carbon_string_cmp(x.spec, "%s"))   carbon_log__printv(stream, *((char **) x.var));
  else    carbon_log__printv(stream, x.var);
}

void carbon_log_print(FILE *stream, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  CBN_StrList fmt_splitted = carbon_strlist_from_splitted_cstr(fmt, "%$");
  carbon_strlist_foreach(fmt_splitted) {
    vfprintf(stream, carbon_strview_to_cstr(it.sv), args);
    if ((it.i != fmt_splitted.size - 1) || (it.i == fmt_splitted.size - 1 && carbon_string_ends_with_substr(fmt, "%$"))) {
      CBN_Log_BoxedVar arg = va_arg(args, CBN_Log_BoxedVar);
      // TODO: check that arg is a valid `CBN_Log_BoxedVar`
      carbon_log__print_boxed_var(stream, arg);
    }
  }
  fflush(stream);
  carbon_strlist_destroy(&fmt_splitted);
  va_end(args);
}
