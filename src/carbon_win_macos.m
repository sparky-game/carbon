// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#ifdef __APPLE__

#import <Cocoa/Cocoa.h>

static NSApplication *carbon_win__app;
static NSWindow *carbon_win__window;
static NSView *carbon_win__view;
static BOOL carbon_win__should_close;

void carbon_win_open(u16 width, u16 height, const char *title) {
  @autoreleasepool {
    // Initialize the shared application instance
    carbon_win__app = [NSApplication sharedApplication];
    [carbon_win__app setActivationPolicy:NSApplicationActivationPolicyRegular];
    // Create the window
    NSRect frame = NSMakeRect(0, 0, width, height);
    carbon_win__window = [[NSWindow alloc]
                           initWithContentRect:frame
                                     styleMask:(NSWindowStyleMaskTitled   |
                                                NSWindowStyleMaskClosable |
                                                NSWindowStyleMaskResizable)
                                       backing:NSBackingStoreBuffered
                                         defer:NO];
    NSString *win_title = [NSString stringWithUTF8String:title];
    [carbon_win__window setTitle:win_title];
    [carbon_win__window makeKeyAndOrderFront:nil];
    // Add a custom view for drawing
    carbon_win__view = [[NSView alloc] initWithFrame:frame];
    [carbon_win__window setContentView:carbon_win__view];
    // Track if the window is requested to close
    carbon_win__should_close = NO;
    // Setup window close notification
    [[NSNotificationCenter defaultCenter]
      addObserverForName:NSWindowWillCloseNotification
                  object:carbon_win__window
                   queue:nil
              usingBlock:^(__unused NSNotification *note) { carbon_win__should_close = YES; }];
    [carbon_win__app finishLaunching];
  }
}

void carbon_win_close(void) {
  @autoreleasepool {
    [[NSNotificationCenter defaultCenter] removeObserver:carbon_win__window];
    [carbon_win__window close];
    carbon_win__window = nil;
    carbon_win__view = nil;
    carbon_win__app = nil;
    carbon_win__should_close = NO;
  }
}

void carbon_win_update(CBN_DrawCanvas dc) {
  @autoreleasepool {
    if (!carbon_win__view) return;
    // Lock and use the provided draw canvas to update the window
    NSBitmapImageRep *image_repr = [[NSBitmapImageRep alloc]
                                     initWithBitmapDataPlanes:(u8 **) &dc.pixels
                                                   pixelsWide:(i32) dc.width
                                                   pixelsHigh:(i32) dc.height
                                                bitsPerSample:8
                                              samplesPerPixel:4
                                                     hasAlpha:YES
                                                     isPlanar:NO
                                               colorSpaceName:NSDeviceRGBColorSpace
                                                 bitmapFormat:NSBitmapFormatAlphaFirst
                                                  bytesPerRow:(i32) dc.stride
                                                 bitsPerPixel:32];
    NSImage *image = [[NSImage alloc] init];
    [image addRepresentation:image_repr];
    // Draw the image in the view
    [carbon_win__view lockFocus];
    [image drawInRect:[carbon_win__view bounds]];
    [carbon_win__view unlockFocus];
  }
}

u8 carbon_win_shouldclose(void) {
  return carbon_win__should_close;
}

#endif  // __APPLE__
