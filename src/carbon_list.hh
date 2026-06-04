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
      *(CBN_List_t *)this = l;
      carbon_memory_set(&l, 0, sizeof(l));
    }
    return *this;
  }

  ~CBN_List_tt(void) { Free(); }

  CBN_Span_tt<value_type> ToSpan(void) const { return CBN_Span_tt(*this); }

  void Clear(void) {
    if constexpr (!cbn::meta::IsTriviallyDestructible_v<value_type>) {
      for (auto &i : *this) i.~value_type();
    }
    carbon_list_clear((CBN_List *)this);
  }

  void Reserve(usz min) { carbon_list_reserve((CBN_List *)this, min); }

  void ShrinkToFit(void) { carbon_list_shrink_to_fit((CBN_List *)this); }

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

  void Push(const value_type &value) {
    if constexpr (!cbn::meta::IsTriviallyCopyable_v<value_type>) {
      Reserve(size + 1);
      new (items + size*stride) value_type(value);
      ++size;
    }
    else carbon_list_push((CBN_List *)this, (void *)&value);
  }

  void Push(const CBN_StrView &sv) requires typeeq(value_type, char) { PushRange(sv); }
  CBN_List_tt &operator<<(const CBN_StrView &sv) { Push(sv); return *this; }

  void Push(const char *s) { Push(CBN_StrView(s)); }
  CBN_List_tt &operator<<(const char *s) { Push(s); return *this; }

  void PushRange(CBN_Span_tt<value_type> range) { carbon_list_push_range((CBN_List *)this, *(CBN_Span *)&range); }

  value_type PopFront(void) {
    if constexpr (!cbn::meta::IsTriviallyDestructible_v<value_type>) {
      value_type x = cbn::meta::Move((*this)[0]);
      (*this)[0].~value_type();
      if (size > 1) {
        new (items) value_type(cbn::meta::Move((*this)[1]));
        for (usz i = 1; i < size - 1; ++i) (*this)[i] = cbn::meta::Move((*this)[i + 1]);
        (*this)[size - 1].~value_type();
      }
      --size;
      return x;
    }
    else {
      value_type x;
      carbon_list_pop_front((CBN_List *)this, &x);
      return x;
    }
  }

  value_type PopBack(void) {
    if constexpr (!cbn::meta::IsTriviallyDestructible_v<value_type>) {
      value_type x = cbn::meta::Move((*this)[size - 1]);
      (*this)[size - 1].~value_type();
      --size;
      return x;
    }
    else {
      value_type x;
      carbon_list_pop_back((CBN_List *)this, &x);
      return x;
    }
  }

  void Assign(usz count, const value_type &value) { carbon_list_assign((CBN_List *)this, count, (void *)&value); }
  void Assign(CBN_Span_tt<value_type> range) { carbon_list_assign_range((CBN_List *)this, *(CBN_Span *)&range); }

  isz Find(const value_type &value) const { return carbon_list_find((CBN_List *)this, (void *)&value); }

  template <typename Predicate>
  isz FindIf(Predicate pred) const {
    for (usz i = 0; i < size; ++i) {
      if (pred((*this)[i])) return i;
    }
    return -1;
  }

  void Remove(usz idx) {
    if constexpr (!cbn::meta::IsTriviallyDestructible_v<value_type>) {
      (*this)[idx].~value_type();
      if (idx < size - 1) {
        new (items + idx*stride) value_type(cbn::meta::Move((*this)[idx + 1]));
        for (usz i = idx + 1; i < size - 1; ++i) (*this)[i] = cbn::meta::Move((*this)[i + 1]);
        (*this)[size - 1].~value_type();
      }
      --size;
    }
    else carbon_list_remove((CBN_List *)this, idx);
  }

  template <typename Compare>
  void Sort(cbn::sort::Func sort, Compare) {
    auto f_cmp = [](const void *ap, const void *bp) {
      Compare cmp;
      const auto &a = *static_cast<const value_type *>(ap);
      const auto &b = *static_cast<const value_type *>(bp);
      if (cmp(a, b)) return -1;
      if (cmp(b, a)) return 1;
      return 0;
    };
    sort(items, size, sizeof(value_type), f_cmp);
  }

  iterator begin(void) const { return (iterator)items; }

  iterator end(void) const { return (iterator)items + size; }

  value_type &operator[](usz idx) const { return carbon_list_at(value_type, *this, idx); }

private:
  void Free(void) {
    Clear();
    carbon_list_destroy((CBN_List *)this);
  }
};

#endif
