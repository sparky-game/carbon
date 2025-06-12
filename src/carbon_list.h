/*
**  $$==================$$
**  ||       List       ||
**  $$==================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a sequence container that encapsulates dynamic size arrays.
 *
 * In C++, this is a templated class/struct, which means it's not a type by itself,
 * until it gets instantiated with the needed template arguments. The type `CBN_List`
 * is an alias for `CBN_List_t<void *>`, which provides full-compatibility with the
 * C API, even through C++ templates.
 *
 * @param T Type information of what will be stored in the container.
 */
CARBON_TEMPLATE_CLASS(CBN_List_t) {
  void *items;
  usz capacity;
  usz stride;
  usz size;
#ifdef __cplusplus
  using value_type = T;
  using iterator = T *;
  using const_iterator = const T *;
  /**
   * @brief carbon_list_create
   * @return The list container.
   */
  static CBN_List_t make(void);
  /**
   * @brief carbon_list_destroy
   */
  void Free(void);
  /**
   * @brief carbon_list_push
   * @param value The value to append.
   */
  void Push(const value_type &value);
  /**
   * @brief carbon_list_pop
   * @return The value of the element popped out.
   */
  value_type Pop(void);
  /**
   * @brief carbon_list_find
   * @param value The value of the element to check.
   * @return The index of the provided element, or -1 if not present.
   */
  isz Find(const value_type &value) const;
  /**
   * @brief carbon_list_remove
   * @param idx The index of the element to remove.
   */
  void Remove(usz idx);
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
  value_type &operator[](usz idx);
  const value_type &operator[](usz idx) const;
#endif
};

#ifdef __cplusplus
using CBN_List = CBN_List_t<void *>;
#else
typedef struct CBN_List_t CBN_List;
#endif

#define carbon_list_at_raw(T, l, i) ((T *) (l).items)[(i)]
#define carbon_list_at(T, l, i) (CARBON_ASSERT(0 <= (i32) (i) && (i) < (l).size && "List index out of bounds"), CARBON_ASSERT(sizeof(T) == (l).stride && "List type doesn't match"), carbon_list_at_raw(T, l, i))
#define carbon_list_foreach(T, l) for (struct { usz i; T var; } it = {0, carbon_list_at(T, l, 0)}; it.i < (l).size; ++it.i, it.i < (l).size ? it.var = carbon_list_at(T, l, it.i) : it.var)

/**
 * @brief Create a new list container.
 * @param stride Size in bytes of the elements the list will hold.
 * @return The list container.
 */
CARBON_API CBN_List carbon_list_create(usz stride);

/**
 * @brief Destroy a list container.
 * @param l The list container.
 */
CARBON_API void carbon_list_destroy(CBN_List *l);

/**
 * @brief Appends a copy of the value to the end of the list.
 * @param l The list container.
 * @param value The value to append.
 */
CARBON_API void carbon_list_push(CBN_List *l, void *value);

/**
 * @brief Removes the last element from the list (LIFO).
 * @param l The list container.
 * @param out_value The value of the element popped out.
 */
CARBON_API void carbon_list_pop(CBN_List *l, void *out_value);

/**
 * @brief Obtains the index of the provided element, or -1 if not present.
 * @param l The list container.
 * @param value The value of the element to check.
 * @return The index of the provided element, or -1 if not present.
 */
CARBON_API isz carbon_list_find(const CBN_List *l, const void *value);

/**
 * @brief Removes the element specified by the provided index from the list.
 * @param l The list container.
 * @param idx The index of the element to remove.
 */
CARBON_API void carbon_list_remove(CBN_List *l, usz idx);

// Because `CBN_List_t` is a templated class/struct, the definitions of all its
// member functions need to be available at compile-time, not link-time; thus,
// we define them here for the template instantiation to work properly.
#ifdef __cplusplus
/**
 * @brief CBN_List_t<T>::make
 */
template <typename T>
CBN_List_t<T> CBN_List_t<T>::make(void) {
  auto l = carbon_list_create(sizeof(T));
  return *(CBN_List_t<T> *) &l;
}
/**
 * @brief CBN_List_t<T>.Free
 */
template <typename T>
void CBN_List_t<T>::Free(void) {
  carbon_list_destroy((CBN_List *) this);
}
/**
 * @brief CBN_List_t<T>.Push
 */
template <typename T>
void CBN_List_t<T>::Push(const value_type &value) {
  carbon_list_push((CBN_List *) this, (void *) &value);
}
/**
 * @brief CBN_List_t<T>.Pop
 */
template <typename T>
typename CBN_List_t<T>::value_type CBN_List_t<T>::Pop(void) {
  value_type x;
  carbon_list_pop((CBN_List *) this, &x);
  return x;
}
/**
 * @brief CBN_List_t<T>.Find
 */
template <typename T>
isz CBN_List_t<T>::Find(const value_type &value) const {
  return carbon_list_find((CBN_List *) this, (void *) &value);
}
/**
 * @brief CBN_List_t<T>.Remove
 */
template <typename T>
void CBN_List_t<T>::Remove(usz idx) {
  carbon_list_remove((CBN_List *) this, idx);
}
/**
 * @brief CBN_List_t<T>.begin
 */
template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::begin(void) {
  return const_cast<iterator>(static_cast<const CBN_List_t &>(*this).begin());
}
/**
 * @brief CBN_List_t<T>.begin (const)
 */
template <typename T>
typename CBN_List_t<T>::const_iterator CBN_List_t<T>::begin(void) const {
  return static_cast<const_iterator>(items);
}
/**
 * @brief CBN_List_t<T>.end
 */
template <typename T>
typename CBN_List_t<T>::iterator CBN_List_t<T>::end(void) {
  return const_cast<iterator>(static_cast<const CBN_List_t &>(*this).end());
}
/**
 * @brief CBN_List_t<T>.end (const)
 */
template <typename T>
typename CBN_List_t<T>::const_iterator CBN_List_t<T>::end(void) const {
  return static_cast<const_iterator>(items) + size;
}
/**
 * @brief CBN_List_t<T>[idx]
 */
template <typename T>
typename CBN_List_t<T>::value_type &CBN_List_t<T>::operator[](usz idx) {
  return const_cast<value_type &>(static_cast<const CBN_List_t &>(*this)[idx]);
}
/**
 * @brief CBN_List_t<T>[idx] (const)
 */
template <typename T>
const typename CBN_List_t<T>::value_type &CBN_List_t<T>::operator[](usz idx) const {
  CARBON_ASSERT(0 <= idx && idx < size && "List index out of bounds");
  CARBON_ASSERT(sizeof(value_type) == stride && "List type doesn't match");
  return ((value_type *) items)[idx];
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
