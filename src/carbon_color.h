/*
**  $$===================$$
**  ||       Color       ||
**  $$===================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Transforms HSV color into RGB (A always set to 0xff).
 * @param h Hue, in [0..360] range.
 * @param s Saturation, in [0..1] range.
 * @param v Value, in [0..1] range.
 * @return 32-bit RGBA color value (A always set to 0xff).
 */
CBNDEF u32 carbon_color_from_hsv(f32 h, f32 s, f32 v);

/**
 * @brief Transforms 32-bit RGBA color into HSV.
 * @param color The color.
 * @return 3D vector holding the HSV data:
 *   .x := Hue, in [0..360] range.
 *   .y := Saturation, in [0..1] range.
 *   .z := Value, in [0..1] range.
 */
CBNDEF CBN_Vec3 carbon_color_to_hsv(u32 color);

/**
 * @brief Scales the 32-bit RGBA color by the specified scalar value (A is left untouched).
 * @param color The color.
 * @param s The scalar value.
 * @return The scaled 32-bit RGBA color.
 */
CBNDEF u32 carbon_color_scale(u32 color, f32 s);

/**
 * @brief Adds up two 32-bit RGBA colors together.
 * @param c1 The first color.
 * @param c2 The second color.
 * @return The resultant 32-bit RGBA color.
 */
CBNDEF u32 carbon_color_add(u32 c1, u32 c2);

/**
 * @brief Computes the complementary color of the one provided (32-bit RGBA).
 * @param color The color.
 * @return The complementary 32-bit RGBA color.
 */
CBNDEF u32 carbon_color_complementary(u32 color);
