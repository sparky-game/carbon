// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

#define VWRAP(fn, vfn)                                \
  CBNFMT(1, 2) inline char *fn(const char *s, ...) {  \
  va_list args;                                       \
  va_start(args, s);                                  \
  char *x = vfn(s, args);                             \
  va_end(args);                                       \
  return x;                                           \
  }

namespace cbn::str {
  CBNFMT(1, 0) inline char *vFormat(const char *s, va_list args) {
    return carbon_string_vfmt(s, args);
  }
  VWRAP(Format, vFormat);

  CBNFMT(1, 0) inline char *vfmt(const char *s, va_list args) {
    return vFormat(s, args);
  }
  VWRAP(fmt, vfmt);
}

#undef VWRAP

#endif
