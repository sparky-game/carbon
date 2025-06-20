// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_WIN__DLDECL(name)                \
  typedef CARBON_TYPE_OF(&name) name ## _ptr_t; \
  static name ## _ptr_t name ## _ptr;

#define CARBON_WIN__DLDECL_VAR(name)            \
  typedef CARBON_TYPE_OF(name) name ## _t;      \
  name ## _t *name ## _ptr;

#define CARBON_WIN__DLOPEN(lib, name)           \
  lib = dlopen(name, RTLD_LAZY | RTLD_LOCAL);   \
  CARBON_ASSERT(lib && "Failed to load");

#define CARBON_WIN__DLSYM(lib, name)                    \
  if (!name ## _ptr) {                                  \
    name ## _ptr = (name ## _ptr_t) dlsym(lib, #name);  \
    CARBON_ASSERT(name ## _ptr && "Failed to load");    \
  }

#define CARBON_WIN__DLSYM_VAR(lib, name)                \
  name ## _ptr = (name ## _t *) dlsym(lib, #name);      \
  CARBON_ASSERT(name ## _ptr && "Failed to load");

#ifdef __APPLE__
#define CARBON_WIN__DL_BASE_PATH "/System/Library/Frameworks/"
#define kCFAllocatorDefault             (*kCFAllocatorDefault_ptr)
#define kCFRunLoopDefaultMode           (*kCFRunLoopDefaultMode_ptr)
#define kCFTypeDictionaryKeyCallBacks   (*kCFTypeDictionaryKeyCallBacks_ptr)
#define kCFTypeDictionaryValueCallBacks (*kCFTypeDictionaryValueCallBacks_ptr)
#endif

#define RGFW_ALLOC   CARBON_MALLOC
#define RGFW_FREE    CARBON_FREE
#define RGFW_ASSERT  CARBON_ASSERT
#define RGFW_MEMCPY  carbon_memory_copy
#define RGFW_STRNCMP carbon_string_cmp_n
#define RGFW_STRTOL  strtol
#define RGFW_NO_TYPES
// TODO: Support other rendering methods (e.g. Vulkan, Metal, DirectX, OpenGL).
#define RGFW_BUFFER  // i.e. CPU software rendering
#define RGFW_IMPLEMENTATION
#include "../vendor/RGFW/RGFW.h"

static RGFW_window *carbon_win__handle;
static u32 carbon_win__max_fps;
static CBN_Image carbon_win__icon;

#if defined(__APPLE__)
// Cocoa
static void *carbon_win__dl_Cocoa;

// Cocoa :: kCFAllocatorDefault
CARBON_WIN__DLDECL_VAR(kCFAllocatorDefault);

// Cocoa :: kCFRunLoopDefaultMode
CARBON_WIN__DLDECL_VAR(kCFRunLoopDefaultMode);

// Cocoa :: kCFTypeDictionaryKeyCallBacks
CARBON_WIN__DLDECL_VAR(kCFTypeDictionaryKeyCallBacks);

// Cocoa :: kCFTypeDictionaryValueCallBacks
CARBON_WIN__DLDECL_VAR(kCFTypeDictionaryValueCallBacks);

// Cocoa :: CFArrayGetCount
CARBON_WIN__DLDECL(CFArrayGetCount);
CFIndex CFArrayGetCount(CFArrayRef theArray) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFArrayGetCount);
  return CFArrayGetCount_ptr(theArray);
}

// Cocoa :: CFArrayGetValueAtIndex
CARBON_WIN__DLDECL(CFArrayGetValueAtIndex);
const void *CFArrayGetValueAtIndex(CFArrayRef theArray, CFIndex idx) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFArrayGetValueAtIndex);
  return CFArrayGetValueAtIndex_ptr(theArray, idx);
}

// Cocoa :: CFBundleCopyResourcesDirectoryURL
CARBON_WIN__DLDECL(CFBundleCopyResourcesDirectoryURL);
CFURLRef CFBundleCopyResourcesDirectoryURL(CFBundleRef bundle) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFBundleCopyResourcesDirectoryURL);
  return CFBundleCopyResourcesDirectoryURL_ptr(bundle);
}

// Cocoa :: CFBundleGetMainBundle
CARBON_WIN__DLDECL(CFBundleGetMainBundle);
CFBundleRef CFBundleGetMainBundle(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFBundleGetMainBundle);
  return CFBundleGetMainBundle_ptr();
}

