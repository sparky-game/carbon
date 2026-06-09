// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

template <typename T>
struct CBN_HashMap_Traits {
  static constexpr auto hash = carbon_hashmap_hash_mem;
  static constexpr auto eq = carbon_hashmap_eq_mem;
};

template <>
struct CBN_HashMap_Traits<const char *> {
  static constexpr auto hash = carbon_hashmap_hash_cstr;
  static constexpr auto eq = carbon_hashmap_eq_cstr;
};

template <typename K, typename V>
struct CBN_HashMap_tt : CBN_HashMap_t {
  using Node = CBN_HashMap_Node;
  using key_type = K;
  using value_type = V;

  CBN_HashMap_tt(void) : CBN_HashMap_t{carbon_hashmap_create(sizeof(key_type), sizeof(value_type),
                                                             CBN_HashMap_Traits<key_type>::hash,
                                                             CBN_HashMap_Traits<key_type>::eq)} {}

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

  void Set(const key_type &k, const value_type &v) { carbon_hashmap_set((CBN_HashMap *)this, (void *)&k, (void *)&v); }

  value_type *Find(const key_type &k) const { return (value_type *)carbon_hashmap_get((CBN_HashMap *)this, (void *)&k); }

  value_type &Get(const key_type &k) const {
    auto p = Find(k);
    CBN_ASSERT(p && "Key not found in HashMap");
    return *p;
  }
  value_type &operator[](const key_type &k) const { return Get(k); }

private:
  void Free(void) { carbon_hashmap_destroy((CBN_HashMap *)this); }
};

#endif
