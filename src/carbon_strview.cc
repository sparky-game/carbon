// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __cplusplus

CBN_StrView CBN_StrView::make(const char *data, usz size) {
  return carbon_strview_from_buf(data, size);
}

CBN_StrView CBN_StrView::make(const char *s) {
  return carbon_strview_from_cstr(s);
}

CBN_StrView CBN_StrView::make(const CBN_StrBuilder &sb) {
  return carbon_strview_from_strbuilder(&sb);
}

const char *CBN_StrView::c_str(void) const {
  return carbon_strview_to_cstr(*this);
}

CBN_StrView CBN_StrView::Trim(void) const {
  return carbon_strview_trim_both(*this);
}

CBN_StrView CBN_StrView::TrimStart(void) const {
  return carbon_strview_trim_left(*this);
}

CBN_StrView CBN_StrView::TrimEnd(void) const {
  return carbon_strview_trim_right(*this);
}

CBN_StrView CBN_StrView::Chop(void) {
  return carbon_strview_chop_by_space(this);
}

CBN_StrView CBN_StrView::Chop(char c) {
  return carbon_strview_chop(this, c);
}

usz CBN_StrView::LevDist(const CBN_StrView &sv) const {
  return carbon_string_lev_dist(c_str(), sv.c_str());
}

bool CBN_StrView::operator==(const CBN_StrView &sv) const {
  return carbon_strview_are_equal(*this, sv);
}

bool CBN_StrView::operator==(const char *s) const {
  return *this == make(s);
}

bool operator==(const char *s, const CBN_StrView &sv) {
  return sv == s;
}

bool CBN_StrView::operator!=(const CBN_StrView &sv) const {
  return !(*this == sv);
}

bool CBN_StrView::operator!=(const char *s) const {
  return !(*this == s);
}

bool operator!=(const char *s, const CBN_StrView &sv) {
  return !(s == sv);
}

constexpr CBN_StrView operator""sv(const char *s, usz len) {
  return CBN_StrView::make(s, len);
}

#endif  // __cplusplus