// Cocoa :: CFDictionaryCreateMutable
CARBON_WIN__DLDECL(CFDictionaryCreateMutable);
CFMutableDictionaryRef CFDictionaryCreateMutable(CFAllocatorRef allocator,
                                                 CFIndex capacity,
                                                 const CFDictionaryKeyCallBacks *keyCallBacks,
                                                 const CFDictionaryValueCallBacks *valueCallBacks) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFDictionaryCreateMutable);
  return CFDictionaryCreateMutable_ptr(allocator, capacity, keyCallBacks, valueCallBacks);
}

// Cocoa :: CFDictionarySetValue
CARBON_WIN__DLDECL(CFDictionarySetValue);
void CFDictionarySetValue(CFMutableDictionaryRef theDict, const void *key, const void *value) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFDictionarySetValue);
  return CFDictionarySetValue_ptr(theDict, key, value);
}

// Cocoa :: CFNumberCreate
CARBON_WIN__DLDECL(CFNumberCreate);
CFNumberRef CFNumberCreate(CFAllocatorRef allocator, CFNumberType theType, const void *valuePtr) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFNumberCreate);
  return CFNumberCreate_ptr(allocator, theType, valuePtr);
}

// Cocoa :: CFNumberGetValue
CARBON_WIN__DLDECL(CFNumberGetValue);
Boolean CFNumberGetValue(CFNumberRef number, CFNumberType theType, void *valuePtr) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFNumberGetValue);
  return CFNumberGetValue_ptr(number, theType, valuePtr);
}

// Cocoa :: CFRelease
CARBON_WIN__DLDECL(CFRelease);
void CFRelease(CFTypeRef cf) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFRelease);
  return CFRelease_ptr(cf);
}

// Cocoa :: CFRunLoopGetCurrent
CARBON_WIN__DLDECL(CFRunLoopGetCurrent);
CFRunLoopRef CFRunLoopGetCurrent(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFRunLoopGetCurrent);
  return CFRunLoopGetCurrent_ptr();
}

// Cocoa :: CFRunLoopRunInMode
CARBON_WIN__DLDECL(CFRunLoopRunInMode);
CFRunLoopRunResult CFRunLoopRunInMode(CFRunLoopMode mode, CFTimeInterval seconds, Boolean returnAfterSourceHandled) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFRunLoopRunInMode);
  return CFRunLoopRunInMode_ptr(mode, seconds, returnAfterSourceHandled);
}

// Cocoa :: CFStringCompare
CARBON_WIN__DLDECL(CFStringCompare);
CFComparisonResult CFStringCompare(CFStringRef theString1, CFStringRef theString2, CFStringCompareFlags compareOptions) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFStringCompare);
  return CFStringCompare_ptr(theString1, theString2, compareOptions);
}

// Cocoa :: CFStringGetCString
CARBON_WIN__DLDECL(CFStringGetCString);
Boolean CFStringGetCString(CFStringRef theString, char *buffer, CFIndex bufferSize, CFStringEncoding encoding) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFStringGetCString);
  return CFStringGetCString_ptr(theString, buffer, bufferSize, encoding);
}

// Cocoa :: CFURLCopyLastPathComponent
CARBON_WIN__DLDECL(CFURLCopyLastPathComponent);
CFStringRef CFURLCopyLastPathComponent(CFURLRef url) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFURLCopyLastPathComponent);
  return CFURLCopyLastPathComponent_ptr(url);
}

// Cocoa :: CFURLGetFileSystemRepresentation
CARBON_WIN__DLDECL(CFURLGetFileSystemRepresentation);
Boolean CFURLGetFileSystemRepresentation(CFURLRef url, Boolean resolveAgainstBase, UInt8 *buffer, CFIndex maxBufLen) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CFURLGetFileSystemRepresentation);
  return CFURLGetFileSystemRepresentation_ptr(url, resolveAgainstBase, buffer, maxBufLen);
}

// Cocoa :: CGAssociateMouseAndMouseCursorPosition
CARBON_WIN__DLDECL(CGAssociateMouseAndMouseCursorPosition);
CGError CGAssociateMouseAndMouseCursorPosition(boolean_t connected) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGAssociateMouseAndMouseCursorPosition);
  return CGAssociateMouseAndMouseCursorPosition_ptr(connected);
}

// Cocoa :: CGBitmapContextCreate
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

// Cocoa :: CGBitmapContextCreateImage
CARBON_WIN__DLDECL(CGBitmapContextCreateImage);
CGImageRef CGBitmapContextCreateImage(CGContextRef context) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGBitmapContextCreateImage);
  return CGBitmapContextCreateImage_ptr(context);
}

// Cocoa :: CGColorSpaceCreateDeviceRGB
CARBON_WIN__DLDECL(CGColorSpaceCreateDeviceRGB);
CGColorSpaceRef CGColorSpaceCreateDeviceRGB(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGColorSpaceCreateDeviceRGB);
  return CGColorSpaceCreateDeviceRGB_ptr();
}

// Cocoa :: CGColorSpaceRelease
CARBON_WIN__DLDECL(CGColorSpaceRelease);
void CGColorSpaceRelease(CGColorSpaceRef space) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGColorSpaceRelease);
  return CGColorSpaceRelease_ptr(space);
}

// Cocoa :: CGContextDrawImage
CARBON_WIN__DLDECL(CGContextDrawImage);
void CGContextDrawImage(CGContextRef c, CGRect rect, CGImageRef image) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGContextDrawImage);
  return CGContextDrawImage_ptr(c, rect, image);
}

// Cocoa :: CGContextRelease
CARBON_WIN__DLDECL(CGContextRelease);
void CGContextRelease(CGContextRef c) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGContextRelease);
  return CGContextRelease_ptr(c);
}

// Cocoa :: CGDisplayBounds
CARBON_WIN__DLDECL(CGDisplayBounds);
CGRect CGDisplayBounds(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayBounds);
  return CGDisplayBounds_ptr(display);
}

// Cocoa :: CGDisplayCopyAllDisplayModes
CARBON_WIN__DLDECL(CGDisplayCopyAllDisplayModes);
CFArrayRef CGDisplayCopyAllDisplayModes(CGDirectDisplayID display, CFDictionaryRef options) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayCopyAllDisplayModes);
  return CGDisplayCopyAllDisplayModes_ptr(display, options);
}

// Cocoa :: CGDisplayCopyDisplayMode
CARBON_WIN__DLDECL(CGDisplayCopyDisplayMode);
CGDisplayModeRef CGDisplayCopyDisplayMode(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayCopyDisplayMode);
  return CGDisplayCopyDisplayMode_ptr(display);
}

// Cocoa :: CGDisplayHideCursor
CARBON_WIN__DLDECL(CGDisplayHideCursor);
CGError CGDisplayHideCursor(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayHideCursor);
  return CGDisplayHideCursor_ptr(display);
}

// Cocoa :: CGDisplayModeGetHeight
CARBON_WIN__DLDECL(CGDisplayModeGetHeight);
size_t CGDisplayModeGetHeight(CGDisplayModeRef mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayModeGetHeight);
  return CGDisplayModeGetHeight_ptr(mode);
}

// Cocoa :: CGDisplayModeGetRefreshRate
CARBON_WIN__DLDECL(CGDisplayModeGetRefreshRate);
double CGDisplayModeGetRefreshRate(CGDisplayModeRef mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayModeGetRefreshRate);
  return CGDisplayModeGetRefreshRate_ptr(mode);
}

// Cocoa :: CGDisplayModeGetWidth
CARBON_WIN__DLDECL(CGDisplayModeGetWidth);
size_t CGDisplayModeGetWidth(CGDisplayModeRef mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayModeGetWidth);
  return CGDisplayModeGetWidth_ptr(mode);
}

// Cocoa :: CGDisplayPixelsHigh
CARBON_WIN__DLDECL(CGDisplayPixelsHigh);
size_t CGDisplayPixelsHigh(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayPixelsHigh);
  return CGDisplayPixelsHigh_ptr(display);
}

// Cocoa :: CGDisplayPixelsWide
CARBON_WIN__DLDECL(CGDisplayPixelsWide);
size_t CGDisplayPixelsWide(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayPixelsWide);
  return CGDisplayPixelsWide_ptr(display);
}

// Cocoa :: CGDisplayScreenSize
CARBON_WIN__DLDECL(CGDisplayScreenSize);
CGSize CGDisplayScreenSize(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayScreenSize);
  return CGDisplayScreenSize_ptr(display);
}

// Cocoa :: CGDisplaySetDisplayMode
CARBON_WIN__DLDECL(CGDisplaySetDisplayMode);
CGError CGDisplaySetDisplayMode(CGDirectDisplayID display, CGDisplayModeRef mode, CFDictionaryRef options) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplaySetDisplayMode);
  return CGDisplaySetDisplayMode_ptr(display, mode, options);
}

