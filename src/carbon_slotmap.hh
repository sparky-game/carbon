/*
  $$=====================$$
  ||       SlotMap       ||
  $$=====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents the unique key format used to access stored values on a SlotMap.
 */
CBNDEF_T(CBN_SlotMap_Key) {
  u64 id;
  u64 gen;
};

/**
 * @brief Represents a container with persistent unique keys, O(1) time operations and low overhead.
 *
 * Great for storing collections of objects that need stable, safe references but have
 * no clear ownership otherwise, such as game entities or graph nodes.
 */
CBNDEF_TT(CBN_SlotMap) {
  usz stride;
  u64 size;
  u64 freelist;
  u64 generation;
  CBN_List indices;
  CBN_List data;
  CBN_List erase;
};
CBNDEF_TAKA(cbn, SlotMap, CBN_SlotMap);

#define carbon_slotmap_foreach(T, sm) for (struct { usz i; T var; } it = {0, carbon_list_at(T, sm.data, 0)}; it.i < (sm.data).size; ++it.i, it.i < (sm.data).size ? it.var = carbon_list_at(T, sm.data, it.i) : it.var)

/**
 * @brief Creates a new SlotMap container.
 * @param stride Size in bytes of the elements the SlotMap will hold.
 * @return The SlotMap container.
 */
CBNDEF CBN_SlotMap carbon_slotmap_create(usz stride);

/**
 * @brief Destroys a SlotMap container.
 * @param sm The SlotMap container.
 */
CBNDEF void carbon_slotmap_destroy(CBN_SlotMap *sm);

/**
 * @brief Inserts a copy of the value into the SlotMap.
 * @param sm The SlotMap container.
 * @param value The value to insert.
 * @return The generated unique key for the inserted value.
 */
CBNDEF CBN_SlotMap_Key carbon_slotmap_push(CBN_SlotMap *sm, void *value);

/**
 * @brief Removes an element using its key from the SlotMap.
 * @param sm The SlotMap container.
 * @param key The key of the element to be removed.
 * @return A boolean value indicating whether it removed the element successfully or not.
 */
CBNDEF u8 carbon_slotmap_remove(CBN_SlotMap *sm, const CBN_SlotMap_Key key);

/**
 * @brief Requests the retrieval or acquisition of an element from the SlotMap.
 * @param sm The SlotMap container.
 * @param key The key of the element to be retrieved.
 * @param out_value The value of the retrieved element (output argument pointer).
 * @return A boolean value indicating whether it retrieved the element successfully or not.
 */
CBNDEF u8 carbon_slotmap_lookup(const CBN_SlotMap *sm, const CBN_SlotMap_Key key, void *out_value);

/**
 * @brief Returns the string representation of the key using default formatting.
 * @param key The key to be serialized.
 * @return The serialized key as `(id, gen)`.
 */
CBNDEF char *carbon_slotmap_key_to_cstr(const CBN_SlotMap_Key key);

#ifdef __cplusplus
struct CBN_SlotMap_Key : CBN_SlotMap_Key_t {
  const char *ToString(void) const {
    return carbon_slotmap_key_to_cstr(*this);
  }
};

template <typename T>
struct CBN_SlotMap_tt : CBN_SlotMap_t {
  using Key = CBN_SlotMap_Key;
  using value_type = T;
  using iterator = value_type *;
  using indices_type = cbn::List<Key>;
  using data_type = cbn::List<value_type>;
  using erase_type = cbn::List<u64>;

  static CBN_SlotMap_tt make(void) {
    auto sm = carbon_slotmap_create(sizeof(value_type));
    return *(CBN_SlotMap_tt *) &sm;
  }

  void Free(void) {
    carbon_slotmap_destroy((CBN_SlotMap *) this);
  }

  Key Push(const value_type &value) {
    return carbon_slotmap_push((CBN_SlotMap *) this, (void *) &value);
  }

  u8 Remove(const Key k) {
    return carbon_slotmap_remove((CBN_SlotMap *) this, k);
  }

  iterator begin(void) const {
    return ((data_type *) &data)->begin();
  }

  iterator end(void) const {
    return ((data_type *) &data)->end();
  }

  cbn::Opt<value_type> operator[](const Key k) {
    value_type x;
    if (!carbon_slotmap_lookup((CBN_SlotMap *) this, k, &x)) return {};
    return x;
  }  
};
#endif
