/*
**  $$==================$$
**  ||       Math       ||
**  $$==================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_E        2.71828182845904523536
#define CARBON_E_0_5    1.64872127070012814684
#define CARBON_E_0_25   1.28402541668774148407
#define CARBON_E_0_125  1.13314845306682631682
#define CARBON_PI       3.14159265358979323846
#define CARBON_PI_2     1.57079632679489661923
#define CARBON_2PI      6.28318530717958647692
#define CARBON_PI_3     1.04719755119659774615
#define CARBON_3PI      9.42477796076937971538
#define CARBON_PI_4     0.78539816339744830961
#define CARBON_4PI      12.56637061435917295384
#define CARBON_PI_6     0.52359877559829887307
#define CARBON_6PI      18.84955592153875943076
#define CARBON_1_PI     0.31830988618379067153
#define CARBON_1_2PI    0.15915494309189533576
#define CARBON_SQRT2    1.41421356237309504880
#define CARBON_1_SQRT2  0.7071067811865475244
#define CARBON_SQRT3    1.73205080756887729352
#define CARBON_SQRT3_2  0.86602540378443864676
#define CARBON_1_SQRT3  0.57735026918962576451
#define CARBON_LOG2_E   1.4426950408889634074
#define CARBON_LOG2_10  3.32192809488736234789
#define CARBON_EPS      1e-6
#define CARBON_EPS_SQ   1e-12
#define CARBON_INF      1e40

/**
 * @brief Returns the smaller of two numbers.
 * @param x The first number.
 * @param y The second number.
 */
#define CARBON_MIN(x, y) (x < y ? x : y)

/**
 * @brief Returns the smaller of three numbers.
 * @param x The first number.
 * @param y The second number.
 * @param z The third number.
 */
#define CARBON_MIN3(x, y, z) CARBON_MIN(x, CARBON_MIN(y, z))

/**
 * @brief Returns the larger of two numbers.
 * @param x The first number.
 * @param y The second number.
 */
#define CARBON_MAX(x, y) (x > y ? x : y)

/**
 * @brief Returns the larger of three numbers.
 * @param x The first number.
 * @param y The second number.
 * @param z The third number.
 */
#define CARBON_MAX3(x, y, z) CARBON_MAX(x, CARBON_MAX(y, z))

/**
 * @brief Returns an integer {-1, 0, 1} that indicates the sign of a number.
 * @param x The number.
 */
#define CARBON_SIGN(x) (!x ? 0 : x < 0 ? -1 : 1)

/**
 * @brief Converts degrees to radians.
 * @param angle The angle in degrees.
 */
#define CARBON_TO_RADIANS(angle) ((angle) * CARBON_PI / 180)

/**
 * @brief Reverses the order of the bytes in the provided 32-bit unsigned integer (e.g. 0xAABBCCDD -> 0xDDCCBBAA).
 * @param x The 32-bit unsigned integer.
 * @return The byte-reversed 32-bit unsigned integer.
 */
CBNDEF u32 carbon_math_bswap32(u32 x);

/**
 * @brief Returns the absolute value of a specified number.
 * @param x The number.
 * @return The non-negative value of x without regard to its sign.
 */
CBNDEF f32 carbon_math_abs(f32 x);
CBNDEF_FN(cbn::math, Abs, carbon_math_abs);

/**
 * @brief Rounds a number to the nearest integral value.
 * @param x The number.
 * @return The rounded integral value of x.
 */
CBNDEF f32 carbon_math_round(f32 x);
CBNDEF_FN(cbn::math, Round, carbon_math_round);

/**
 * @brief Returns the largest integral value less than or equal to the specified number.
 * @param x The number.
 * @return The largest integral value ≤ x.
 */
CBNDEF f32 carbon_math_floor(f32 x);

/**
 * @brief Returns the smallest integral value great than or equal to the specified number.
 * @param x The number.
 * @return The smallest integral value ≥ x.
 */
CBNDEF f32 carbon_math_ceil(f32 x);

/**
 * @brief Restricts a number between a minimum and a maximum value.
 * @param x The number.
 * @param min The minimum value.
 * @param max The maximum value.
 */