// Cocoa :: CGDisplayShowCursor
CARBON_WIN__DLDECL(CGDisplayShowCursor);
CGError CGDisplayShowCursor(CGDirectDisplayID display) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGDisplayShowCursor);
  return CGDisplayShowCursor_ptr(display);
}

// Cocoa :: CGEventCreate
CARBON_WIN__DLDECL(CGEventCreate);
CGEventRef CGEventCreate(CGEventSourceRef source) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGEventCreate);
  return CGEventCreate_ptr(source);
}

// Cocoa :: CGEventGetLocation
CARBON_WIN__DLDECL(CGEventGetLocation);
CGPoint CGEventGetLocation(CGEventRef event) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGEventGetLocation);
  return CGEventGetLocation_ptr(event);
}

// Cocoa :: CGGetActiveDisplayList
CARBON_WIN__DLDECL(CGGetActiveDisplayList);
CGError CGGetActiveDisplayList(uint32_t maxDisplays, CGDirectDisplayID *activeDisplays, uint32_t *displayCount) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGGetActiveDisplayList);
  return CGGetActiveDisplayList_ptr(maxDisplays, activeDisplays, displayCount);
}

// Cocoa :: CGGetDisplaysWithPoint
CARBON_WIN__DLDECL(CGGetDisplaysWithPoint);
CGError CGGetDisplaysWithPoint(CGPoint point, uint32_t maxDisplays, CGDirectDisplayID *displays, uint32_t *matchingDisplayCount) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGGetDisplaysWithPoint);
  return CGGetDisplaysWithPoint_ptr(point, maxDisplays, displays, matchingDisplayCount);
}

// Cocoa :: CGImageRelease
CARBON_WIN__DLDECL(CGImageRelease);
void CGImageRelease(CGImageRef image) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGImageRelease);
  return CGImageRelease_ptr(image);
}

// Cocoa :: CGMainDisplayID
CARBON_WIN__DLDECL(CGMainDisplayID);
CGDirectDisplayID CGMainDisplayID(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGMainDisplayID);
  return CGMainDisplayID_ptr();
}

// Cocoa :: CGWarpMouseCursorPosition
CARBON_WIN__DLDECL(CGWarpMouseCursorPosition);
CGError CGWarpMouseCursorPosition(CGPoint newCursorPosition) {
  CARBON_WIN__DLSYM(carbon_win__dl_Cocoa, CGWarpMouseCursorPosition);
  return CGWarpMouseCursorPosition_ptr(newCursorPosition);
}

// CoreVideo
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
// CoreVideo :: CVDisplayLinkCreateWithCGDisplay
CARBON_WIN__DLDECL(CVDisplayLinkCreateWithCGDisplay);
CVReturn CVDisplayLinkCreateWithCGDisplay(CGDirectDisplayID displayID, CVDisplayLinkRef *displayLinkOut) {
  CARBON_WIN__DLSYM(carbon_win__dl_CoreVideo, CVDisplayLinkCreateWithCGDisplay);
  return CVDisplayLinkCreateWithCGDisplay_ptr(displayID, displayLinkOut);
}
// CoreVideo :: CVDisplayLinkGetNominalOutputVideoRefreshPeriod
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

// IOKit
static void *carbon_win__dl_IOKit;

// IOKit :: IOHIDDeviceGetProperty
CARBON_WIN__DLDECL(IOHIDDeviceGetProperty);
CFTypeRef IOHIDDeviceGetProperty(IOHIDDeviceRef device, CFStringRef key) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDDeviceGetProperty);
  return IOHIDDeviceGetProperty_ptr(device, key);
}

// IOKit :: IOHIDDeviceRegisterInputValueCallback
CARBON_WIN__DLDECL(IOHIDDeviceRegisterInputValueCallback);
void IOHIDDeviceRegisterInputValueCallback(IOHIDDeviceRef device, IOHIDValueCallback callback, void *context) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDDeviceRegisterInputValueCallback);
  return IOHIDDeviceRegisterInputValueCallback_ptr(device, callback, context);
}

// IOKit :: IOHIDElementGetDevice
CARBON_WIN__DLDECL(IOHIDElementGetDevice);
IOHIDDeviceRef IOHIDElementGetDevice(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetDevice);
  return IOHIDElementGetDevice_ptr(element);
}

