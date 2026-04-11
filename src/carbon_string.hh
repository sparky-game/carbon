// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

#define VSFMT_WRAP(fn, vfn)                                           \
  CBNFMT(3, 4) inline void fn(char *buf, usz n, const char *s, ...) { \
  va_list args;                                                       \
  va_start(args, s);                                                  \
  vfn(buf, n, s, args);                                               \
  va_end(args);                                                       \
  }
#define VFMT_WRAP(fn, vfn)                            \
  CBNFMT(1, 2) inline char *fn(const char *s, ...) {  \
  va_list args;                                       \
  va_start(args, s);                                  \
  char *x = vfn(s, args);                             \
  va_end(args);                                       \
  return x;                                           \
  }

namespace cbn::str {
  CBNFMT(3, 0) inline void vsFormat(char *buf, usz n, const char *s, va_list args) {
    carbon_string_vsfmt(buf, n, s, args);
  }
  VSFMT_WRAP(sFormat, vsFormat);

  CBNFMT(3, 0) inline void vsfmt(char *buf, usz n, const char *s, va_list args) {
    vsFormat(buf, n, s, args);
  }
  VSFMT_WRAP(sfmt, vsfmt);

  CBNFMT(1, 0) inline char *vFormat(const char *s, va_list args) {
    return carbon_string_vfmt(s, args);
  }
  VFMT_WRAP(Format, vFormat);

  CBNFMT(1, 0) inline char *vfmt(const char *s, va_list args) {
    return vFormat(s, args);
  }
  VFMT_WRAP(fmt, vfmt);
}

#undef VFMT_WRAP
#undef VSFMT_WRAP

#endif
