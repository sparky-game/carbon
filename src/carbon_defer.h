/*
**  $$===================$$
**  ||       Defer       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 */
#ifdef __cplusplus
#define defer [[maybe_unused]] const auto carbon_defer__var = carbon_defer__adder + [&]()
#else
// TODO: implement the `defer` keyword in C
#endif

// Local Variables:
// mode: c++
// End:
