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
#ifdef __cplusplus
template <typename T>
#endif
struct CBN_List_t {
  void *items;
  usz capacity;
  usz stride;
  usz size;
#ifdef __cplusplus
  using value_type = T;
  using iterator = value_type *;
  /**
   * @see carbon_list_create
   */
  static CBN_List_t make(void);
  /**
   * @see carbon_list_destroy
   */
  void Free(void);
  /**
   * @see carbon_list_push
   */
  void Push(const value_type &value);
  /**
   * @see carbon_list_pop
   */
  value_type Pop(void);
  /**
   * @see carbon_list_find
   */
  isz Find(const value_type &value) const;
  /**
   * @see carbon_list_remove
   */
  void Remove(usz idx);
  /**
   * @see carbon_list_shrink_to_fit
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
  /**
   * @see carbon_list_at
   */
  value_type &operator[](usz idx);
#endif
};
#ifdef __cplusplus
using CBN_List = CBN_List_t<void *>;
#else
typedef struct CBN_List_t CBN_List;
#endif

/**
 * @brief Gets the element in the provided List at a specific index.
 * @param T The underlying type the List was created to handle.
 * @param l The List container.
 * @param i The index to access.
 */
#define carbon_list_at_raw(T, l, i) ((T *) (l).items)[(i)]
#define carbon_list_at(T, l, i) (CBN_ASSERT((i) < (l).size && "List index out of bounds"), CBN_ASSERT(sizeof(T) == (l).stride && "List type doesn't match"), carbon_list_at_raw(T, l, i))

/**
 * @brief Loops through the provided List with a custom-defined iterator.
 *
 * The iterator consists of the following elements:
 *   - `i`  :: The index of the actual element being evaluated.
 *   - `var` :: The actual element being evaluated, as an object of type T.
 * @param T The underlying type the List was created to handle.
 * @param name The name to give to the iterator (optional; default = `it`).
 * @param l The List container.
 */
#define carbon_list_foreach__named(T, name, l) for (struct { usz i; T var; } name = {0, carbon_list_at(T, l, 0)}; name.i < (l).size; ++name.i, name.i < (l).size ? name.var = carbon_list_at(T, l, name.i) : name.var)
#define carbon_list_foreach__default(T, l) carbon_list_foreach__named(T, it, l)
#define carbon_list_foreach__dispatcher(_1, _2, _3, NAME, ...) NAME
#define carbon_list_foreach(...) carbon_list_foreach__dispatcher(__VA_ARGS__, carbon_list_foreach__named, carbon_list_foreach__default)(__VA_ARGS__)

/**
 * @brief Creates a new list container.
 * @param stride Size in bytes of the elements the list will hold.
 * @return The list container.
 */
CBNDEF CBN_List carbon_list_create(usz stride);

/**
 * @brief Destroys a list container.
 * @param l The list container.
 */
CBNDEF void carbon_list_destroy(CBN_List *l);

/**
 * @brief Appends a copy of the value to the end of the list.
 * @param l The list container.
 * @param value The value to append.
 */
CBNDEF void carbon_list_push(CBN_List *l, void *value);

/**
 * @brief Removes the last element from the list (LIFO).
 * @param l The list container.
 * @param out_value The value of the element popped out (output argument pointer).
 */
CBNDEF void carbon_list_pop(CBN_List *l, void *out_value);

/**
 * @brief Obtains the index of the provided element, or -1 if not present.
 * @param l The list container.
 * @param value The value of the element to check.
 * @return The index of the provided element, or -1 if not present.
 */
CBNDEF isz carbon_list_find(const CBN_List *l, const void *value);

/**
 * @brief Removes the element specified by the provided index from the list.
 * @param l The list container.
 * @param idx The index of the element to remove.
 */
CBNDEF void carbon_list_remove(CBN_List *l, usz idx);

/**
 * @brief Performs a non-binding request to remove the unused capacity, by reducing capacity to size.
 * @param l The list container.
 */
CBNDEF void carbon_list_shrink_to_fit(CBN_List *l);

// Local Variables:
// mode: c++
// End:
