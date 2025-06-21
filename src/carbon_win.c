// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_WIN__DLDECL(name)                \
  typedef CARBON_TYPE_OF(&name) name ## _ptr_t; \
  static name ## _ptr_t name ## _ptr;

#define CARBON_WIN__DLDECL_VAR(name)            \
  typedef CARBON_TYPE_OF(name) name ## _t;      \
  name ## _t *name ## _ptr;

#if defined(__linux__) || defined(__FreeBSD__)
#define CARBON_WIN__DLOPEN(lib, name)                           \
  lib = dlmopen(LM_ID_NEWLM, name, RTLD_LAZY | RTLD_LOCAL);     \
  CARBON_ASSERT(lib && "Failed to load");
#else
#define CARBON_WIN__DLOPEN(lib, name)           \
  lib = dlopen(name, RTLD_LAZY | RTLD_LOCAL);   \
  CARBON_ASSERT(lib && "Failed to load");
#endif

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

#elif defined(__linux__) || defined(__FreeBSD__)
// X11
static void *carbon_win__dl_X11;

// X11 :: XSendEvent
CARBON_WIN__DLDECL(XSendEvent);
Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSendEvent);
  return XSendEvent_ptr(display, w, propagate, event_mask, event_send);
}

// X11 :: XTranslateCoordinates
CARBON_WIN__DLDECL(XTranslateCoordinates);
Bool XTranslateCoordinates(Display *display,
                           Window src_w,
                           Window dest_w,
                           int src_x,
                           int src_y,
                           int *dest_x_return,
                           int *dest_y_return,
                           Window *child_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XTranslateCoordinates);
  return XTranslateCoordinates_ptr(display, src_w, dest_w, src_x, src_y, dest_x_return, dest_y_return, child_return);
}

// X11 :: XGetKeyboardControl
CARBON_WIN__DLDECL(XGetKeyboardControl);
int XGetKeyboardControl(Display *display, XKeyboardState *values_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetKeyboardControl);
  return XGetKeyboardControl_ptr(display, values_return);
}

// X11 :: XResizeWindow
CARBON_WIN__DLDECL(XResizeWindow);
int XResizeWindow(Display *display, Window w, unsigned width, unsigned height) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XResizeWindow);
  return XResizeWindow_ptr(display, w, width, height);
}

// X11 :: XCreateFontCursor
CARBON_WIN__DLDECL(XCreateFontCursor);
Cursor XCreateFontCursor(Display *display, unsigned int shape) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateFontCursor);
  return XCreateFontCursor_ptr(display, shape);
}

// X11 :: XrmGetResource
CARBON_WIN__DLDECL(XrmGetResource);
Bool XrmGetResource(XrmDatabase database,
                    const char *str_name,
                    const char *str_class,
                    char **str_type_return,
                    XrmValue *value_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XrmGetResource);
  return XrmGetResource_ptr(database, str_name, str_class, str_type_return, value_return);
}

// X11 :: XStoreName
CARBON_WIN__DLDECL(XStoreName);
int XStoreName(Display *display, Window w, const char *window_name) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XStoreName);
  return XStoreName_ptr(display, w, window_name);
}

// X11 :: XEventsQueued
CARBON_WIN__DLDECL(XEventsQueued);
int XEventsQueued(Display *display, int mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XEventsQueued);
  return XEventsQueued_ptr(display, mode);
}

// X11 :: XPending
CARBON_WIN__DLDECL(XPending);
int XPending(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XPending);
  return XPending_ptr(display);
}

// X11 :: XkbGetState
CARBON_WIN__DLDECL(XkbGetState);
Status XkbGetState(Display *display, unsigned int device_spec, XkbStatePtr state_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XkbGetState);
  return XkbGetState_ptr(display, device_spec, state_return);
}

// X11 :: XIconifyWindow
CARBON_WIN__DLDECL(XIconifyWindow);
Status XIconifyWindow(Display *display, Window w, int screen_number) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XIconifyWindow);
  return XIconifyWindow_ptr(display, w, screen_number);
}

