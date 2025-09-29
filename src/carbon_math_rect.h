/*
**  $$=========================$$
**  ||       Math (Rect)       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Defines an inline rectangle.
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 */
#define CARBON_RECT(x, y, w, h) (CBN_Rect){(f32)(x), (f32)(y), (f32)(w), (f32)(h)}

/**
 * @brief Defines an inline rectangle.
 * @param v The 2D vector to assign to the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 */
#define CARBON_RECT_V(v, w, h) CARBON_RECT(v.x, v.y, w, h)

/**
 * @brief Defines an inline rectangle whose width and height are equal (square).
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param sz The value to assign to both the width and the height of the size.
 */
#define CARBON_RECT_SQUARE(x, y, sz) CARBON_RECT(x, y, sz, sz)

/**
 * @brief Defines an inline rectangle whose width and height are equal (square).
 * @param v The 2D vector to assign to the position.
 * @param sz The value to assign to both the width and the height of the size.
 */
#define CARBON_RECT_SQUARE_V(v, sz) CARBON_RECT_SQUARE(v.x, v.y, sz)

/**
 * @brief Defines an inline rectangle whose 4 elements are equal to zero.
 */
#define CARBON_RECT_ZERO CARBON_RECT(0, 0, 0, 0)

/**
 * @brief Represents the position and size of a rectangle with four 32-bit floating-point (f32) values.
 */
typedef struct CBN_Rect {
  f32 x, y;
  f32 w, h;
#ifdef __cplusplus
  /**
   * @brief carbon_math_rect_contains_point
   * @param p The point to check.
   * @return A boolean value representing whether the point is contained within this rectangle or not.
   */
  u8 Contains(const CBN_Vec2 &p) const;
  /**
   * @brief carbon_math_rect_detect_collision
   * @param r The other rectangle to check with.
   * @return A boolean value representing whether they overlap (also touching).
   */
  u8 Overlaps(const CBN_Rect &r) const;
  /**
   * @brief carbon_math_rect_scale
   * @param s The scale factor value.
   */
  void Scale(const f32 s);
  // Overloaded Operators
  bool operator==(const CBN_Rect &r) const;
#endif
} CBN_Rect;

/**
 * @brief Determines whether the specified point is contained within a rectangle.
 * @param r The rectangle.
 * @param p The point to check.
 * @return A boolean value representing whether the point is contained within this rectangle or not.
 */
CBNDEF u8 carbon_math_rect_contains_point(CBN_Rect r, CBN_Vec2 p);

/**
 * @brief Determines whether the rectangles collide/overlap/intersect with each other.
 * @param r1 The first rectangle.
 * @param r2 The second rectangle.
 * @return A boolean value representing whether they overlap (also touching).
 */
CBNDEF u8 carbon_math_rect_detect_collision(CBN_Rect r1, CBN_Rect r2);

/**
 * @brief Scales the rectangle by the specified value.
 * @param r The rectangle.
 * @param s The scale factor value.
 */
CBNDEF void carbon_math_rect_scale(CBN_Rect *r, const f32 s);

// Local Variables:
// mode: c++
// End:
