// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

static void *carbon_win__dl_IOKit;

CARBON_WIN__DLDECL(IOHIDDeviceGetProperty);
CFTypeRef IOHIDDeviceGetProperty(IOHIDDeviceRef device, CFStringRef key) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDDeviceGetProperty);
  return IOHIDDeviceGetProperty_ptr(device, key);
}

CARBON_WIN__DLDECL(IOHIDDeviceRegisterInputValueCallback);
void IOHIDDeviceRegisterInputValueCallback(IOHIDDeviceRef device, IOHIDValueCallback callback, void *context) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDDeviceRegisterInputValueCallback);
  return IOHIDDeviceRegisterInputValueCallback_ptr(device, callback, context);
}

CARBON_WIN__DLDECL(IOHIDElementGetDevice);
IOHIDDeviceRef IOHIDElementGetDevice(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetDevice);
  return IOHIDElementGetDevice_ptr(element);
}

CARBON_WIN__DLDECL(IOHIDElementGetLogicalMax);
CFIndex IOHIDElementGetLogicalMax(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetLogicalMax);
  return IOHIDElementGetLogicalMax_ptr(element);
}

CARBON_WIN__DLDECL(IOHIDElementGetLogicalMin);
CFIndex IOHIDElementGetLogicalMin(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetLogicalMin);
  return IOHIDElementGetLogicalMin_ptr(element);
}

CARBON_WIN__DLDECL(IOHIDElementGetUsage);
uint32_t IOHIDElementGetUsage(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetUsage);
  return IOHIDElementGetUsage_ptr(element);
}

CARBON_WIN__DLDECL(IOHIDElementGetUsagePage);
uint32_t IOHIDElementGetUsagePage(IOHIDElementRef element) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDElementGetUsagePage);
  return IOHIDElementGetUsagePage_ptr(element);
}

CARBON_WIN__DLDECL(IOHIDManagerCreate);
IOHIDManagerRef IOHIDManagerCreate(CFAllocatorRef allocator, IOOptionBits options) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerCreate);
  return IOHIDManagerCreate_ptr(allocator, options);
}

CARBON_WIN__DLDECL(IOHIDManagerOpen);
IOReturn IOHIDManagerOpen(IOHIDManagerRef manager, IOOptionBits options) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerOpen);
  return IOHIDManagerOpen_ptr(manager, options);
}

CARBON_WIN__DLDECL(IOHIDManagerRegisterDeviceMatchingCallback);
void IOHIDManagerRegisterDeviceMatchingCallback(IOHIDManagerRef manager, IOHIDDeviceCallback callback, void *context) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerRegisterDeviceMatchingCallback);
  return IOHIDManagerRegisterDeviceMatchingCallback_ptr(manager, callback, context);
}

CARBON_WIN__DLDECL(IOHIDManagerRegisterDeviceRemovalCallback);
void IOHIDManagerRegisterDeviceRemovalCallback(IOHIDManagerRef manager, IOHIDDeviceCallback callback, void *context) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerRegisterDeviceRemovalCallback);
  return IOHIDManagerRegisterDeviceRemovalCallback_ptr(manager, callback, context);
}

CARBON_WIN__DLDECL(IOHIDManagerScheduleWithRunLoop);
void IOHIDManagerScheduleWithRunLoop(IOHIDManagerRef manager, CFRunLoopRef runLoop, CFStringRef runLoopMode) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerScheduleWithRunLoop);
  return IOHIDManagerScheduleWithRunLoop_ptr(manager, runLoop, runLoopMode);
}

CARBON_WIN__DLDECL(IOHIDManagerSetDeviceMatching);
void IOHIDManagerSetDeviceMatching(IOHIDManagerRef manager, CFDictionaryRef matching) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDManagerSetDeviceMatching);
  return IOHIDManagerSetDeviceMatching_ptr(manager, matching);
}

CARBON_WIN__DLDECL(IOHIDValueGetElement);
IOHIDElementRef IOHIDValueGetElement(IOHIDValueRef value) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDValueGetElement);
  return IOHIDValueGetElement_ptr(value);
}

CARBON_WIN__DLDECL(IOHIDValueGetIntegerValue);
CFIndex IOHIDValueGetIntegerValue(IOHIDValueRef value) {
  CARBON_WIN__DLSYM(carbon_win__dl_IOKit, IOHIDValueGetIntegerValue);
  return IOHIDValueGetIntegerValue_ptr(value);
}

// Local Variables:
// mode: c
// End:
