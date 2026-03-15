// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#import <Metal/Metal.h>
#import <AppKit/NSView.h>
#import <QuartzCore/CAMetalLayer.h>

@interface CBNView : NSView
@end

@implementation CBNView
- (void)cursorUpdate:(NSEvent *)event {
  if (carbon_win__cursor_visible) [[NSCursor arrowCursor] set];
  else [NSCursor hide];
}
@end

static NSApplication *carbon_win__app;
static NSWindow *carbon_win__window;

static id<MTLDevice> carbon_win__mtl_device;
static id<MTLCommandQueue> carbon_win__mtl_queue;
static CAMetalLayer *carbon_win__mtl_layer;
static id<MTLRenderPipelineState> carbon_win__mtl_pipeline;
static id<MTLTexture> carbon_win__mtl_texture;

CBNINL void carbon_win__create_window(usz w, usz h, const char *title) {
  carbon_win__app = [NSApplication sharedApplication];
  [carbon_win__app setActivationPolicy:NSApplicationActivationPolicyRegular];
  carbon_win__window = [[NSWindow alloc]
                         initWithContentRect:NSMakeRect(0, 0, w, h)
                                   styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
                                     backing:NSBackingStoreBuffered
                                       defer:NO];
  CBNView *view = [[CBNView alloc] initWithFrame:NSMakeRect(0, 0, w, h)];
  NSTrackingArea *tracking = [[NSTrackingArea alloc]
                               initWithRect:[view bounds]
                                    options:NSTrackingCursorUpdate | NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect
                                      owner:view
                                   userInfo:nil];
  [view addTrackingArea:tracking];
  [carbon_win__window setContentView:view];
  [carbon_win__window setTitle:[NSString stringWithUTF8String:title]];
  [carbon_win__window center];
  [carbon_win__window makeKeyAndOrderFront:nil];
  [carbon_win__app activateIgnoringOtherApps:YES];
}

CBNINL void carbon_win__destroy_window(void) {
  [carbon_win__window close];
  carbon_win__window = nil;
  carbon_win__app = nil;
}

CBNINL bool carbon_win__poll_event(void) {
  NSEvent *e = [carbon_win__app nextEventMatchingMask:NSEventMaskAny
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];
  if (!e) return false;
  [carbon_win__app sendEvent:e];
  return true;
}

CBNINL void carbon_win__renderer_init(void *view, usz w, usz h) {
  CBNView *cbn_view = (CBNView *) view;
  carbon_win__mtl_device = MTLCreateSystemDefaultDevice();
  carbon_win__mtl_queue  = [carbon_win__mtl_device newCommandQueue];
  carbon_win__mtl_layer  = [CAMetalLayer layer];
  carbon_win__mtl_layer.device          = carbon_win__mtl_device;
  carbon_win__mtl_layer.pixelFormat     = MTLPixelFormatBGRA8Unorm;
  carbon_win__mtl_layer.framebufferOnly = NO;
  cbn_view.wantsLayer = YES;
  cbn_view.layer      = carbon_win__mtl_layer;
  // ...
}

CBNINL void carbon_win__renderer_present(const u32 *pixels, usz w, usz h) {
  [carbon_win__mtl_texture replaceRegion:MTLRegionMake2D(0, 0, w, h)
                             mipmapLevel:0
                               withBytes:pixels
                             bytesPerRow:w*4];
  id<CAMetalDrawable> drawable = [carbon_win__mtl_layer nextDrawable];
  if (!drawable) return;
  MTLRenderPassDescriptor *pass = [MTLRenderPassDescriptor renderPassDescriptor];
  pass.colorAttachments[0].texture     = drawable.texture;
  pass.colorAttachments[0].loadAction  = MTLLoadActionClear;
  pass.colorAttachments[0].storeAction = MTLStoreActionStore;
  id<MTLCommandBuffer> cmd = [carbon_win__mtl_queue commandBuffer];
  id<MTLRenderCommandEncoder> enc = [cmd renderCommandEncoderWithDescriptor:pass];
  [enc setRenderPipelineState:carbon_win__mtl_pipeline];
  [enc setFragmentTexture:carbon_win__mtl_texture atIndex:0];
  [enc drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
  [enc endEncoding];
  [cmd presentDrawable:drawable];
  [cmd commit];
}

CBNINL void carbon_win__renderer_shutdown(void) {
  [carbon_win__mtl_texture  release];
  [carbon_win__mtl_pipeline release];
  [carbon_win__mtl_layer    release];
  [carbon_win__mtl_queue    release];
  [carbon_win__mtl_device   release];
}