// X11 :: XFreeEventData
CARBON_WIN__DLDECL(XFreeEventData);
void XFreeEventData(Display *display, XGenericEventCookie *cookie) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeEventData);
  return XFreeEventData_ptr(display, cookie);
}

// X11 :: XGetWMNormalHints
CARBON_WIN__DLDECL(XGetWMNormalHints);
Status XGetWMNormalHints(Display *display, Window w, XSizeHints *hints_return, long *supplied_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetWMNormalHints);
  return XGetWMNormalHints_ptr(display, w, hints_return, supplied_return);
}

// X11 :: XFreeColors
CARBON_WIN__DLDECL(XFreeColors);
int XFreeColors(Display *display, Colormap colormap, unsigned long pixels[], int npixels, unsigned long planes) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeColors);
  return XFreeColors_ptr(display, colormap, pixels, npixels, planes);
}

// X11 :: XDestroyRegion
CARBON_WIN__DLDECL(XDestroyRegion);
int XDestroyRegion(Region r) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDestroyRegion);
  return XDestroyRegion_ptr(r);
}

// X11 :: XFree
CARBON_WIN__DLDECL(XFree);
int XFree(void *data) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFree);
  return XFree_ptr(data);
}

// X11 :: XMoveWindow
CARBON_WIN__DLDECL(XMoveWindow);
int XMoveWindow(Display *display, Window w, int x, int y) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMoveWindow);
  return XMoveWindow_ptr(display, w, x, y);
}

// X11 :: XDeleteProperty
CARBON_WIN__DLDECL(XDeleteProperty);
int XDeleteProperty(Display *display, Window w, Atom property) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDeleteProperty);
  return XDeleteProperty_ptr(display, w, property);
}

// X11 :: XrmGetStringDatabase
CARBON_WIN__DLDECL(XrmGetStringDatabase);
XrmDatabase XrmGetStringDatabase(const char *data) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XrmGetStringDatabase);
  return XrmGetStringDatabase_ptr(data);
}

// X11 :: XSetWMHints
CARBON_WIN__DLDECL(XSetWMHints);
int XSetWMHints(Display *display, Window w, XWMHints *wmhints) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMHints);
  return XSetWMHints_ptr(display, w, wmhints);
}

// X11 :: XPeekEvent
CARBON_WIN__DLDECL(XPeekEvent);
int XPeekEvent(Display *display, XEvent *event_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XPeekEvent);
  return XPeekEvent_ptr(display, event_return);
}

// X11 :: XDefaultVisual
CARBON_WIN__DLDECL(XDefaultVisual);
Visual *XDefaultVisual(Display *display, int screen) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefaultVisual);
  return XDefaultVisual_ptr(display, screen);
}

// X11 :: XSetWMNormalHints
CARBON_WIN__DLDECL(XSetWMNormalHints);
void XSetWMNormalHints(Display *display, Window w, XSizeHints *hints) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMNormalHints);
  XSetWMNormalHints_ptr(display, w, hints);
}

// X11 :: XGrabPointer
CARBON_WIN__DLDECL(XGrabPointer);
int XGrabPointer(Display *display,
                 Window grab_window,
                 Bool owner_events,
                 unsigned int event_mask,
                 int pointer_mode,
                 int keyboard_mode,
                 Window confine_to,
                 Cursor cursor,
                 Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGrabPointer);
  return XGrabPointer_ptr(display, grab_window, owner_events, event_mask, pointer_mode, keyboard_mode, confine_to, cursor, time);
}

// X11 :: XInitThreads
CARBON_WIN__DLDECL(XInitThreads);
Status XInitThreads(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XInitThreads);
  return XInitThreads_ptr();
}

// X11 :: XWidthOfScreen
CARBON_WIN__DLDECL(XWidthOfScreen);
int XWidthOfScreen(Screen *screen) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XWidthOfScreen);
  return XWidthOfScreen_ptr(screen);
}

// X11 :: XDefaultScreen
CARBON_WIN__DLDECL(XDefaultScreen);
int XDefaultScreen(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefaultScreen);
  return XDefaultScreen_ptr(display);
}

