/*
**  $$=======================$$
**  ||       Coroutine       ||
**  $$=======================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Obtains the ID of the coroutine that governs the current scope.
 * @return The ID.
 */
CBNDEF usz carbon_coroutine_id(void);

/**
 * @brief Gets the number of coroutines still alive (being asleep is a form of being alive).
 * @return A counter of the currently alive coroutines.
 */
CBNDEF usz carbon_coroutine_alive(void);

/**
 * @brief Creates a new coroutine.
 *
 * It doesn't automatically switch to it, it continues executing the current one.
 * It'll get to it eventually via the `yield` operation.
 *
 * @param f The function containing the body of the coroutine.
 * @param arg The arbitrary amount of arguments passed to `f`.
 */
CBNDEF void carbon_coroutine_go(void (*f)(void *), void *arg);

/**
 * @brief Puts the current coroutine to sleep, and switches execution to the next one.
 */
CBNDEF void carbon_coroutine_yield(void);

/**
 * @brief Yields the current coroutine until a non-blocking socket has data to read.
 * @param fd The file descriptor of a non-blocking socket.
 */
CBNDEF void carbon_coroutine_sleep_read(i32 fd);

/**
 * @brief Yields the current coroutine until a non-blocking socket is ready to accept data to write.
 * @param fd The file descriptor of a non-blocking socket.
 */
CBNDEF void carbon_coroutine_sleep_write(i32 fd);

/**
 * @brief Wakes up a specific coroutine by ID.
 *
 * A coroutine may be sleeping due to `carbon_coroutine_sleep_read` or
 * `carbon_coroutine_sleep_write` function calls;
 *
 * @param id The ID.
 */
CBNDEF void carbon_coroutine_wakeup(usz id);
