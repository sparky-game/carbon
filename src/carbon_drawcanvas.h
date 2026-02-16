/*
  $$========================$$
  ||       DrawCanvas       ||
  $$========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a 2D canvas to draw things to.
 */
typedef struct CBN_DrawCanvas CBN_DrawCanvas;
CBNDEF_AKA(cbn, DrawCanvas, CBN_DrawCanvas);

/**
 * @brief Set of optional flags to customize the renderer as you see fit.
 */
enum {
  CARBON_DRAWCANVAS_FLAG_MULTITHREAD      = 1 << 0,
  CARBON_DRAWCANVAS_FLAG_FRONTFACE_CW     = 1 << 1,
  CARBON_DRAWCANVAS_FLAG_BACKFACE_CULLING = 1 << 2
};

typedef struct {
  CBN_Vec3 position;
  CBN_Vec3 rotation;
  CBN_Vec3 scale;
} CBN_Transform;
CBNDEF_AKA(cbn, Transform, CBN_Transform);

/**
 * @brief Creates a new DrawCanvas object.
 * @param width The width to set the canvas to.
 * @param height The height to set the canvas to.
 * @return The newly created DrawCanvas object.
 */
CBNDEF CBN_DrawCanvas *carbon_drawcanvas_create(usz width, usz height);

/**
 * @brief Destroys a DrawCanvas object.
 * @param dc The DrawCanvas to destroy.
 */
CBNDEF void carbon_drawcanvas_destroy(CBN_DrawCanvas *dc);

/**
 */
CBNDEF u32 *carbon_drawcanvas_pixels(const CBN_DrawCanvas *dc);

/**
 */
CBNDEF usz carbon_drawcanvas_width(const CBN_DrawCanvas *dc);

/**
 */
CBNDEF usz carbon_drawcanvas_height(const CBN_DrawCanvas *dc);

/**
 * @brief Obtains the current set of flags enabled for the DrawCanvas.
 * @param dc The DrawCanvas object.
 * @return The current enabled flag(s).
 */
CBNDEF u32 carbon_drawcanvas_flags(const CBN_DrawCanvas *dc);

/**
 * @brief Enables a particular set of flags for the DrawCanvas.
 * @param dc The DrawCanvas object.
 * @param flags The flag(s) to enable.
 */
CBNDEF void carbon_drawcanvas_flags_enable(CBN_DrawCanvas *dc, u32 flags);

/**
 * @brief Disables a particular set of flags for the DrawCanvas.
 * @param dc The DrawCanvas object.
 * @param flags The flag(s) to disable.
 */
CBNDEF void carbon_drawcanvas_flags_disable(CBN_DrawCanvas *dc, u32 flags);

/**
 * @brief Toggles (ON/OFF) a particular set of flags for the DrawCanvas.
 * @param dc The DrawCanvas object.
 * @param flags The flag(s) to toggle.
 */
CBNDEF void carbon_drawcanvas_flags_toggle(CBN_DrawCanvas *dc, u32 flags);

/**
 * @brief Fills the DrawCanvas with the specified color.
 * @param dc The DrawCanvas object.
 * @param color The color to fill the canvas with.
 */
CBNDEF void carbon_drawcanvas_fill(CBN_DrawCanvas *dc, u32 color);

/**
 * @brief Draws a line to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param v1 The first point of the line.
 * @param v2 The second point of the line.
 * @param color The color to draw the line with.
 */
CBNDEF void carbon_drawcanvas_line(CBN_DrawCanvas *dc, CBN_Vec2 v1, CBN_Vec2 v2, u32 color);

/**
 * @brief Draws a triangle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param v1 The first vertex of the triangle.
 * @param v2 The second vertex of the triangle.
 * @param v3 The third vertex of the triangle.
 * @param color The color to draw the triangle with.
 */
CBNDEF void carbon_drawcanvas_triangle(CBN_DrawCanvas *dc, CBN_Vec2 v1, CBN_Vec2 v2, CBN_Vec2 v3, u32 color);

/**
 * @brief Draws a rectangle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param r The rectangle to draw.
 * @param color The color to draw the rectangle with.
 */
CBNDEF void carbon_drawcanvas_rect(CBN_DrawCanvas *dc, CBN_Rect r, u32 color);

/**
 * @brief Draws a circle to the canvas with a specific color.
 * @param dc The DrawCanvas object.
 * @param center The position of the center of the circle.
 * @param radius The radius of the circle.
 * @param color The color to draw the circle with.
 */
CBNDEF void carbon_drawcanvas_circle(CBN_DrawCanvas *dc, CBN_Vec2 center, usz radius, u32 color);

/**
 * @brief Draws a sprite to the canvas.
 * @param dc The DrawCanvas object.
 * @param sprite The sprite to draw.
 * @param position The position (top-left corner) to draw the sprite to.
 */
CBNDEF void carbon_drawcanvas_sprite(CBN_DrawCanvas *dc, const CBN_Sprite *s, CBN_Vec2 position, CBN_Vec2 scale);

/**
 * @brief Draws a 3D mesh to the canvas.
 * @param dc The DrawCanvas object.
 * @param c The 3D camera used to look into the world.
 * @param mesh The 3D mesh to draw.
 * @param t The 3D transform to draw the mesh with.
 * @param color The color to draw the mesh's faces with.
 */
CBNDEF void carbon_drawcanvas_mesh(CBN_DrawCanvas *dc, const CBN_Camera *c, const CBN_Mesh *m, CBN_Transform t, u32 color);

/**
 * @brief Draws a 3D XZ-axis-aligned plane to the canvas.
 * @param dc The DrawCanvas object.
 * @param c The 3D camera used to look into the world.
 * @param center The position of the center of the plane.
 * @param size The size of the plane (size.x -> X, size.y -> Z).
 * @param color The color to draw the plane with.
 */
CBNDEF void carbon_drawcanvas_plane_xz(CBN_DrawCanvas *dc, const CBN_Camera *c, CBN_Vec3 center, CBN_Vec2 size, u32 color);

/**
 * @brief Draws text to the canvas with a specific color (using a default font).
 * @param dc The DrawCanvas object.
 * @param txt The text to draw.
 * @param position The position (top-left corner) to draw the text to.
 * @param size The font size to use.
 * @param color The color to draw the text with.
 */
CBNDEF void carbon_drawcanvas_text(CBN_DrawCanvas *dc, const char *txt, CBN_Vec2 position, usz size, u32 color);

/**
 * @brief Draws text to the canvas with a specific color and a semi-transparent shadow (using a default font).
 * @param dc The DrawCanvas object.
 * @param txt The text to draw.
 * @param position The position (top-left corner) to draw the text to.
 * @param size The font size to use.
 * @param color The color to draw the text with.
 */
CBNDEF void carbon_drawcanvas_text_with_shadow(CBN_DrawCanvas *dc, const char *txt, CBN_Vec2 position, usz size, u32 color);

// CBNDEF void carbon_drawcanvas_text_with_font(CBN_DrawCanvas *dc, CBN_Font *f, const char *txt, CBN_Vec2 position, usz size, u32 color);

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