// X11 :: XWarpPointer
CARBON_WIN__DLDECL(XWarpPointer);
int XWarpPointer(Display *display,
                 Window src_w,
                 Window dest_w,
                 int src_x,
                 int src_y,
                 unsigned int src_width,
                 unsigned int src_height,
                 int dest_x,
                 int dest_y) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XWarpPointer);
  return XWarpPointer_ptr(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y);
}

// X11 :: XCreateRegion
CARBON_WIN__DLDECL(XCreateRegion);
Region XCreateRegion(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateRegion);
  return XCreateRegion_ptr();
}

// X11 :: XHeightOfScreen
CARBON_WIN__DLDECL(XHeightOfScreen);
int XHeightOfScreen(Screen *screen) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XHeightOfScreen);
  return XHeightOfScreen_ptr(screen);
}

// X11 :: XDestroyWindow
CARBON_WIN__DLDECL(XDestroyWindow);
int XDestroyWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDestroyWindow);
  return XDestroyWindow_ptr(display, w);
}

// X11 :: XOpenDisplay
CARBON_WIN__DLDECL(XOpenDisplay);
Display *XOpenDisplay(const char *display_name) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XOpenDisplay);
  return XOpenDisplay_ptr(display_name);
}

// X11 :: XSetWMProtocols
CARBON_WIN__DLDECL(XSetWMProtocols);
Status XSetWMProtocols(Display *display, Window w, Atom *protocols, int count) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMProtocols);
  return XSetWMProtocols_ptr(display, w, protocols, count);
}

// X11 :: XQueryPointer
CARBON_WIN__DLDECL(XQueryPointer);
Bool XQueryPointer(Display *display,
                   Window w,
                   Window *root_return,
                   Window *child_return,
                   int *root_x_return,
                   int *root_y_return,
                   int *win_x_return,
                   int *win_y_return,
                   unsigned int *mask_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XQueryPointer);
  return XQueryPointer_ptr(display, w, root_return, child_return, root_x_return, root_y_return, win_x_return, win_y_return, mask_return);
}

// X11 :: XUngrabPointer
CARBON_WIN__DLDECL(XUngrabPointer);
int XUngrabPointer(Display *display, Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XUngrabPointer);
  return XUngrabPointer_ptr(display, time);
}

// X11 :: XGetSelectionOwner
CARBON_WIN__DLDECL(XGetSelectionOwner);
Window XGetSelectionOwner(Display *display, Atom selection) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetSelectionOwner);
  return XGetSelectionOwner_ptr(display, selection);
}

// X11 :: XNextEvent
CARBON_WIN__DLDECL(XNextEvent);
int XNextEvent(Display *display, XEvent *event_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XNextEvent);
  return XNextEvent_ptr(display, event_return);
}

// X11 :: XResourceManagerString
CARBON_WIN__DLDECL(XResourceManagerString);
char *XResourceManagerString(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XResourceManagerString);
  return XResourceManagerString_ptr(display);
}

// X11 :: XMapRaised
CARBON_WIN__DLDECL(XMapRaised);
int XMapRaised(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMapRaised);
  return XMapRaised_ptr(display, w);
}

// X11 :: XInternAtom
CARBON_WIN__DLDECL(XInternAtom);
Atom XInternAtom(Display *display, const char *atom_name, Bool only_if_exists) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XInternAtom);
  return XInternAtom_ptr(display, atom_name, only_if_exists);
}

// X11 :: XGetEventData
CARBON_WIN__DLDECL(XGetEventData);
Status XGetEventData(Display *display, XGenericEventCookie *cookie) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetEventData);
  return XGetEventData_ptr(display, cookie);
}

// X11 :: XChangeProperty
CARBON_WIN__DLDECL(XChangeProperty);
int XChangeProperty(Display *display,
                    Window w,
                    Atom property,
                    Atom type,
                    int format,
                    int mode,
                    const unsigned char *data,
                    int nelements) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XChangeProperty);
  return XChangeProperty_ptr(display, w, property, type, format, mode, data, nelements);
}

