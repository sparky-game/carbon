// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

template <typename T>
struct CBN_HashMap_tt : CBN_HashMap_t {
  using value_type = T;
  using Node = CBN_HashMap_Node;

  CBN_HashMap_tt(void) : CBN_HashMap_t{carbon_hashmap_create(sizeof(value_type))} {}

  CBN_HashMap_tt(const CBN_HashMap_tt &) = delete;
  CBN_HashMap_tt &operator=(const CBN_HashMap_tt &) = delete;

  CBN_HashMap_tt(CBN_HashMap_tt &&hm) : CBN_HashMap_t{hm} { carbon_memory_set(&hm, 0, sizeof(hm)); }

  CBN_HashMap_tt &operator=(CBN_HashMap_tt &&hm) {
    if (this != &hm) {
      Free();
      *(CBN_HashMap_t *)this = hm;
      carbon_memory_set(&hm, 0, sizeof(hm));
    }
    return *this;
  }

  ~CBN_HashMap_tt(void) { Free(); }

  void Set(const char *key, const value_type &value) { carbon_hashmap_set((CBN_HashMap *)this, key, (void *)&value); }

  cbn::Opt<value_type> Get(const char *key) const {
    value_type x;
    if (!carbon_hashmap_get((CBN_HashMap *)this, key, (void *)&x)) return {};
    return x;
  }

private:
  void Free(void) { carbon_hashmap_destroy((CBN_HashMap *)this); }
};

#endif