// IOKit :: IOHIDElementGetLogicalMax
CARBON_WIN__DLDECL(IOHIDElementGetLogicalMax);
CFIndex IOHIDElementGetLogicalMax(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetLogicalMax);
  return IOHIDElementGetLogicalMax_ptr(element);
}

// IOKit :: IOHIDElementGetLogicalMin
CARBON_WIN__DLDECL(IOHIDElementGetLogicalMin);
CFIndex IOHIDElementGetLogicalMin(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetLogicalMin);
  return IOHIDElementGetLogicalMin_ptr(element);
}

// IOKit :: IOHIDElementGetUsage
CARBON_WIN__DLDECL(IOHIDElementGetUsage);
uint32_t IOHIDElementGetUsage(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetUsage);
  return IOHIDElementGetUsage_ptr(element);
}

// IOKit :: IOHIDElementGetUsagePage
CARBON_WIN__DLDECL(IOHIDElementGetUsagePage);
uint32_t IOHIDElementGetUsagePage(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetUsagePage);
  return IOHIDElementGetUsagePage_ptr(element);
}

// IOKit :: IOHIDManagerCreate
CARBON_WIN__DLDECL(IOHIDManagerCreate);
IOHIDManagerRef IOHIDManagerCreate(CFAllocatorRef allocator, IOOptionBits options) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerCreate);
  return IOHIDManagerCreate_ptr(allocator, options);
}

// IOKit :: IOHIDManagerOpen
CARBON_WIN__DLDECL(IOHIDManagerOpen);
IOReturn IOHIDManagerOpen(IOHIDManagerRef manager, IOOptionBits options) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerOpen);
  return IOHIDManagerOpen_ptr(manager, options);
}

// IOKit :: IOHIDManagerRegisterDeviceMatchingCallback
CARBON_WIN__DLDECL(IOHIDManagerRegisterDeviceMatchingCallback);
void IOHIDManagerRegisterDeviceMatchingCallback(IOHIDManagerRef manager, IOHIDDeviceCallback callback, void *context) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerRegisterDeviceMatchingCallback);
  return IOHIDManagerRegisterDeviceMatchingCallback_ptr(manager, callback, context);
}

// IOKit :: IOHIDManagerRegisterDeviceRemovalCallback
CARBON_WIN__DLDECL(IOHIDManagerRegisterDeviceRemovalCallback);
void IOHIDManagerRegisterDeviceRemovalCallback(IOHIDManagerRef manager, IOHIDDeviceCallback callback, void *context) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerRegisterDeviceRemovalCallback);
  return IOHIDManagerRegisterDeviceRemovalCallback_ptr(manager, callback, context);
}

// IOKit :: IOHIDManagerScheduleWithRunLoop
CARBON_WIN__DLDECL(IOHIDManagerScheduleWithRunLoop);
void IOHIDManagerScheduleWithRunLoop(IOHIDManagerRef manager, CFRunLoopRef runLoop, CFStringRef runLoopMode) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerScheduleWithRunLoop);
  return IOHIDManagerScheduleWithRunLoop_ptr(manager, runLoop, runLoopMode);
}

// IOKit :: IOHIDManagerSetDeviceMatching
CARBON_WIN__DLDECL(IOHIDManagerSetDeviceMatching);
void IOHIDManagerSetDeviceMatching(IOHIDManagerRef manager, CFDictionaryRef matching) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerSetDeviceMatching);
  return IOHIDManagerSetDeviceMatching_ptr(manager, matching);
}

// IOKit :: IOHIDValueGetElement
CARBON_WIN__DLDECL(IOHIDValueGetElement);
IOHIDElementRef IOHIDValueGetElement(IOHIDValueRef value) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDValueGetElement);
  return IOHIDValueGetElement_ptr(value);
}

// IOKit :: IOHIDValueGetIntegerValue
CARBON_WIN__DLDECL(IOHIDValueGetIntegerValue);
CFIndex IOHIDValueGetIntegerValue(IOHIDValueRef value) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDValueGetIntegerValue);
  return IOHIDValueGetIntegerValue_ptr(value);
}
#endif

