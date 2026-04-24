// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

template <typename T>
struct CBN_List_tt : CBN_List_t {
  using value_type = T;
  using iterator = value_type *;

  CBN_List_tt(void) : CBN_List_t{carbon_list_create(sizeof(value_type))} {}

  static cbn::Opt<CBN_List_tt> FromFile(const char *file);
  bool ToFile(const char *file) const;

  CBN_List_tt(const CBN_List_tt &) = delete;
  CBN_List_tt &operator=(const CBN_List_tt &) = delete;

  CBN_List_tt(CBN_List_tt &&l) : CBN_List_t{l} { carbon_memory_set(&l, 0, sizeof(l)); }

  CBN_List_tt &operator=(CBN_List_tt &&l) {
    if (this != &l) {
      Free();
      *this = l;
      carbon_memory_set(&l, 0, sizeof(l));
    }
    return *this;
  }

  ~CBN_List_tt(void) { Free(); }

  void Clear(void) { carbon_list_clear((CBN_List *)this); }

  value_type Front(void) const {
    value_type x;
    carbon_list_front((CBN_List *)this, &x);
    return x;
  }

  value_type Back(void) const {
    value_type x;
    carbon_list_back((CBN_List *)this, &x);
    return x;
  }

  void Push(const value_type &value) { carbon_list_push((CBN_List *)this, (void *)&value); }

  void Push(const CBN_StrView &sv) requires typeeq(value_type, char) { PushRange(sv); }
  CBN_List_tt &operator<<(const CBN_StrView &sv) { Push(sv); return *this; }

  void Push(const char *s) { Push(CBN_StrView(s)); }
  CBN_List_tt &operator<<(const char *s) { Push(s); return *this; }

  void PushRange(CBN_Span_tt<value_type> range) { carbon_list_push_range((CBN_List *)this, *(CBN_Span *)&range); }

  value_type PopFront(void) {
    value_type x;
    carbon_list_pop_front((CBN_List *)this, &x);
    return x;
  }

  value_type PopBack(void) {
    value_type x;
    carbon_list_pop_back((CBN_List *)this, &x);
    return x;
  }

  void Assign(usz count, const value_type &value) { carbon_list_assign((CBN_List *)this, count, (void *)&value); }
  void Assign(CBN_Span_tt<value_type> range) { carbon_list_assign_range((CBN_List *)this, *(CBN_Span *)&range); }

  isz Find(const value_type &value) const { return carbon_list_find((CBN_List *)this, (void *)&value); }

  void Remove(usz idx) { carbon_list_remove((CBN_List *)this, idx); }

  void ShrinkToFit(void) { carbon_list_shrink_to_fit((CBN_List *)this); }

  CBN_Span_tt<value_type> ToSpan(void) const { return CBN_Span_tt(*this); }

  iterator begin(void) const { return (iterator)items; }

  iterator end(void) const { return (iterator)items + size; }

  value_type &operator[](usz idx) const { return carbon_list_at(value_type, *this, idx); }

private:
  void Free(void) { carbon_list_destroy((CBN_List *)this); }
};

#endif
