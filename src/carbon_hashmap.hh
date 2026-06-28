// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

template <typename T>
struct CBN_HashMap_KeyTraits {
  static constexpr auto hash = carbon_hashmap_hash_mem;
  static constexpr auto eq = carbon_hashmap_eq_mem;
};

template <>
struct CBN_HashMap_KeyTraits<const char *> {
  static constexpr auto hash = carbon_hashmap_hash_cstr;
  static constexpr auto eq = carbon_hashmap_eq_cstr;
};

template <>
struct CBN_HashMap_KeyTraits<std::string> {
  static constexpr CBN_HashMap_HashFunc hash = [](const void *p, usz){
    const std::string &s = *(const std::string *)p;
    return carbon_hashmap_hash_mem((void *)s.data(), s.size());
  };
  static constexpr CBN_HashMap_EqFunc eq = [](const void *a, const void *b, usz){
    return *(const std::string *)a == *(const std::string *)b;
  };
};

template <typename K, typename V>
struct CBN_HashMap_tt : CBN_HashMap_t {
  using Node = CBN_HashMap_Node;
  using key_type = K;
  using key_traits = CBN_HashMap_KeyTraits<key_type>;
  using value_type = V;

  CBN_HashMap_tt(void)
    : CBN_HashMap_t{carbon_hashmap_create(sizeof(key_type), sizeof(value_type), key_traits::hash, key_traits::eq)}
  {}

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

  void Clear(void) {
    TearDownInternals();
    carbon_hashmap_clear((CBN_HashMap *)this);
  }

  template <typename T>
  value_type *Set(const key_type &k, T &&v) {
    constexpr auto itc_k = cbn::meta::IsTriviallyCopyable_v<key_type>;
    constexpr auto itc_v = cbn::meta::IsTriviallyCopyable_v<value_type>;
    value_type *it = Find(k);
    if (it) {
      if constexpr (!itc_v) *it = cbn::meta::Forward<T>(v);
      else cbn::mem::cp(it, &v, v_stride);
      return it;
    }
    if constexpr (itc_k and itc_v) {
      return (value_type *)carbon_hashmap_set((CBN_HashMap *)this, (void *)&k, (void *)&v);
    }
    else {
      auto v_it = (value_type *)carbon_hashmap_set((CBN_HashMap *)this, (void *)&k, nullptr);
      auto k_it = (key_type *)((u8 *)v_it - k_stride);
      if constexpr (!itc_k) new (k_it) key_type(k);
      new (v_it) value_type(cbn::meta::Forward<T>(v));
      return v_it;
    }
  }

  value_type *Find(const key_type &k) const
  { return (value_type *)carbon_hashmap_get((CBN_HashMap *)this, (void *)&k); }

  value_type &Get(const key_type &k) const
  {// If key exists, returns existing value. Otherwise, triggers a runtime error.
    auto p = Find(k);
    CBN_ASSERT(p && "Key not found in HashMap");
    return *p;
  }

  value_type &operator[](const key_type &k)
  {// If key exists, returns existing value. Otherwise, it default-constructs one.
    if (auto it = Find(k)) return *it;
    return *Set(k, value_type{});
  }

private:
  void Free(void) {
    TearDownInternals();
    carbon_hashmap_destroy((CBN_HashMap *)this);
  }

  void TearDownInternals(void) {
    constexpr auto itd_k = cbn::meta::IsTriviallyDestructible_v<key_type>;
    constexpr auto itd_v = cbn::meta::IsTriviallyDestructible_v<value_type>;
    if ((itd_k and itd_v) or !buckets) return;
    for (usz i = 0; i < capacity; ++i) {
      Node *curr = buckets[i];
      while (curr) {
        if constexpr (!itd_k) ((key_type *)curr->kv)->~key_type();
        if constexpr (!itd_v) ((value_type *)(curr->kv + k_stride))->~value_type();
        curr = curr->next;
      }
    }
  }
};

#endif
