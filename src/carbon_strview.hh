// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_StrView : CBN_StrView_t {
  CBN_StrView(const char *p, usz sz) : CBN_StrView(carbon_strview_from_buf(p, sz)) {}
  CBN_StrView(const char *s) : CBN_StrView(carbon_strview_from_cstr(s)) {}

  operator CBN_Span_tt<char>(void) const { return CBN_Span_tt<char>(data, size); }

  const char *c_str(void) const { return carbon_strview_to_cstr(*this); }

  CBN_StrView Trim(void) const { return carbon_strview_trim_both(*this); }

  CBN_StrView TrimLeft(void) const { return carbon_strview_trim_left(*this); }

  CBN_StrView TrimRight(void) const { return carbon_strview_trim_right(*this); }

  CBN_StrView Chop(void) { return carbon_strview_chop_by_space(this); }

  CBN_StrView Chop(char c) { return carbon_strview_chop(this, c); }

  bool Contains(const CBN_StrView &sv) const { return carbon_strview_contains(*this, sv); }

  bool StartsWith(const CBN_StrView &sv) const { return carbon_strview_starts_with(*this, sv); }

  bool EndsWith(const CBN_StrView &sv) const { return carbon_strview_ends_with(*this, sv); }

  usz LevDist(const CBN_StrView &sv) const { return carbon_string_lev_dist(c_str(), sv.c_str()); }
  
  bool operator==(const CBN_StrView &sv) const { return carbon_strview_are_equal(*this, sv); }
  
  bool operator==(const char *s) const { return *this == CBN_StrView(s); }
  
  friend bool operator==(const char *s, const CBN_StrView &sv) { return sv == s; }
  
  bool operator!=(const CBN_StrView &sv) const { return !(*this == sv); }
  
  bool operator!=(const char *s) const { return !(*this == s); }
  
  friend bool operator!=(const char *s, const CBN_StrView &sv) { return !(s == sv); }
};

namespace cbn::str::literals {
  inline View operator""_sv(const char *s, usz len) { return View(s, len); }
}

#endif
