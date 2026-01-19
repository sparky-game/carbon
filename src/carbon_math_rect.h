/*
**  $$=========================$$
**  ||       Math (Rect)       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents the position and size of a rectangle with four 32-bit floating-point (f32) values.
 */
CBNDEF_PDS(CBN_Rect) {
  f32 x, y;
  f32 w, h;
};

// Method declarations
#ifdef __cplusplus
struct CBN_Rect : CBN_Rect_t {
  /**
   * @see carbon_math_rect
   */
  constexpr CBN_Rect(f32 _x, f32 _y, f32 _w, f32 _h) : CBN_Rect_t{_x, _y, _w, _h} {}
  constexpr CBN_Rect(void) : CBN_Rect(0, 0, 0, 0) {}
  /**
   * @see carbon_math_rect_v
   */
  constexpr CBN_Rect(CBN_Vec2 v, f32 _w, f32 _h) : CBN_Rect(v.x, v.y, _w, _h) {}
  /**
   * @see carbon_math_rect_sq
   */
  constexpr CBN_Rect(f32 _x, f32 _y, f32 sz) : CBN_Rect(_x, _y, sz, sz) {}
  /**
   * @see carbon_math_rect_sq_v(CBN_Vec2 v, f32 sz)
   */
  constexpr CBN_Rect(CBN_Vec2 v, f32 sz) : CBN_Rect(v.x, v.y, sz) {}
  /**
   * @see carbon_math_rect_contains
   */
  bool Contains(const CBN_Vec2 &p) const;
  /**
   * @see carbon_math_rect_overlaps
   */
  bool Overlaps(const CBN_Rect &r) const;
  /**
   * @see carbon_math_rect_intersection
   */
  CBN_Rect Intersection(const CBN_Rect &r) const;
  /**
   * @see carbon_math_rect_scale
   */
  void Scale(const f32 s);
  // Overloaded Operators
  bool operator==(const CBN_Rect &r) const;
};
#endif
CBNDEF_T(cbn::math, Rect, CBN_Rect);

/**
 * @brief Creates a rectangle.
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 * @return The newly created rectangle.
 */
CBNDEF CBN_Rect carbon_math_rect(f32 x, f32 y, f32 w, f32 h);

/**
 * @brief Creates a rectangle.
 * @param v The 2D vector to assign to the position.
 * @param w The value to assign to the width of the size.
 * @param h The value to assign to the height of the size.
 * @return The newly created rectangle.
 */
CBNDEF CBN_Rect carbon_math_rect_v(CBN_Vec2 v, f32 w, f32 h);

/**
 * @brief Creates a rectangle whose width and height are equal (square).
 * @param x The value to assign to the X coordinate of the position.
 * @param y The value to assign to the Y coordinate of the position.
 * @param sz The value to assign to both the width and the height of the size.
 * @return The newly created rectangle.
 */
CBNDEF CBN_Rect carbon_math_rect_sq(f32 x, f32 y, f32 sz);

/**
 * @brief Creates a rectangle whose width and height are equal (square).
 * @param v The 2D vector to assign to the position.
 * @param sz The value to assign to both the width and the height of the size.
 * @return The newly created rectangle.
 */
CBNDEF CBN_Rect carbon_math_rect_sq_v(CBN_Vec2 v, f32 sz);

/**
 * @brief Determines whether the specified point is contained within a rectangle.
 * @param r The rectangle.
 * @param p The point to check.
 * @return A boolean value representing whether the point is contained within this rectangle or not.
 */
CBNDEF bool carbon_math_rect_contains(CBN_Rect r, CBN_Vec2 p);

/**
 * @brief Determines whether the rectangles overlap/intersect/collide with each other.
 * @param r1 The first rectangle.
 * @param r2 The second rectangle.
 * @return A boolean value representing whether they overlap (also touching).
 */
CBNDEF bool carbon_math_rect_overlaps(CBN_Rect r1, CBN_Rect r2);

/**
 * @brief Computes the intersection rectangle between the rectangles.
 * @param r1 The first rectangle.
 * @param r2 The second rectangle.
 * @return The rectangle holding the area of the intersection between them.
 */
CBNDEF CBN_Rect carbon_math_rect_intersection(CBN_Rect r1, CBN_Rect r2);

/**
 * @brief Scales the rectangle by the specified value.
 * @param r The rectangle.
 * @param s The scale factor value.
 */
CBNDEF void carbon_math_rect_scale(CBN_Rect *r, const f32 s);

// Local Variables:
// mode: c++
// End:
