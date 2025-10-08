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
#ifdef __cplusplus
  /**
   * @see carbon_drawcanvas_create
   */
  static CBN_DrawCanvas make(usz width, usz height);
  /**
   * @see carbon_drawcanvas_destroy
   */
  void Free(void);
  /**
   * @see carbon_drawcanvas_fill
   */
  void Fill(u32 color);
  /**
   * @see carbon_drawcanvas_line
   */
  void DrawLine(CBN_Vec2 v1, CBN_Vec2 v2, u32 color);
  /**
   * @see carbon_drawcanvas_triangle
   */
  void DrawTriangle(CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color);
  /**
   * @see carbon_drawcanvas_rect
   */
  void DrawRect(CBN_Rect r, u32 color);
  /**
   * @see carbon_drawcanvas_circle
   */
  void DrawCircle(CBN_Vec2 center, usz radius, u32 color);
  /**
   * @see carbon_drawcanvas_sprite
   */
  void DrawSprite(const CBN_Sprite *s, CBN_Vec2 position);
  /**
   * @see carbon_drawcanvas_box
   */
  void DrawBox(CBN_Rect r);
  /**
   * @see carbon_drawcanvas_text_with_shadow
   */
  void DrawText(const char *txt, CBN_Vec2 position, usz size, u32 color);
  /**
   * @see carbon_drawcanvas_text
   */
  void DrawTextRaw(const char *txt, CBN_Vec2 position, usz size, u32 color);
  /**
   * @see carbon_drawcanvas_get_text_width
   */
  static usz TextWidth(const char *txt, usz size);
  /**
   * @see carbon_drawcanvas_get_text_height
   */
  static usz TextHeight(usz size);
  /**
   * @see carbon_drawcanvas_hsv_to_rgb
   */
  static u32 HSVToRGB(f32 h, f32 s, f32 v);
  /**
   * @see carbon_drawcanvas_at
   */
  u32 &operator()(usz i, usz j) const;
  explicit operator bool(void) const;
#endif
} CBN_DrawCanvas;

#define carbon_drawcanvas_at(dc, i, j) (dc).pixels[(j) * (dc).width + (i)]

/**
 * @brief Creates a new DrawCanvas object.
 * @param width The width to set the canvas to.
 * @param height The height to set the canvas to.
 * @return The newly created DrawCanvas object.
 */
CBNDEF CBN_DrawCanvas carbon_drawcanvas_create(usz width, usz height);

/**
 * @brief Destroys a DrawCanvas object.
 * @param dc The DrawCanvas to destroy.
 */
CBNDEF void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc);

/**
 * @brief Fills the DrawCanvas with the specified color.
 * @param dc The DrawCanvas object.
 * @param color The color to fill the canvas with.
 */
CBNDEF void carbon_drawcanvas_fill(CBN_DrawCanvas dc, u32 color);

/**
 * @brief Draws a line to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param v1 The first point of the line.
 * @param v2 The second point of the line.
 * @param color The color to draw the line with.
 */
CBNDEF void carbon_drawcanvas_line(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, u32 color);

/**
 * @brief Draws a triangle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param v1 The first vertex of the triangle.
 * @param v2 The second vertex of the triangle.
 * @param v3 The third vertex of the triangle.
 * @param color The color to draw the triangle with.
 */
CBNDEF void carbon_drawcanvas_triangle(CBN_DrawCanvas dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color);

/**
 * @brief Draws a rectangle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param r The rectangle to draw.
 * @param color The color to draw the rectangle with.
 */
CBNDEF void carbon_drawcanvas_rect(CBN_DrawCanvas dc, CBN_Rect r, u32 color);

/**
 * @brief Draws a circle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param center The position of the center of the circle.
 * @param radius The radius of the circle.
 * @param color The color to draw the circle with.
 */
CBNDEF void carbon_drawcanvas_circle(CBN_DrawCanvas dc, CBN_Vec2 center, usz radius, u32 color);

/**
 * @brief Draws a sprite to the canvas.
 * @param dc The DrawCanvas object.
 * @param sprite The sprite to draw.
 * @param position The position (top-left corner) to draw the sprite to.
 */
CBNDEF void carbon_drawcanvas_sprite(CBN_DrawCanvas dc, const CBN_Sprite *s, CBN_Vec2 position);

/**
 * @brief Draws a box to the canvas.
 * @param dc The DrawCanvas object.
 * @param r The spec of the desired box as a rectangle.
 */
CBNDEF void carbon_drawcanvas_box(CBN_DrawCanvas dc, CBN_Rect r);

/**
 * @brief Draws text to the canvas with a specific color (using a default font).
 * @param dc The DrawCanvas object.
 * @param txt The text to draw.
 * @param position The position (top-left corner) to draw the text to.
 * @param size The font size to use.
 * @param color The color to draw the text with.
 */
CBNDEF void carbon_drawcanvas_text(CBN_DrawCanvas dc, const char *txt, CBN_Vec2 position, usz size, u32 color);

/**
 * @brief Draws text to the canvas with a specific color and a semi-transparent shadow (using a default font).
 * @param dc The DrawCanvas object.
 * @param txt The text to draw.
 * @param position The position (top-left corner) to draw the text to.
 * @param size The font size to use.
 * @param color The color to draw the text with.
 */
CBNDEF void carbon_drawcanvas_text_with_shadow(CBN_DrawCanvas dc, const char *txt, CBN_Vec2 position, usz size, u32 color);

/**
 * @brief Measures text width (using a default font).
 * @param txt The text to measure.
 * @param size The font size to use.
 * @return The width of the given text using the specified font size.
 */
CBNDEF usz carbon_drawcanvas_get_text_width(const char *txt, usz size);

/**
 * @brief Measures text height (using a default font).
 * @param size The font size to use.
 * @return The height of the text using the specified font size.
 */
CBNDEF usz carbon_drawcanvas_get_text_height(usz size);

/**
 * @brief Transforms HSV color into RGB (A always set to 0xff).
 * @param h Hue, in [0..360] range.
 * @param s Saturation, in [0..1] range.
 * @param v Value, in [0..1] range.
 * @return 32-bit RGBA color value (A always set to 0xff).
 */
CBNDEF u32 carbon_drawcanvas_hsv_to_rgb(f32 h, f32 s, f32 v);

// Local Variables:
// mode: c++
// End:
