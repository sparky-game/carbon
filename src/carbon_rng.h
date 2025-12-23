/*
**  $$============================================$$
**  ||       Random Number Generators (RNG)       ||
**  $$============================================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Generates a pseudo-random 32-bit unsigned integer using a Linear Congruential Generator (LCG).
 *
 * The generator is lazily initialized (upon the first call) using a high-entropy seed derived
 * from `carbon_time_get`. It satisfies the Hull-Dobell Theorem, thus it has full maximum period of 2^32.
 *
 * @return A pseudo-random 32-bit unsigned integer X, such that 0 ≤ X ≤ 2^32-1.
 */
CBNDEF u32 carbon_rng_lcg(void);
CBNDEF_FN(cbn::rng, LCG, carbon_rng_lcg);

/**
 * @brief Generates a pseudo-random 32-bit unsigned integer using a LCG within a specified range.
 * @see carbon_rng_lcg
 * @param min The inclusive lower bound of the desired range.
 * @param max The inclusive upper bound of the desired range.
 * @return A pseudo-random 32-bit unsigned integer X, such that min ≤ X ≤ max.
 */
CBNDEF u32 carbon_rng_lcg_range(u32 min, u32 max);
CBNDEF_FN(cbn::rng, LCGr, carbon_rng_lcg_range);

/**
 * @brief Generates a pseudo-random 32-bit floating-point using a LCG.
 * @see carbon_rng_lcg
 * @return A pseudo-random 32-bit floating-point X, such that 0 ≤ X < 1.
 */
CBNDEF f32 carbon_rng_lcgf(void);
CBNDEF_FN(cbn::rng, LCGf, carbon_rng_lcgf);

/**
 * @brief Generates a pseudo-random 32-bit floating-point using a LCG within a specified range.
 * @see carbon_rng_lcgf
 * @param min The inclusive lower bound of the desired range.
 * @param max The exclusive upper bound of the desired range.
 * @return A pseudo-random 32-bit floating-point X, such that min ≤ X < max.
 */
CBNDEF f32 carbon_rng_lcgf_range(f32 min, f32 max);
CBNDEF_FN(cbn::rng, LCGfr, carbon_rng_lcgf_range);

CBNDEF void carbon_rng_mt19937_64_srand(u64 seed);

CBNDEF u64 carbon_rng_mt19937_64_rand(void);
CBNDEF_FN(cbn::rng, MT19937, carbon_rng_mt19937_64_rand);

/**
 * @brief ...
 * @param expr ...
 * @return ...
 */
CBNDEF u32 carbon_rng_roll_dice(const char *expr);
CBNDEF_FN(cbn::rng, RollDice, carbon_rng_roll_dice);
