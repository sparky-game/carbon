/*
  $$===================$$
  ||       Color       ||
  $$===================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Transforms HSV color into RGB (A always set to 0xff).
 * @param h Hue, in [0..360] range.
 * @param s Saturation, in [0..1] range.
 * @param v Value, in [0..1] range.
 * @return 32-bit RGBA color value (A always set to 0xff).
 */
CBNDEF u32 carbon_color_from_hsv(f32 h, f32 s, f32 v);
CBNDEF_FN(cbn::color, FromHSV, carbon_color_from_hsv);

/**
 * @brief Transforms 32-bit RGBA color into HSV.
 * @param color The color.
 * @return 3D vector holding the HSV data:
 *   .x := Hue, in [0..360] range.
 *   .y := Saturation, in [0..1] range.
 *   .z := Value, in [0..1] range.
 */
CBNDEF CBN_Vec3 carbon_color_to_hsv(u32 color);
CBNDEF_FN(cbn::color, ToHSV, carbon_color_to_hsv);

/**
 * @brief Scales the 32-bit RGBA color by the specified scalar value (A is left untouched).
 * @param color The color.
 * @param s The scalar value.
 * @return The scaled 32-bit RGBA color.
 */
CBNDEF u32 carbon_color_scale(u32 color, f32 s);
CBNDEF_FN(cbn::color, Scale, carbon_color_scale);

/**
 * @brief Adds up two 32-bit RGBA colors together.
 * @param c1 The first color.
 * @param c2 The second color.
 * @return The resultant 32-bit RGBA color.
 */
CBNDEF u32 carbon_color_add(u32 c1, u32 c2);
CBNDEF_FN(cbn::color, Add, carbon_color_add);

/**
 * @brief Performs linear interpolation from c1 to c2 by factor t.
 * @param c1 The first color.
 * @param c2 The second color.
 * @param t The interpolation factor in [0..1].
 * @return The interpolated color between c1 and c2.
 */
CBNDEF u32 carbon_color_lerp(u32 c1, u32 c2, f32 t);
CBNDEF_FN(cbn::color, Lerp, carbon_color_lerp);

/**
 * @brief Computes the complementary color of the one provided (32-bit RGBA).
 * @param color The color.
 * @return The complementary 32-bit RGBA color.
 */
CBNDEF u32 carbon_color_complementary(u32 color);
CBNDEF_FN(cbn::color, Complementary, carbon_color_complementary);

/**
 * @brief Transforms 32-bit RGBA color into normalized 4D vector.
 * @param color The color.
 * @return The resultant normalized RGBA color (as 4D vector).
 */
CBNDEF CBN_Vec4 carbon_color_to_normalized(u32 color);
CBNDEF_FN(cbn::color, ToNormalized, carbon_color_to_normalized);

/**
 * @brief Transforms normalized 4D vector into 32-bit RGBA color.
 * @param v The normalized RGBA color (as 4D vector).
 * @return The resultant 32-bit RGBA color.
 */
CBNDEF u32 carbon_color_from_normalized(CBN_Vec4 v);
CBNDEF_FN(cbn::color, FromNormalized, carbon_color_from_normalized);
