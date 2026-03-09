// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

static const CBN_Vec2 carbon_ui__anchor_factors[] = {
  [CBN_UI_Anchor_TopLeft]      = {{{  0,   0}}},
  [CBN_UI_Anchor_TopCenter]    = {{{0.5,   0}}},
  [CBN_UI_Anchor_TopRight]     = {{{  1,   0}}},
  [CBN_UI_Anchor_LeftCenter]   = {{{  0, 0.5}}},
  [CBN_UI_Anchor_Center]       = {{{0.5, 0.5}}},
  [CBN_UI_Anchor_RightCenter]  = {{{  1, 0.5}}},
  [CBN_UI_Anchor_BottomLeft]   = {{{  0,   1}}},
  [CBN_UI_Anchor_BottomCenter] = {{{0.5,   1}}},
  [CBN_UI_Anchor_BottomRight]  = {{{  1,   1}}}
};
static_assert(CARBON_ARRAY_LEN(carbon_ui__anchor_factors) == CBN_UI_Anchor_Count,
              "Amount of UI Anchor points has changed");

CBN_Vec2 carbon_ui_anchor_position(CBN_Rect cntr, CBN_Vec2 size, CBN_UI_Anchor anchor, CBN_Vec2 pad) {
  CBN_Vec2 af = carbon_ui__anchor_factors[anchor];
  CBN_Vec2 s = carbon_math_vec2(cntr.w - size.x, cntr.h - size.y);
  return carbon_math_vec2_clamp(carbon_math_vec2(cntr.x + pad.x + af.x*carbon_math_max(0, s.x - 2*pad.x),
                                                 cntr.y + pad.y + af.y*carbon_math_max(0, s.y - 2*pad.y)),
                                carbon_math_vec2_add(cntr.xy, pad),
                                carbon_math_vec2_sub(carbon_math_vec2_add(cntr.xy, s), pad));
}

void carbon_ui_hstack(CBN_Rect cntr, const CBN_Vec2 *sizes, usz sizes_count, f32 gap, CBN_UI_Anchor anchor, CBN_UI_Anchor cross_anchor, CBN_Vec2 pad, CBN_Vec2 *out_pos) {
  f32 total_w = gap * (sizes_count - 1);
  f32 total_h = 0;
  for (usz i = 0; i < sizes_count; ++i) {
    total_w += sizes[i].x;
    total_h = carbon_math_max(total_h, sizes[i].y);
  }
  CBN_Vec2 origin = carbon_ui_anchor_position(cntr, carbon_math_vec2(total_w, total_h), anchor, pad);
  f32 af = carbon_ui__anchor_factors[cross_anchor].y;
  f32 cursor = origin.x;
  for (usz i = 0; i < sizes_count; ++i) {
    out_pos[i] = carbon_math_vec2(cursor, origin.y + af*(total_h - sizes[i].y));
    cursor += sizes[i].x + gap;
  }
}

void carbon_ui_vstack(CBN_Rect cntr, const CBN_Vec2 *sizes, usz sizes_count, f32 gap, CBN_UI_Anchor anchor, CBN_UI_Anchor cross_anchor, CBN_Vec2 pad, CBN_Vec2 *out_pos) {
  f32 total_w = 0;
  f32 total_h = gap * (sizes_count - 1);
  for (usz i = 0; i < sizes_count; ++i) {
    total_w = carbon_math_max(total_w, sizes[i].x);
    total_h += sizes[i].y;
  }
  CBN_Vec2 origin = carbon_ui_anchor_position(cntr, carbon_math_vec2(total_w, total_h), anchor, pad);
  f32 af = carbon_ui__anchor_factors[cross_anchor].x;
  f32 cursor = origin.y;
  for (usz i = 0; i < sizes_count; ++i) {
    out_pos[i] = carbon_math_vec2(origin.x + af*(total_w - sizes[i].x), cursor);
    cursor += sizes[i].y + gap;
  }
}
