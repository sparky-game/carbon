// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

template <typename T>
struct CBN_Span_tt : CBN_Span_t {
  using value_type = T;
  using iterator = value_type *;

  CBN_Span_tt(void) : CBN_Span_t{} {}
  CBN_Span_tt(auto *p, usz sz) : CBN_Span_t{carbon_span_from_buf(p, sz)} {}
  CBN_Span_tt(const CBN_List &l) : CBN_Span_t{carbon_span_from_list(&l)} {}

  CBN_Span_tt SubSpan(usz offset, usz count) const {
    return carbon_span_subspan(*this, offset, count);
  }

  iterator begin(void) const {
    return (iterator) data;
  }

  iterator end(void) const {
    return (iterator) data + size;
  }

  value_type &operator[](usz idx) {
    return carbon_span_at(value_type, *this, idx);
  }
};

#endif
