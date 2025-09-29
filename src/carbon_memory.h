/*
**  $$====================$$
**  ||       Memory       ||
**  $$====================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief ...
 * @param size ...
 * @return ...
 */
CBNDEF void *carbon_memory_alloc(usz size);

/**
 * @brief ...
 * @param p ...
 * @param size ...
 * @return ...
 */
CBNDEF void *carbon_memory_realloc(void *p, usz size);

/**
 * @brief ...
 * @param size ...
 * @return ...
 */
CBNDEF void *carbon_memory_zeroed(usz size);

/**
 * @brief Copies a set of bytes from a memory area to another one.
 * @param dst The destination memory area address.
 * @param src The source memory area address.
 * @param n The amount of bytes to copy from `src`.
 * @return The original value of `dst`.
 */
CBNDEF void *carbon_memory_copy(void * restrict dst, const void * restrict src, usz n);

/**
 * @brief Compares two sets of bytes from two different memory areas.
 * @param v1 The first memory area address.
 * @param v2 The second memory area address.
 * @param n The amount of bytes to compare from both `v1` and `v2`.
 * @return The difference between the first two differing bytes.
 */
CBNDEF i32 carbon_memory_cmp(const void *v1, const void *v2, usz n);

/**
 * @brief Fills a set of bytes from a memory area with a specific value.
 * @param dst The memory area address.
 * @param c The value to fill the memory area with.
 * @param n The amount of bytes to fill from `dst`.
 * @return The same `dst` value.
 */
CBNDEF void *carbon_memory_set(void *dst, i32 c, usz n);

/**
 * @brief ...
 * @param p ...
 */
CBNDEF void carbon_memory_free(void *p);