// X11 :: XDisplayName
CARBON_WIN__DLDECL(XDisplayName);
char *XDisplayName(const char *name) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDisplayName);
  return XDisplayName_ptr(name);
}

// X11 :: XCreateWindow
CARBON_WIN__DLDECL(XCreateWindow);
Window XCreateWindow(Display *display,
                     Window parent,
                     int x,
                     int y,
                     unsigned int width,
                     unsigned int height,
                     unsigned int border_width,
                     int depth,
                     unsigned int class,
                     Visual *visual,
                     unsigned long valuemask,
                     XSetWindowAttributes *attributes) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateWindow);
  return XCreateWindow_ptr(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
}

// X11 :: XSync
CARBON_WIN__DLDECL(XSync);
int XSync(Display *display, Bool discard) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSync);
  return XSync_ptr(display, discard);
}

// X11 :: XSelectInput
CARBON_WIN__DLDECL(XSelectInput);
int XSelectInput(Display *display, Window w, long event_mask) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSelectInput);
  return XSelectInput_ptr(display, w, event_mask);
}

// X11 :: XSetClassHint
CARBON_WIN__DLDECL(XSetClassHint);
int XSetClassHint(Display *display, Window w, XClassHint *class_hint) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetClassHint);
  return XSetClassHint_ptr(display, w, class_hint);
}

// X11 :: XSetWMSizeHints
CARBON_WIN__DLDECL(XSetWMSizeHints);
void XSetWMSizeHints(Display *display, Window w, XSizeHints *hints, Atom property) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMSizeHints);
  return XSetWMSizeHints_ptr(display, w, hints, property);
}

// X11 :: XSetSelectionOwner
CARBON_WIN__DLDECL(XSetSelectionOwner);
int XSetSelectionOwner(Display *display, Atom selection, Window owner, Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetSelectionOwner);
  return XSetSelectionOwner_ptr(display, selection, owner, time);
}

// X11 :: XDefineCursor
CARBON_WIN__DLDECL(XDefineCursor);
int XDefineCursor(Display *display, Window w, Cursor cursor) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefineCursor);
  return XDefineCursor_ptr(display, w, cursor);
}

// X11 :: XCreateGC
CARBON_WIN__DLDECL(XCreateGC);
GC XCreateGC(Display *display, Drawable d, unsigned long valuemask, XGCValues *values) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateGC);
  return XCreateGC_ptr(display, d, valuemask, values);
}

// X11 :: XUnmapWindow
CARBON_WIN__DLDECL(XUnmapWindow);
int XUnmapWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XUnmapWindow);
  return XUnmapWindow_ptr(display, w);
}

// X11 :: XDefaultRootWindow
CARBON_WIN__DLDECL(XDefaultRootWindow);
Window XDefaultRootWindow(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefaultRootWindow);
  return XDefaultRootWindow_ptr(display);
}

// X11 :: XFlush
CARBON_WIN__DLDECL(XFlush);
int XFlush(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFlush);
  return XFlush_ptr(display);
}

// X11 :: XCreateColormap
CARBON_WIN__DLDECL(XCreateColormap);
Colormap XCreateColormap(Display *display, Window w, Visual *visual, int alloc) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateColormap);
  return XCreateColormap_ptr(display, w, visual, alloc);
}

// X11 :: XGetWindowAttributes
CARBON_WIN__DLDECL(XGetWindowAttributes);
int XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetWindowAttributes);
  return XGetWindowAttributes_ptr(display, w, window_attributes_return);
}

// X11 :: XrmDestroyDatabase
CARBON_WIN__DLDECL(XrmDestroyDatabase);
void XrmDestroyDatabase(XrmDatabase db) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XrmDestroyDatabase);
  XrmDestroyDatabase_ptr(db);
}

// X11 :: XPutImage
CARBON_WIN__DLDECL(XPutImage);
int XPutImage(Display *display,
              Drawable d,
              GC gc,
              XImage *image,
              int src_x,
              int src_y,
              int dest_x,
              int dest_y,
              unsigned int width,
              unsigned int height) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XPutImage);
  return XPutImage_ptr(display, d, gc, image, src_x, src_y, dest_x, dest_y, width, height);
}