CBNDEF f32 carbon_math_clamp(f32 x, f32 min, f32 max);
CBNDEF_FN(cbn::math, Clamp, carbon_math_clamp);

/**
 * @brief Snaps a number to the next integer up or down, depending on whether Δx is positive or negative.
 * @param x The number.
 * @param dx A directional delta (Δ). Only its sign is used to determine the direction of the snap.
 * @return The snapped number.
 */
CBNDEF f32 carbon_math_snap(f32 x, f32 dx);

/**
 * @brief Returns the square root of a specified number.
 * @param x The number.
 * @return The positive square root of x, provided x ≥ 0.
 */
CBNDEF f32 carbon_math_sqrt(f32 x);

/**
 * @brief Returns the reciprocal (multiplicative inverse) of the square root of a specified number.
 * @param x The number.
 * @return An approximation of 1/√x, provided x ≥ 0.
 */
CBNDEF f32 carbon_math_rsqrt(f32 x);

/**
 * @brief Performs a true modulo operation for integers.
 * @param x The dividend.
 * @param y The divisor.
 * @return The remainder r of x/y, such that 0 ≤ r < |y|.
 */
CBNDEF i32 carbon_math_imod(i32 x, i32 y);

/**
 * @brief Performs a true modulo operation for floating-point numbers.
 * @param x The dividend.
 * @param y The divisor.
 * @return The remainder r of x/y, such that 0 ≤ r < |y|.
 */
CBNDEF f32 carbon_math_fmod(f32 x, f32 y);

/**
 * @brief Returns a specified number raised to the specified power.
 * @param x The number.
 * @param y The power.
 * @return The result of x^y.
 */
CBNDEF f32 carbon_math_pow(f32 x, f32 y);

/**
 * @brief Returns the base-2 logarithm of the specified number.
 * @param x The number.
 * @return The result of log₂(x).
 */
CBNDEF f32 carbon_math_log2(f32 x);

/**
 * @brief Returns 2 raised to the specified power.
 * @param x The power.
 * @return The result of 2^x.
 */
CBNDEF f32 carbon_math_exp2(f32 x);

/**
 * @brief Multiplies a number by an integer power of 2.
 * @param x The number.
 * @param n The power.
 * @return The result of x * 2^n.
 */
CBNDEF f32 carbon_math_ldexp2(f32 x, i32 n);

/**
 * @brief Returns the base-e (natural) logarithm of the specified number.
 * @param x The number.
 * @return The result of ln(x).
 */
CBNDEF f32 carbon_math_log(f32 x);

/**
 * @brief Returns e raised to the specified power.
 * @param x The power.
 * @return The result of e^x.
 */
CBNDEF f32 carbon_math_exp(f32 x);
CBNDEF_FN(cbn::math, Exp, carbon_math_exp);

/**
 * @brief Multiplies a number by an integer power of e.
 * @param x The number.
 * @param n The power.
 * @return The result of x * e^n.
 */
CBNDEF f32 carbon_math_ldexp(f32 x, i32 n);

/**
 * @brief Returns the base-10 logarithm of the specified number.
 * @param x The number.
 * @return The result of log₁₀(x).
 */
CBNDEF f32 carbon_math_log10(f32 x);

/**
 * @brief Returns 10 raised to the specified power.
 * @param x The power.
 * @return The result of 10^x.
 */
CBNDEF f32 carbon_math_exp10(f32 x);

/**
 * @brief Multiplies a number by an integer power of 10.
 * @param x The number.
 * @param n The power.
 * @return The result of x * 10^n.
 */
CBNDEF f32 carbon_math_ldexp10(f32 x, i32 exp);

/**
 * @brief Breaks a number into a normalized fraction and an integer power of 2.
 * @param x The number.
 * @param n The power (output argument pointer).
 * @return A number y, such that 1/2 ≤ y < 1 and x = y * 2^n.
 */
CBNDEF f32 carbon_math_frexp(f32 x, i32 *n);

/**
 * @brief Computes the logistic function (S-shaped sigmoid function/curve), mapping a number to a value in [0..1].
 * @param x The number.
 * @return The result of the application of the function.
 */
CBNDEF f32 carbon_math_sigmoid(f32 x);

/**
 * @brief Performs linear interpolation from a to b by factor t.
 * @param a The start value.
 * @param b The end value.
 * @param t The interpolation factor in [0..1].
 * @return The interpolated value between a and b.
 */
CBNDEF f32 carbon_math_lerp(f32 a, f32 b, f32 t);
CBNDEF_FN(cbn::math, Lerp, carbon_math_lerp);

/**
 * @brief Computes the interpolation factor of x between two values following 3t²-2t³.
 * @param a The start of the range.
 * @param b The end of the range.
 * @param x The source value for interpolation.
 * @return The interpolation factor in [0..1].
 */
CBNDEF f32 carbon_math_smoothstep(f32 a, f32 b, f32 x);
CBNDEF_FN(cbn::math, SmoothStep, carbon_math_smoothstep);

/**
 * @brief Compares two numbers with a floating-point tolerance (ε).
 * @param x The first number.
 * @param y The second number.
 * @return An indicator of the comparison result:
 *   - If x < y, then -1.
 *   - If x > y, then +1.
 *   - If x ± ε ≈ y, then 0.
 */
CBNDEF i8 carbon_math_cmp(f32 x, f32 y);

/**
 * @brief Concatenates two unsigned integers together (e.g. `10` || `69` :: `1069`).
 * @param x The first unsigned integer.
 * @param y The second unsigned integer.
 * @return The unsigned integer representing their concatenation.
 */
CBNDEF u64 carbon_math_concat(u64 x, u64 y);

/**
 * @brief Returns the Greatest Common Divisor (using Euclidean algorithm) of two integers.
 * @param x The first integer.
 * @param y The second integer.
 * @return The GCD of `x` and `y`.
 */
CBNDEF i32 carbon_math_egcd(i32 x, i32 y);

/**
 * @brief Returns the sine of the specified angle.
 * @param x The angle in radians.
 * @return The sine of x.
 */
CBNDEF f32 carbon_math_sin(f32 x);
CBNDEF_FN(cbn::math, Sin, carbon_math_sin);

/**
 * @brief Returns the cosine of the specified angle.
 * @param x The angle in radians.
 * @return The cosine of x.
 */
CBNDEF f32 carbon_math_cos(f32 x);
CBNDEF_FN(cbn::math, Cos, carbon_math_cos);

/**
 * @brief Returns the tangent of the specified angle.
 * @param x The angle in radians.
 * @return The tangent of x.
 */
CBNDEF f32 carbon_math_tan(f32 x);
CBNDEF_FN(cbn::math, Tan, carbon_math_tan);

/**
 * @brief Returns the hyperbolic tangent of the specified angle.
 * @param x The angle in radians.
 * @return The hyperbolic tangent of x.
 */
CBNDEF f32 carbon_math_tanh(f32 x);
CBNDEF_FN(cbn::math, Tanh, carbon_math_tanh);

/**
 * @brief Returns the angle whose sine is the specified number.
 * @param x The number representing a sine, such that -1 ≤ x ≤ 1.
 * @return An angle θ, in radians, such that -π/2 ≤ θ ≤ π/2.
 */
CBNDEF f32 carbon_math_asin(f32 x);

/**
 * @brief Returns the angle whose tangent is the specified number.
 * @param x The number representing a tangent.
 * @return An angle θ, in radians, such that -π/2 ≤ θ ≤ π/2.
 */
CBNDEF f32 carbon_math_atan(f32 x);
CBNDEF_FN(cbn::math, Atan, carbon_math_atan);

/**
 * @brief Returns the angle whose tangent is the quotient of the two specified numbers.
 * @param y The Y coordinate of a point.
 * @param x The X coordinate of a point.
 * @return An angle θ, in radians, such that tan(θ) = y/x, where (x, y) is a point in the Cartesian plane.
 */
CBNDEF f32 carbon_math_atan2(f32 y, f32 x);
CBNDEF_FN(cbn::math, Atan2, carbon_math_atan2);
