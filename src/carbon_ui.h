/*
  $$================$$
  ||       UI       ||
  $$================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief ...
 */
#define CARBON_UI_ANCHORS(x, p)                         \
  x(p, TopLeft)    x(p, TopCenter)    x(p, TopRight)    \
  x(p, LeftCenter) x(p, Center)       x(p, RightCenter) \
  x(p, BottomLeft) x(p, BottomCenter) x(p, BottomRight)
CBNDEF_ENUM(CBN_UI_Anchor, CARBON_UI_ANCHORS);
CBNDEF_AKA(cbn::ui, Anchor, CBN_UI_Anchor);

/**
 * @brief Computes the anchored top-left position of an element within a container.
 * @param cntr The bounding rectangle of the container.
 * @param size The size of the element being placed.
 * @param anchor The anchor point to use against the container.
 * @param pad The padding applied inward from each edge of the container.
 * @return The screen-space position of the element inside the container.
 */
CBNDEF CBN_Vec2 carbon_ui_anchor_position(CBN_Rect cntr, CBN_Vec2 size, CBN_UI_Anchor anchor, CBN_Vec2 pad);
CBNDEF_FN(cbn::ui, AnchorPosition, carbon_ui_anchor_position);

/**
 * @brief Computes the top-left positions for a horizontal stack layout of elements within a container.
 * @param cntr The bounding rectangle of the container.
 * @param sizes Array of sizes of the elements being placed.
 * @param sizes_count Number of elements.
 * @param gap Spacing between consecutive elements alongside the horizontal (X) axis.
 * @param anchor The anchor point to use (for the whole stack block) against the container.
 * @param cross_anchor The anchor point to use (element-wise) alongside the vertical (Y) axis.
 * @param pad The padding applied inward from each edge of the container.
 * @param out_pos Output array of screen-space positions of the elements inside the container.
 */
CBNDEF void carbon_ui_hstack(CBN_Rect cntr, const CBN_Vec2 *sizes, usz sizes_count, f32 gap, CBN_UI_Anchor anchor, CBN_UI_Anchor cross_anchor, CBN_Vec2 pad, CBN_Vec2 *out_pos);
//CBNDEF_FN(cbn::ui, HStack, carbon_ui_hstack);

/**
 * @brief Computes the top-left positions for a vertical stack layout of elements within a container.
 * @param cntr The bounding rectangle of the container.
 * @param sizes Array of sizes of the elements being placed.
 * @param sizes_count Number of elements.
 * @param gap Spacing between consecutive elements alongside the vertical (Y) axis.
 * @param anchor The anchor point to use (for the whole stack block) against the container.
 * @param cross_anchor The anchor point to use (element-wise) alongside the horizontal (X) axis.
 * @param pad The padding applied inward from each edge of the container.
 * @param out_pos Output array of screen-space positions of the elements inside the container.
 */
CBNDEF void carbon_ui_vstack(CBN_Rect cntr, const CBN_Vec2 *sizes, usz sizes_count, f32 gap, CBN_UI_Anchor anchor, CBN_UI_Anchor cross_anchor, CBN_Vec2 pad, CBN_Vec2 *out_pos);
//CBNDEF_FN(cbn::ui, VStack, carbon_ui_vstack);