CARBON_INLINE void carbon_win__dl_open(void) {
#if defined(__APPLE__)
  CARBON_WIN__DLOPEN(carbon_win__dl_Cocoa, CARBON_WIN__DL_BASE_PATH "Cocoa.framework/Cocoa");
  CARBON_WIN__DLOPEN(carbon_win__dl_CoreVideo, CARBON_WIN__DL_BASE_PATH "CoreVideo.framework/CoreVideo");
  CARBON_WIN__DLOPEN(carbon_win__dl_IOKit, CARBON_WIN__DL_BASE_PATH "IOKit.framework/IOKit");
  // Global Variables
  CARBON_WIN__DLSYM_VAR(carbon_win__dl_Cocoa, kCFAllocatorDefault);
  CARBON_WIN__DLSYM_VAR(carbon_win__dl_Cocoa, kCFRunLoopDefaultMode);
  CARBON_WIN__DLSYM_VAR(carbon_win__dl_Cocoa, kCFTypeDictionaryKeyCallBacks);
  CARBON_WIN__DLSYM_VAR(carbon_win__dl_Cocoa, kCFTypeDictionaryValueCallBacks);
#endif
}

CARBON_INLINE void carbon_win__dl_close(void) {
#if defined(__APPLE__)
  dlclose(carbon_win__dl_Cocoa);
  dlclose(carbon_win__dl_CoreVideo);
  dlclose(carbon_win__dl_IOKit);
#endif
}

void carbon_win_open(u16 width, u16 height, const char *title) {
  carbon_win__dl_open();
  RGFW_area screen_size = RGFW_getScreenSize();
  // TODO: let user resize the window.
  carbon_win__handle = RGFW_createWindow(title, RGFW_RECT((screen_size.w - width) / 2,
                                                          (screen_size.h - height) / 2,
                                                          width, height), RGFW_windowNoResize);
  // TODO: use `RGFW_window_initBuffer` instead, so the buffer isn't tied up to the initial window size.
  RGFW_window_initBufferSize(carbon_win__handle, RGFW_AREA(carbon_win__handle->r.w, carbon_win__handle->r.h));
  carbon_log_info("Opened window at (%$, %$) of size %$x%$",
                  $(carbon_win__handle->r.x), $(carbon_win__handle->r.y),
                  $(carbon_win__handle->r.w), $(carbon_win__handle->r.h));
}

void carbon_win_close(void) {
  if (carbon_win__icon.data) carbon_fs_destroy_img(&carbon_win__icon);
  RGFW_window_close(carbon_win__handle);
  carbon_win__dl_close();
  carbon_log_info("Window closed successfully");
}

void carbon_win_set_max_fps(u32 fps) {
  carbon_win__max_fps = fps;
  carbon_log_info("Window max FPS set to %$", $(carbon_win__max_fps));
}

void carbon_win_set_icon(CBN_Image img) {
  carbon_win__icon = img;
  RGFW_bool status = RGFW_window_setIcon(carbon_win__handle,
                                         carbon_win__icon.data,
                                         RGFW_AREA(carbon_win__icon.metadata.width,
                                                   carbon_win__icon.metadata.height),
                                         carbon_win__icon.metadata.channels);
  if (!status) carbon_log_error("Unable to set the window icon");
  else carbon_log_info("Set window icon correctly");
}

f64 carbon_win_get_deltatime(void) {
  static f64 last_frame_time;
  static u8 is_first_time = true;
  if (is_first_time) {
    last_frame_time = carbon_time_get();
    is_first_time = false;
  }
  f64 curr_frame_time = carbon_time_get();
  f64 dt = curr_frame_time - last_frame_time;
  last_frame_time = curr_frame_time;
  return dt;
}

void carbon_win_update(CBN_DrawCanvas dc) {
  for (usz i = 0; i < dc.width * dc.height; ++i) {
    u32 color = dc.pixels[i];
    u8 r = (color >> 24) & 0xff;
    u8 g = (color >> 16) & 0xff;
    u8 b = (color >> 8)  & 0xff;
    u8 a = (color >> 0)  & 0xff;
    carbon_win__handle->buffer[i * 4 + 0] = r;
    carbon_win__handle->buffer[i * 4 + 1] = g;
    carbon_win__handle->buffer[i * 4 + 2] = b;
    carbon_win__handle->buffer[i * 4 + 3] = a;
  }
  RGFW_window_swapBuffers(carbon_win__handle);
  RGFW_window_checkFPS(carbon_win__handle, carbon_win__max_fps);
}

u8 carbon_win_shouldclose(void) {
  RGFW_window_checkEvent(carbon_win__handle);
  return RGFW_window_shouldClose(carbon_win__handle);
}