// X11 :: XMatchVisualInfo
CARBON_WIN__DLDECL(XMatchVisualInfo);
Status XMatchVisualInfo(Display *display, int screen, int depth, int class, XVisualInfo *visual_info_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMatchVisualInfo);
  return XMatchVisualInfo_ptr(display, screen, depth, class, visual_info_return);
}

// X11 :: XFreeCursor
CARBON_WIN__DLDECL(XFreeCursor);
int XFreeCursor(Display *display, Cursor cursor) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeCursor);
  return XFreeCursor_ptr(display, cursor);
}

// X11 :: XMapWindow
CARBON_WIN__DLDECL(XMapWindow);
int XMapWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMapWindow);
  return XMapWindow_ptr(display, w);
}

// X11 :: XFreeGC
CARBON_WIN__DLDECL(XFreeGC);
int XFreeGC(Display *display, GC gc) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeGC);
  return XFreeGC_ptr(display, gc);
}

// X11 :: XGetWindowProperty
CARBON_WIN__DLDECL(XGetWindowProperty);
int XGetWindowProperty(Display *display,
                       Window w,
                       Atom property,
                       long long_offset,
                       long long_length,
                       Bool delete,
                       Atom req_type,
                       Atom *actual_type_return,
                       int *actual_format_return,
                       unsigned long *nitems_return,
                       unsigned long *bytes_after_return,
                       unsigned char **prop_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetWindowProperty);
  return XGetWindowProperty_ptr(display, w, property, long_offset, long_length, delete, req_type, actual_type_return, actual_format_return, nitems_return, bytes_after_return, prop_return);
}

// X11 :: XRaiseWindow
CARBON_WIN__DLDECL(XRaiseWindow);
int XRaiseWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XRaiseWindow);
  return XRaiseWindow_ptr(display, w);
}

// X11 :: XCloseDisplay
CARBON_WIN__DLDECL(XCloseDisplay);
int XCloseDisplay(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCloseDisplay);
  return XCloseDisplay_ptr(display);
}

// X11 :: XCreateImage
CARBON_WIN__DLDECL(XCreateImage);
XImage *XCreateImage(Display *display,
                     Visual *visual,
                     unsigned int depth,
                     int format,
                     int offset,
                     char *data,
                     unsigned int width,
                     unsigned int height,
                     int bitmap_pad,
                     int bytes_per_line) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateImage);
  return XCreateImage_ptr(display, visual, depth, format, offset, data, width, height, bitmap_pad, bytes_per_line);
}

// X11 :: XCreatePixmap
CARBON_WIN__DLDECL(XCreatePixmap);
Pixmap XCreatePixmap(Display *display,
                     Drawable d,
                     unsigned int width,
                     unsigned int height,
                     unsigned int depth) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreatePixmap);
  return XCreatePixmap_ptr(display, d, width, height, depth);
}

// X11 :: XkbKeycodeToKeysym
CARBON_WIN__DLDECL(XkbKeycodeToKeysym);
KeySym XkbKeycodeToKeysym(Display *display, KeyCode kc, int group, int level) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XkbKeycodeToKeysym);
  return XkbKeycodeToKeysym_ptr(display, kc, group, level);
}

// X11 :: XSetInputFocus
CARBON_WIN__DLDECL(XSetInputFocus);
int XSetInputFocus(Display *display, Window focus, int revert_to, Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetInputFocus);
  return XSetInputFocus_ptr(display, focus, revert_to, time);
}

// X11 :: XConvertSelection
CARBON_WIN__DLDECL(XConvertSelection);
int XConvertSelection(Display *display,
                      Atom selection,
                      Atom target,
                      Atom property,
                      Window requestor,
                      Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XConvertSelection);
  return XConvertSelection_ptr(display, selection, target, property, requestor, time);
}

// Xrandr
static void *carbon_win__dl_Xrandr;

