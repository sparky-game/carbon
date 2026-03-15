/*
  $$============================================$$
  ||       Random Number Generators (RNG)       ||
  $$============================================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Generates a pseudo-random 32-bit unsigned integer using a Linear Congruential Generator (LCG).
 *
 * The generator is lazily initialized (upon the first call) using a high-entropy seed derived
 * from `carbon_time_get`. It satisfies the Hull-Dobell Theorem, thus it has full maximum period of 2^32.
 *
 * @return A pseudo-random 32-bit unsigned integer X, such that 0 ≤ X ≤ 2^32-1.
 */
CBNDEF u32 carbon_rng_lcg(void);
CBNDEF_AKA(cbn::rng, LCG, carbon_rng_lcg);

/**
 * @brief Generates a pseudo-random 32-bit unsigned integer using a LCG within a specified range.
 * @see carbon_rng_lcg
 * @param min The inclusive lower bound of the desired range.
 * @param max The inclusive upper bound of the desired range.
 * @return A pseudo-random 32-bit unsigned integer X, such that min ≤ X ≤ max.
 */
CBNDEF u32 carbon_rng_lcg_range(u32 min, u32 max);
CBNDEF_AKA(cbn::rng, LCGr, carbon_rng_lcg_range);

/**
 * @brief Generates a pseudo-random 32-bit floating-point using a LCG.
 * @see carbon_rng_lcg
 * @return A pseudo-random 32-bit floating-point X, such that 0 ≤ X < 1.
 */
CBNDEF f32 carbon_rng_lcgf(void);
CBNDEF_AKA(cbn::rng, LCGf, carbon_rng_lcgf);

/**
 * @brief Generates a pseudo-random 32-bit floating-point using a LCG within a specified range.
 * @see carbon_rng_lcgf
 * @param min The inclusive lower bound of the desired range.
 * @param max The exclusive upper bound of the desired range.
 * @return A pseudo-random 32-bit floating-point X, such that min ≤ X < max.
 */
CBNDEF f32 carbon_rng_lcgf_range(f32 min, f32 max);
CBNDEF_AKA(cbn::rng, LCGfr, carbon_rng_lcgf_range);

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

CBNDEF void carbon_rng_mt19937_64_srand(u64 seed);

CBNDEF u64 carbon_rng_mt19937_64_rand(void);
CBNDEF_AKA(cbn::rng, MT19937, carbon_rng_mt19937_64_rand);

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

/**
 * @brief Generates a psueudo-random boolean following a Bernoulli distribution.
 * @param gen A uniform RNG function returning X, such that 0 ≤ X < 1.
 * @param p The probability of success (i.e. true), in [0..1].
 * @return true with probability p, false with probability 1-p.
 */
CBNDEF bool carbon_rng_bernoulli_dist(f32(*gen)(void), f32 p);
CBNDEF_AKA(cbn::rng, BernoulliDist, carbon_rng_bernoulli_dist);

/**
 * @brief Generates a psuedo-random 32-bit floating-point following a Normal (Gaussian) distribution.
 * @param gen A uniform RNG function returning X, such that 0 ≤ X < 1.
 * @param mu The mean (μ) of the distribution, i.e. the center of symmetry of the bell curve.
 *           It shifts the output range, so the returned value is centered around μ.
 * @param sigma The standard deviation (σ) of the distribution. It controls the dispersion of the
 *              output: ~68% of samples fall within [μ-σ, μ+σ], ~95% within [μ-2σ, μ+2σ], and
 *              ~99.7% within [μ-3σ, μ+3σ]. A larger σ produces a wider, flatter bell curve; a
 *              smaller σ produces a narrower, taller one.
 * @return A A pseudo-random 32-bit floating-point in (-∞, +∞), distributed as N(μ, σ²).
 */
CBNDEF f32 carbon_rng_normal_dist(f32(*gen)(void), f32 mu, f32 sigma);
CBNDEF_AKA(cbn::rng, NormalDist, carbon_rng_normal_dist);

/**
 * @brief ...
 * @param expr ...
 * @return ...
 */
CBNDEF u32 carbon_rng_roll_dice(const char *expr);
CBNDEF_AKA(cbn::rng, RollDice, carbon_rng_roll_dice);
