/*
**  $$========================$$
**  ||       DrawCanvas       ||
**  $$========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Represents a 2D canvas to draw things to.
 */
typedef struct CBN_DrawCanvas {
  u32 *pixels;
  usz width;
  usz height;
  usz stride;
#ifdef __cplusplus
  /**
   * @brief carbon_drawcanvas_create
   * @param width The width to set the canvas to.
   * @param height The height to set the canvas to.
   * @return The newly created DrawCanvas object.
   */
  static CBN_DrawCanvas make(usz width, usz height);
  /**
   * @brief carbon_drawcanvas_destroy
   */
  void Free(void);
  /**
   * @brief carbon_drawcanvas_fill
   * @param color The color to fill the canvas with.
   */
  void Fill(u32 color);
  /**
   * @brief carbon_drawcanvas_triangle
   * @param v1 The first vertex of the triangle.
   * @param v2 The second vertex of the triangle.
   * @param v3 The third vertex of the triangle.
   * @param color The color to draw the triangle with.
   */
  void DrawTriangle(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color);
  /**
   * @brief carbon_drawcanvas_rect
   * @param r The rectangle to draw.
   * @param color The color to draw the rectangle with.
   */
  void DrawRect(CBN_Rect r, u32 color);
  /**
   * @brief carbon_drawcanvas_circle
   * @param center The position of the center of the circle.
   * @param radius The radius of the circle.
   * @param color The color to draw the circle with.
   */
  void DrawCircle(CBN_Vec2 center, usz radius, u32 color);
  /**
   * @brief carbon_drawcanvas_hsv_to_rgb
   * @param h Hue, in [0..360] range.
   * @param s Saturation, in [0..1] range.
   * @param v Value, in [0..1] range.
   * @return 32-bit RGBA color value (A always set to 0xff).
   */
  static u32 HSVToRGB(f32 h, f32 s, f32 v);
  // Overloaded Operators
  u32 &operator()(usz i, usz j);
  const u32 &operator()(usz i, usz j) const;
#endif
} CBN_DrawCanvas;

#define CARBON_DRAWCANVAS_AT(dc, i, j) (dc).pixels[(j) * (dc).stride + (i)]

/**
 * @brief Creates a new DrawCanvas object.
 * @param width The width to set the canvas to.
 * @param height The height to set the canvas to.
 * @return The newly created DrawCanvas object.
 */
CARBON_API CBN_DrawCanvas carbon_drawcanvas_create(usz width, usz height);

/**
 * @brief Destroys a DrawCanvas object.
 * @param dc The DrawCanvas to destroy.
 */
CARBON_API void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc);

/**
 * @brief Fills the DrawCanvas with the specified color.
 * @param dc The DrawCanvas object.
 * @param color The color to fill the canvas with.
 */
CARBON_API void carbon_drawcanvas_fill(CBN_DrawCanvas dc, u32 color);

/**
 * @brief Draws a triangle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param v1 The first vertex of the triangle.
 * @param v2 The second vertex of the triangle.
 * @param v3 The third vertex of the triangle.
 * @param color The color to draw the triangle with.
 */
CARBON_API void carbon_drawcanvas_triangle(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color);

/**
 * @brief Draws a rectangle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param r The rectangle to draw.
 * @param color The color to draw the rectangle with.
 */
CARBON_API void carbon_drawcanvas_rect(CBN_DrawCanvas dc, CBN_Rect r, u32 color);

/**
 * @brief Draws a circle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param center The position of the center of the circle.
 * @param radius The radius of the circle.
 * @param color The color to draw the circle with.
 */
CARBON_API void carbon_drawcanvas_circle(CBN_DrawCanvas dc, CBN_Vec2 center, usz radius, u32 color);

/**
 * @brief Transforms HSV color into RGB (A always set to 0xff).
 * @param h Hue, in [0..360] range.
 * @param s Saturation, in [0..1] range.
 * @param v Value, in [0..1] range.
 * @return 32-bit RGBA color value (A always set to 0xff).
 */
CARBON_API u32 carbon_drawcanvas_hsv_to_rgb(f32 h, f32 s, f32 v);

// Local Variables:
// mode: c++
// End:
