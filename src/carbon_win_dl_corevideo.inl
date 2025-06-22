// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

static void *carbon_win__dl_CoreVideo;

// NOTE: we ignore here the `-Wdeprecated-declarations` warning, because no suitable
// non-deprecated replacements were found for [`CVDisplayLinkCreateWithCGDisplay`,
// `CVDisplayLinkGetNominalOutputVideoRefreshPeriod`]. These funcs were deprecated by
// Apple from macOS 15.0 onwards.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

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

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// Local Variables:
// mode: c
// End:
