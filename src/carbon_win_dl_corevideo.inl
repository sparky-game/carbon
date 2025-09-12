// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

static void *carbon_win__dl_CoreVideo;

// NOTE: we ignore here the `-Wdeprecated-declarations` warning, because no suitable
// non-deprecated replacements were found for [`CVDisplayLinkCreateWithCGDisplay`,
// `CVDisplayLinkGetNominalOutputVideoRefreshPeriod`]. These funcs were deprecated by
// Apple from macOS 15.0 onwards.
CARBON_COMPILER_DIAG_BEGIN;
CARBON_COMPILER_DIAG_IGNORE("-Wdeprecated-declarations");

CARBON_WIN__DLDECL(CVDisplayLinkCreateWithCGDisplay);
CVReturn CVDisplayLinkCreateWithCGDisplay(CGDirectDisplayID displayID, CVDisplayLinkRef *displayLinkOut) {
  CARBON_WIN__DLSYM(carbon_win__dl_CoreVideo, CVDisplayLinkCreateWithCGDisplay);
  return CVDisplayLinkCreateWithCGDisplay_ptr(displayID, displayLinkOut);
}

CARBON_WIN__DLDECL(CVDisplayLinkGetNominalOutputVideoRefreshPeriod);
CVTime CVDisplayLinkGetNominalOutputVideoRefreshPeriod(CVDisplayLinkRef displayLink) {
  CARBON_WIN__DLSYM(carbon_win__dl_CoreVideo, CVDisplayLinkGetNominalOutputVideoRefreshPeriod);
  return CVDisplayLinkGetNominalOutputVideoRefreshPeriod_ptr(displayLink);
}

CARBON_COMPILER_DIAG_END;

// Local Variables:
// mode: c
// End:
