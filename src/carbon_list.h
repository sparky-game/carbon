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
  using iterator = value_type *;
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
   * @brief carbon_list_shrink_to_fit
   */
  void ShrinkToFit(void);
  /**
   * @brief Returns an iterator to the beginning.
   * @return The iterator.
   */
  iterator begin(void) const;
  /**
   * @brief Returns an iterator to the end.
   * @return The iterator.
   */
  iterator end(void) const;
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
#define carbon_list_at(T, l, i) (CBN_ASSERT((i) < (l).size && "List index out of bounds"), CBN_ASSERT(sizeof(T) == (l).stride && "List type doesn't match"), carbon_list_at_raw(T, l, i))
#define carbon_list_foreach(T, l) for (struct { usz i; T var; } it = {0, carbon_list_at(T, l, 0)}; it.i < (l).size; ++it.i, it.i < (l).size ? it.var = carbon_list_at(T, l, it.i) : it.var)

/**
 * @brief Creates a new list container.
 * @param stride Size in bytes of the elements the list will hold.
 * @return The list container.
 */
CARBON_API CBN_List carbon_list_create(usz stride);

/**
 * @brief Destroys a list container.
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
 * @param out_value The value of the element popped out (output argument pointer).
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

/**
 * @brief Performs a non-binding request to remove the unused capacity, by reducing capacity to size.
 * @param l The list container.
 */
CARBON_API void carbon_list_shrink_to_fit(CBN_List *l);

// Local Variables:
// mode: c++
// End:
