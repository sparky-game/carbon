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
 * @brief Restricts a number between a minimum and a maximum value.
 * @param x The number.
 * @param min The minimum value.
 * @param max The maximum value.
 */
#define CARBON_CLAMP(x, min, max) ((x <= min) ? min : (x >= max) ? max : x)

/**
 * @brief Returns an integer {-1, 0, 1} that indicates the sign of a number.
 * @param x The number.
 */
#define CARBON_SIGN(x) (!x ? 0 : x < 0 ? -1 : 1)

#define CARBON_STEP(edge, x) (x < edge ? 0 : 1)
#define CARBON_SWAP(T, x, y) do { T z = x; x = y; y = z; } while (0)
#define CARBON_LERP(a, b, t) ((a) + ((b) - (a)) * (t))

/**
 * @brief Converts degrees to radians.
 * @param angle The angle in degrees.
 */
#define CARBON_TO_RADIANS(angle) ((angle) * CARBON_PI / 180)

// TODO: somehow perform bounds checking `CBN_ASSERT(0 <= (i) && (i) < (m).rows && 0 <= (j) && (j) < (m).cols && "Matrix index out of bounds")`
#define CARBON_MAT_AT(m, i, j) (m).items[(i) * (m).cols + (j)]

// TODO: somehow perform bounds checking `CBN_ASSERT(0 <= (i) && (i) < (r).cols && "Row index out of bounds")`
#define CARBON_ROW_AT(r, i) (r).items[(i)]

#define CARBON_MAT_PRINT(m) carbon_math_mat_print(m, #m)
#define CARBON_ROW_PRINT(r) carbon_math_row_print(r, #r)

typedef struct {
  f32 *items;
  usz rows;
  usz cols;
} CBN_Matrix;

typedef struct {
  f32 *items;
  usz cols;
} CBN_Row;

CARBON_API void carbon_math_srand(u64 seed);
CARBON_API i32 carbon_math_rand(void);
CARBON_API i32 carbon_math_rand_between(i32 min, i32 max);
CARBON_API f32 carbon_math_randf(void);
CARBON_API f32 carbon_math_randf_between(f32 min, f32 max);
CARBON_API void carbon_math_mt19937_64_srand(u64 seed);
CARBON_API u64 carbon_math_mt19937_64_rand(void);
CARBON_API f32 carbon_math_abs(f32 x);
CARBON_API f32 carbon_math_round(f32 x);
CARBON_API f32 carbon_math_floor(f32 x);
CARBON_API f32 carbon_math_ceil(f32 x);
CARBON_API f32 carbon_math_snap(f32 x, f32 dx);
CARBON_API f32 carbon_math_sqrt(f32 x);
CARBON_API i32 carbon_math_imod(i32 x, i32 y);
CARBON_API f32 carbon_math_fmod(f32 x, f32 y);
CARBON_API f32 carbon_math_pow(f32 x, f32 y);
CARBON_API f32 carbon_math_log2(f32 x);
CARBON_API f32 carbon_math_exp2(f32 x);
CARBON_API f32 carbon_math_ldexp2(f32 x, i32 exp);
CARBON_API f32 carbon_math_log(f32 x);
CARBON_API f32 carbon_math_exp(f32 x);
CARBON_API f32 carbon_math_ldexp(f32 x, i32 exp);
CARBON_API f32 carbon_math_log10(f32 x);
CARBON_API f32 carbon_math_exp10(f32 x);
CARBON_API f32 carbon_math_ldexp10(f32 x, i32 exp);
CARBON_API f32 carbon_math_frexp(f32 x, i32 *exp);
CARBON_API f32 carbon_math_sigmoid(f32 x);
CARBON_API f32 carbon_math_tanh(f32 x);
CARBON_API f32 carbon_math_smoothstep(f32 a, f32 b, f32 t);
CARBON_API i8 carbon_math_cmp(f32 x, f32 y);

/**
 * @brief Concatenates two unsigned integers together (e.g. `10` || `69` :: `1069`).
 * @param x The first unsigned integer.
 * @param y The second unsigned integer.
 * @return The unsigned integer representing their concatenation.
 */
CARBON_API u64 carbon_math_concat(u64 x, u64 y);

CARBON_API f32 carbon_math_sin(f32 x);
CARBON_API f32 carbon_math_cos(f32 x);
CARBON_API f32 carbon_math_asin(f32 x);
CARBON_API f32 carbon_math_atan(f32 x);
CARBON_API f32 carbon_math_atan2(f32 y, f32 x);

CARBON_API CBN_Matrix carbon_math_mat_create(usz rows, usz cols);
CARBON_API void carbon_math_mat_destroy(CBN_Matrix *m);
CARBON_API void carbon_math_mat_fill(CBN_Matrix m, f32 x);
CARBON_API void carbon_math_mat_rand(CBN_Matrix m, f32 min, f32 max);
CARBON_API CBN_Row carbon_math_mat_row(CBN_Matrix m, usz row);
CARBON_API void carbon_math_mat_copy(CBN_Matrix dst, CBN_Matrix src);
CARBON_API void carbon_math_mat_add(CBN_Matrix dst, CBN_Matrix m);
CARBON_API void carbon_math_mat_dot(CBN_Matrix dst, CBN_Matrix a, CBN_Matrix b);
CARBON_API void carbon_math_mat_map(CBN_Matrix m, f32 (*f)(f32));
CARBON_API void carbon_math_mat_print(CBN_Matrix m, const char *name);
CARBON_API CBN_Row carbon_math_row_create(usz cols);
CARBON_API void carbon_math_row_destroy(CBN_Row *r);
CARBON_API CBN_Matrix carbon_math_row_to_mat(CBN_Row r);
CARBON_API CBN_Row carbon_math_row_slice(CBN_Row r, usz i, usz cols);
CARBON_API void carbon_math_row_fill(CBN_Row r, f32 x);
CARBON_API void carbon_math_row_rand(CBN_Row r, f32 min, f32 max);
CARBON_API void carbon_math_row_copy(CBN_Row dst, CBN_Row src);
CARBON_API void carbon_math_row_print(CBN_Row r, const char *name);

// Local Variables:
// mode: c++
// End:
