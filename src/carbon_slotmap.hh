// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

struct CBN_SlotMap_Key : CBN_SlotMap_Key_t {
  bool operator==(const CBN_SlotMap_Key &k) const { return id == k.id && gen == k.gen; }

  bool operator!=(const CBN_SlotMap_Key &k) const { return !(*this == k); }

  const char *ToString(void) const { return carbon_slotmap_key_to_cstr(*this); }
};

template <typename T>
struct CBN_SlotMap_tt : CBN_SlotMap_t {
  using value_type = T;
  using Key = CBN_SlotMap_Key;
  using iterator = value_type *;
  using indices_type = cbn::List<Key>;
  using data_type = cbn::List<value_type>;
  using erase_type = cbn::List<u64>;

  CBN_SlotMap_tt(void) : CBN_SlotMap_t{carbon_slotmap_create(sizeof(value_type))} {}

  CBN_SlotMap_tt(const CBN_SlotMap_tt &) = delete;
  CBN_SlotMap_tt &operator=(const CBN_SlotMap_tt &) = delete;

  CBN_SlotMap_tt(CBN_SlotMap_tt &&sm) : CBN_SlotMap_t{sm} { carbon_memory_set(&sm, 0, sizeof(sm)); }

  CBN_SlotMap_tt &operator=(CBN_SlotMap_tt &&sm) {
    if (this != &sm) {
      Free();
      *(CBN_SlotMap_t *)this = sm;
      carbon_memory_set(&sm, 0, sizeof(sm));
    }
    return *this;
  }

  ~CBN_SlotMap_tt(void) { Free(); }

  Key Push(const value_type &value) { return carbon_slotmap_push((CBN_SlotMap *)this, (void *)&value); }

  bool Remove(Key k) { return carbon_slotmap_remove((CBN_SlotMap *)this, k); }

  iterator begin(void) const { return ((data_type *)&data)->begin(); }

  iterator end(void) const { return ((data_type *)&data)->end(); }

  cbn::Opt<value_type> operator[](Key k) {
    value_type x;
    if (!carbon_slotmap_lookup((CBN_SlotMap *)this, k, &x)) return {};
    return x;
  }

private:
  void Free(void) { carbon_slotmap_destroy((CBN_SlotMap *)this); }
};

#endif