// Xrandr :: XRRGetCrtcInfo
CARBON_WIN__DLDECL(XRRGetCrtcInfo);
XRRCrtcInfo *XRRGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetCrtcInfo);
  return XRRGetCrtcInfo_ptr(dpy, resources, crtc);
}

// Xrandr :: XRRFreeOutputInfo
CARBON_WIN__DLDECL(XRRFreeOutputInfo);
void XRRFreeOutputInfo(XRROutputInfo *outputInfo) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRFreeOutputInfo);
  return XRRFreeOutputInfo_ptr(outputInfo);
}

// Xrandr :: XRRGetScreenResources
CARBON_WIN__DLDECL(XRRGetScreenResources);
XRRScreenResources *XRRGetScreenResources(Display *dpy, Window window) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetScreenResources);
  return XRRGetScreenResources_ptr(dpy, window);
}

// Xrandr :: XRRFreeScreenResources
CARBON_WIN__DLDECL(XRRFreeScreenResources);
void XRRFreeScreenResources(XRRScreenResources *resources) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRFreeScreenResources);
  return XRRFreeScreenResources_ptr(resources);
}

// Xrandr :: XRRSetCrtcConfig
CARBON_WIN__DLDECL(XRRSetCrtcConfig);
Status XRRSetCrtcConfig(Display *dpy,
                        XRRScreenResources *res,
                        RRCrtc crtc,
                        Time timestamp,
                        int x,
                        int y,
                        RRMode mode,
                        Rotation rotation,
                        RROutput *outputs,
                        int noutputs) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRSetCrtcConfig);
  return XRRSetCrtcConfig_ptr(dpy, res, crtc, timestamp, x, y, mode, rotation, outputs, noutputs);
}

// Xrandr :: XRRGetScreenResourcesCurrent
CARBON_WIN__DLDECL(XRRGetScreenResourcesCurrent);
XRRScreenResources *XRRGetScreenResourcesCurrent(Display *dpy, Window window) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetScreenResourcesCurrent);
  return XRRGetScreenResourcesCurrent_ptr(dpy, window);
}

// Xrandr :: XRRFreeCrtcInfo
CARBON_WIN__DLDECL(XRRFreeCrtcInfo);
void XRRFreeCrtcInfo(XRRCrtcInfo *crtcInfo) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRFreeCrtcInfo);
  return XRRFreeCrtcInfo_ptr(crtcInfo);
}

// Xrandr :: XRRGetOutputInfo
CARBON_WIN__DLDECL(XRRGetOutputInfo);
XRROutputInfo *XRRGetOutputInfo(Display *dpy, XRRScreenResources *res, RROutput output) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetOutputInfo);
  return XRRGetOutputInfo_ptr(dpy, res, output);
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
#elif defined(__linux__) || defined(__FreeBSD__)
  CARBON_WIN__DLOPEN(carbon_win__dl_X11, "libX11.so");
  CARBON_WIN__DLOPEN(carbon_win__dl_Xrandr, "libXrandr.so");
#endif
}

CARBON_INLINE void carbon_win__dl_close(void) {
#if defined(__APPLE__)
  dlclose(carbon_win__dl_Cocoa);
  dlclose(carbon_win__dl_CoreVideo);
  dlclose(carbon_win__dl_IOKit);
#elif defined(__linux__) || defined(__FreeBSD__)
  dlclose(carbon_win__dl_X11);
  dlclose(carbon_win__dl_Xrandr);
#endif
}

void carbon_win_open(u16 width, u16 height, const char *title) {
  carbon_win__dl_open();
  // TODO: let user resize the window.
  carbon_win__handle = RGFW_createWindow(title, RGFW_RECT(0, 0, width, height), RGFW_windowNoResize);
  // TODO: use `RGFW_window_initBuffer` instead, so the buffer isn't tied up to the initial window size.
  RGFW_window_initBufferSize(carbon_win__handle, RGFW_AREA(carbon_win__handle->r.w, carbon_win__handle->r.h));
  carbon_log_info("Opened a %$x%$ window", $(carbon_win__handle->r.w), $(carbon_win__handle->r.h));
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
