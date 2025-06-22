// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

static void *carbon_win__dl_Cocoa;

CARBON_WIN__DLDECL(CGAssociateMouseAndMouseCursorPosition);
CGError CGAssociateMouseAndMouseCursorPosition(boolean_t connected) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGAssociateMouseAndMouseCursorPosition);
  return CGAssociateMouseAndMouseCursorPosition_ptr(connected);
}

CARBON_WIN__DLDECL(CGBitmapContextCreate);
CGContextRef CGBitmapContextCreate(void *data,
                                   size_t width,
                                   size_t height,
                                   size_t bitsPerComponent,
                                   size_t bytesPerRow,
                                   CGColorSpaceRef space,
                                   CGBitmapInfo bitmapInfo) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGBitmapContextCreate);
  return CGBitmapContextCreate_ptr(data, width, height, bitsPerComponent, bytesPerRow, space, bitmapInfo);
}

CARBON_WIN__DLDECL(CGBitmapContextCreateImage);
CGImageRef CGBitmapContextCreateImage(CGContextRef context) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGBitmapContextCreateImage);
  return CGBitmapContextCreateImage_ptr(context);
}

CARBON_WIN__DLDECL(CGColorSpaceCreateDeviceRGB);
CGColorSpaceRef CGColorSpaceCreateDeviceRGB(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGColorSpaceCreateDeviceRGB);
  return CGColorSpaceCreateDeviceRGB_ptr();
}

CARBON_WIN__DLDECL(CGColorSpaceRelease);
void CGColorSpaceRelease(CGColorSpaceRef space) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGColorSpaceRelease);
  return CGColorSpaceRelease_ptr(space);
}

CARBON_WIN__DLDECL(CGContextDrawImage);
void CGContextDrawImage(CGContextRef c, CGRect rect, CGImageRef image) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGContextDrawImage);
  return CGContextDrawImage_ptr(c, rect, image);
}

CARBON_WIN__DLDECL(CGContextRelease);
void CGContextRelease(CGContextRef c) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGContextRelease);
  return CGContextRelease_ptr(c);
}

CARBON_WIN__DLDECL(CGDisplayBounds);
CGRect CGDisplayBounds(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayBounds);
  return CGDisplayBounds_ptr(display);
}

CARBON_WIN__DLDECL(CGDisplayCopyAllDisplayModes);
CFArrayRef CGDisplayCopyAllDisplayModes(CGDirectDisplayID display, CFDictionaryRef options) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayCopyAllDisplayModes);
  return CGDisplayCopyAllDisplayModes_ptr(display, options);
}

CARBON_WIN__DLDECL(CGDisplayCopyDisplayMode);
CGDisplayModeRef CGDisplayCopyDisplayMode(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayCopyDisplayMode);
  return CGDisplayCopyDisplayMode_ptr(display);
}

CARBON_WIN__DLDECL(CGDisplayHideCursor);
CGError CGDisplayHideCursor(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayHideCursor);
  return CGDisplayHideCursor_ptr(display);
}

CARBON_WIN__DLDECL(CGDisplayModeGetHeight);
size_t CGDisplayModeGetHeight(CGDisplayModeRef mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayModeGetHeight);
  return CGDisplayModeGetHeight_ptr(mode);
}

CARBON_WIN__DLDECL(CGDisplayModeGetRefreshRate);
double CGDisplayModeGetRefreshRate(CGDisplayModeRef mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayModeGetRefreshRate);
  return CGDisplayModeGetRefreshRate_ptr(mode);
}

CARBON_WIN__DLDECL(CGDisplayModeGetWidth);
size_t CGDisplayModeGetWidth(CGDisplayModeRef mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayModeGetWidth);
  return CGDisplayModeGetWidth_ptr(mode);
}

CARBON_WIN__DLDECL(CGDisplayPixelsHigh);
size_t CGDisplayPixelsHigh(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayPixelsHigh);
  return CGDisplayPixelsHigh_ptr(display);
}

CARBON_WIN__DLDECL(CGDisplayPixelsWide);
size_t CGDisplayPixelsWide(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayPixelsWide);
  return CGDisplayPixelsWide_ptr(display);
}

CARBON_WIN__DLDECL(CGDisplayScreenSize);
CGSize CGDisplayScreenSize(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayScreenSize);
  return CGDisplayScreenSize_ptr(display);
}

CARBON_WIN__DLDECL(CGDisplaySetDisplayMode);
CGError CGDisplaySetDisplayMode(CGDirectDisplayID display, CGDisplayModeRef mode, CFDictionaryRef options) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplaySetDisplayMode);
  return CGDisplaySetDisplayMode_ptr(display, mode, options);
}

CARBON_WIN__DLDECL(CGDisplayShowCursor);
CGError CGDisplayShowCursor(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayShowCursor);
  return CGDisplayShowCursor_ptr(display);
}

CARBON_WIN__DLDECL(CGEventCreate);
CGEventRef CGEventCreate(CGEventSourceRef source) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGEventCreate);
  return CGEventCreate_ptr(source);
}

CARBON_WIN__DLDECL(CGEventGetLocation);
CGPoint CGEventGetLocation(CGEventRef event) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGEventGetLocation);
  return CGEventGetLocation_ptr(event);
}

CARBON_WIN__DLDECL(CGGetActiveDisplayList);
CGError CGGetActiveDisplayList(uint32_t maxDisplays, CGDirectDisplayID *activeDisplays, uint32_t *displayCount) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGGetActiveDisplayList);
  return CGGetActiveDisplayList_ptr(maxDisplays, activeDisplays, displayCount);
}

CARBON_WIN__DLDECL(CGGetDisplaysWithPoint);
CGError CGGetDisplaysWithPoint(CGPoint point, uint32_t maxDisplays, CGDirectDisplayID *displays, uint32_t *matchingDisplayCount) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGGetDisplaysWithPoint);
  return CGGetDisplaysWithPoint_ptr(point, maxDisplays, displays, matchingDisplayCount);
}

CARBON_WIN__DLDECL(CGImageRelease);
void CGImageRelease(CGImageRef image) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGImageRelease);
  return CGImageRelease_ptr(image);
}

CARBON_WIN__DLDECL(CGMainDisplayID);
CGDirectDisplayID CGMainDisplayID(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGMainDisplayID);
  return CGMainDisplayID_ptr();
}

CARBON_WIN__DLDECL(CGWarpMouseCursorPosition);
CGError CGWarpMouseCursorPosition(CGPoint newCursorPosition) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGWarpMouseCursorPosition);
  return CGWarpMouseCursorPosition_ptr(newCursorPosition);
}

// Local Variables:
// mode: c
// End:
