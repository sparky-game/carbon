/*
**  $$=====================$$
**  ||       SlotMap       ||
**  $$=====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents the unique key format used to access stored values on a SlotMap.
 */
typedef struct {
  u64 id;
  u64 gen;
} CBN_SlotMap_Key;

/**
 * @brief Represents a container with persistent unique keys, O(1) time operations and low overhead.
 *
 * Great for storing collections of objects that need stable, safe references but have
 * no clear ownership otherwise, such as game entities or graph nodes.
 *
 * In C++, this is a templated class/struct, which means it's not a type by itself,
 * until it gets instantiated with the needed template arguments. The type `CBN_SlotMap`
 * is an alias for `CBN_SlotMap_t<void *>`, which provides full-compatibility with the
 * C API, even through C++ templates.
 *
 * @param T Type information of what will be stored in the container.
 */
CARBON_TEMPLATE_CLASS(CBN_SlotMap_t) {
  usz stride;
  u64 size;
  u64 freelist;
  u64 generation;
  CBN_List indices;
  CBN_List data;
  CBN_List erase;
#ifdef __cplusplus
  using Key = CBN_SlotMap_Key;
  using value_type = T;
  using iterator = value_type *;
  using const_iterator = const iterator;
  using indices_type = CBN_List_t<Key>;
  using data_type = CBN_List_t<value_type>;
  using erase_type = CBN_List_t<u64>;
  /**
   * @brief carbon_slotmap_create
   * @return The SlotMap container.
   */
  static CBN_SlotMap_t make(void);
  /**
   * @brief carbon_slotmap_destroy
   */
  void Free(void);
  /**
   * @brief carbon_slotmap_push
   * @param value The value to insert.
   * @return The generated unique key for the inserted value.
   */
  [[nodiscard]] CBN_SlotMap_Key Push(const value_type &value);
  /**
   * @brief carbon_slotmap_remove
   * @param key The key of the element to be removed.
   * @return A boolean value indicating whether it removed the element successfully or not.
   */
  u8 Remove(const CBN_SlotMap_Key key);
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void);
  /**
   * @brief Returns a constant iterator to the beginning.
   * @return The const_iterator.
   */
  const_iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void);
  /**
   * @brief Returns a constant iterator to the end.
   * @return The const_iterator.
   */
  const_iterator end(void) const;
  // Overloaded Operators
  cbn::Opt<value_type> operator[](const CBN_SlotMap_Key key);
  cbn::Opt<const value_type> operator[](const CBN_SlotMap_Key key) const;
#endif
};

#ifdef __cplusplus
using CBN_SlotMap = CBN_SlotMap_t<void *>;
#else
typedef struct CBN_SlotMap_t CBN_SlotMap;
#endif

#define carbon_slotmap_foreach(T, sm) for (struct { usz i; T var; } it = {0, carbon_list_at(T, sm.data, 0)}; it.i < (sm.data).size; ++it.i, it.i < (sm.data).size ? it.var = carbon_list_at(T, sm.data, it.i) : it.var)

/**
 * @brief Creates a new SlotMap container.
 * @param stride Size in bytes of the elements the SlotMap will hold.
 * @return The SlotMap container.
 */
CARBON_API CBN_SlotMap carbon_slotmap_create(usz stride);

/**
 * @brief Destroys a SlotMap container.
 * @param sm The SlotMap container.
 */
CARBON_API void carbon_slotmap_destroy(CBN_SlotMap *sm);

/**
 * @brief Inserts a copy of the value into the SlotMap.
 * @param sm The SlotMap container.
 * @param value The value to insert.
 * @return The generated unique key for the inserted value.
 */
CARBON_API CBN_SlotMap_Key carbon_slotmap_push(CBN_SlotMap *sm, void *value);

/**
 * @brief Removes an element using its key from the SlotMap.
 * @param sm The SlotMap container.
 * @param key The key of the element to be removed.
 * @return A boolean value indicating whether it removed the element successfully or not.
 */
CARBON_API u8 carbon_slotmap_remove(CBN_SlotMap *sm, const CBN_SlotMap_Key key);

/**
 * @brief Requests the retrieval or acquisition of an element from the SlotMap.
 * @param sm The SlotMap container.
 * @param key The key of the element to be retrieved.
 * @param out_value The value of the retrieved element (output argument pointer).
 * @return A boolean value indicating whether it retrieved the element successfully or not.
 */
CARBON_API u8 carbon_slotmap_lookup(const CBN_SlotMap *sm, const CBN_SlotMap_Key key, void *out_value);

// Because `CBN_SlotMap_t` is a templated class/struct, the definitions of all its
// member functions need to be available at compile-time, not link-time; thus,
// we define them here for the template instantiation to work properly.
#ifdef __cplusplus
/**
 * @brief CBN_SlotMap_t<T>::make
 */
template <typename T>
CBN_SlotMap_t<T> CBN_SlotMap_t<T>::make(void) {
  auto sm = carbon_slotmap_create(sizeof(T));
  return *(CBN_SlotMap_t<T> *) &sm;
}
/**
 * @brief CBN_SlotMap_t<T>.Free
 */
template <typename T>
void CBN_SlotMap_t<T>::Free(void) {
  carbon_slotmap_destroy((CBN_SlotMap *) this);
}
/**
 * @brief CBN_SlotMap_t<T>.Push
 */
template <typename T>
CBN_SlotMap_Key CBN_SlotMap_t<T>::Push(const value_type &value) {
  return carbon_slotmap_push((CBN_SlotMap *) this, (void *) &value);
}
/**
 * @brief CBN_SlotMap_t<T>.Remove
 */
template <typename T>
u8 CBN_SlotMap_t<T>::Remove(const CBN_SlotMap_Key key) {
  return carbon_slotmap_remove((CBN_SlotMap *) this, key);
}
/**
 * @brief CBN_SlotMap_t<T>.begin
 */
template <typename T>
typename CBN_SlotMap_t<T>::iterator CBN_SlotMap_t<T>::begin(void) {
  return const_cast<iterator>(static_cast<const CBN_SlotMap_t &>(*this).begin());
}
/**
 * @brief CBN_SlotMap_t<T>.begin (const)
 */
template <typename T>
typename CBN_SlotMap_t<T>::const_iterator CBN_SlotMap_t<T>::begin(void) const {
  return ((data_type *) &data)->begin();
}
/**
 * @brief CBN_SlotMap_t<T>.end
 */
template <typename T>
typename CBN_SlotMap_t<T>::iterator CBN_SlotMap_t<T>::end(void) {
  return const_cast<iterator>(static_cast<const CBN_SlotMap_t &>(*this).end());
}
/**
 * @brief CBN_SlotMap_t<T>.end (const)
 */
template <typename T>
typename CBN_SlotMap_t<T>::const_iterator CBN_SlotMap_t<T>::end(void) const {
  return ((data_type *) &data)->end();
}
/**
 * @brief CBN_SlotMap_t<T>[key]
 */
template <typename T>
cbn::Opt<typename CBN_SlotMap_t<T>::value_type> CBN_SlotMap_t<T>::operator[](const CBN_SlotMap_Key key) {
  return static_cast<const CBN_SlotMap_t &>(*this)[key];
}
/**
 * @brief CBN_SlotMap_t<T>[key] (const)
 */
template <typename T>
cbn::Opt<const typename CBN_SlotMap_t<T>::value_type> CBN_SlotMap_t<T>::operator[](const CBN_SlotMap_Key key) const {
  value_type x;
  if (!carbon_slotmap_lookup((CBN_SlotMap *) this, key, &x)) return {};
  return x;
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
