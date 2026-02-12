// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

CBNINL void carbon_drawcanvas__st_fill(CBN_DrawCanvas *dc, u32 color) {
  for (usz i = 0; i < dc->width * dc->height; ++i) {
    dc->pixels[i] = color;
    dc->zbuffer[i] = 1;
  }
}

// Local Variables:
// mode: c
// End:
