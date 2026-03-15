// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#import <Metal/Metal.h>
#import <Carbon/Carbon.h>
#import <AppKit/AppKit.h>
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

CBNINL CBN_Win_KeyCode carbon_win__map_key_code(u16 key) {
  // <https://forums.macrumors.com/threads/nsevent-keycode-list.780577/post-8428116>
  // <https://boredzo.org/blog/wp-content/uploads/2007/05/IMTx-virtual-keycodes.pdf>
  switch (key) {
  case kVK_ANSI_A:       return CBN_Win_KeyCode_A;
  case kVK_ANSI_B:       return CBN_Win_KeyCode_B;
  case kVK_ANSI_C:       return CBN_Win_KeyCode_C;
  case kVK_ANSI_D:       return CBN_Win_KeyCode_D;
  case kVK_ANSI_E:       return CBN_Win_KeyCode_E;
  case kVK_ANSI_F:       return CBN_Win_KeyCode_F;
  case kVK_ANSI_G:       return CBN_Win_KeyCode_G;
  case kVK_ANSI_H:       return CBN_Win_KeyCode_H;
  case kVK_ANSI_I:       return CBN_Win_KeyCode_I;
  case kVK_ANSI_J:       return CBN_Win_KeyCode_J;
  case kVK_ANSI_K:       return CBN_Win_KeyCode_K;
  case kVK_ANSI_L:       return CBN_Win_KeyCode_L;
  case kVK_ANSI_M:       return CBN_Win_KeyCode_M;
  case kVK_ANSI_N:       return CBN_Win_KeyCode_N;
  case kVK_ANSI_O:       return CBN_Win_KeyCode_O;
  case kVK_ANSI_P:       return CBN_Win_KeyCode_P;
  case kVK_ANSI_Q:       return CBN_Win_KeyCode_Q;
  case kVK_ANSI_R:       return CBN_Win_KeyCode_R;
  case kVK_ANSI_S:       return CBN_Win_KeyCode_S;
  case kVK_ANSI_T:       return CBN_Win_KeyCode_T;
  case kVK_ANSI_U:       return CBN_Win_KeyCode_U;
  case kVK_ANSI_V:       return CBN_Win_KeyCode_V;
  case kVK_ANSI_W:       return CBN_Win_KeyCode_W;
  case kVK_ANSI_X:       return CBN_Win_KeyCode_X;
  case kVK_ANSI_Y:       return CBN_Win_KeyCode_Y;
  case kVK_ANSI_Z:       return CBN_Win_KeyCode_Z;
  case kVK_ANSI_0:       return CBN_Win_KeyCode_Zero;
  case kVK_ANSI_1:       return CBN_Win_KeyCode_One;
  case kVK_ANSI_2:       return CBN_Win_KeyCode_Two;
  case kVK_ANSI_3:       return CBN_Win_KeyCode_Three;
  case kVK_ANSI_4:       return CBN_Win_KeyCode_Four;
  case kVK_ANSI_5:       return CBN_Win_KeyCode_Five;
  case kVK_ANSI_6:       return CBN_Win_KeyCode_Six;
  case kVK_ANSI_7:       return CBN_Win_KeyCode_Seven;
  case kVK_ANSI_8:       return CBN_Win_KeyCode_Eight;
  case kVK_ANSI_9:       return CBN_Win_KeyCode_Nine;
  case kVK_ANSI_Grave:   return CBN_Win_KeyCode_BackQuote;
  case kVK_F1:           return CBN_Win_KeyCode_F1;
  case kVK_F2:           return CBN_Win_KeyCode_F2;
  case kVK_F3:           return CBN_Win_KeyCode_F3;
  case kVK_F4:           return CBN_Win_KeyCode_F4;
  case kVK_F5:           return CBN_Win_KeyCode_F5;
  case kVK_F6:           return CBN_Win_KeyCode_F6;
  case kVK_F7:           return CBN_Win_KeyCode_F7;
  case kVK_F8:           return CBN_Win_KeyCode_F8;
  case kVK_F9:           return CBN_Win_KeyCode_F9;
  case kVK_F10:          return CBN_Win_KeyCode_F10;
  case kVK_F11:          return CBN_Win_KeyCode_F11;
  case kVK_F12:          return CBN_Win_KeyCode_F12;
  case kVK_Escape:       return CBN_Win_KeyCode_Escape;
  case kVK_Tab:          return CBN_Win_KeyCode_Tab;
  case kVK_CapsLock:     return CBN_Win_KeyCode_CapsLock;
  case kVK_Shift:        return CBN_Win_KeyCode_LeftShift;
  case kVK_Control:      return CBN_Win_KeyCode_LeftCtrl;
  case kVK_Command:      return CBN_Win_KeyCode_LeftMeta;
  case kVK_Option:       return CBN_Win_KeyCode_LeftAlt;
  case kVK_Space:        return CBN_Win_KeyCode_Space;
  case kVK_RightOption:  return CBN_Win_KeyCode_RightAlt;
  case kVK_RightControl: return CBN_Win_KeyCode_RightCtrl;
  case kVK_RightShift:   return CBN_Win_KeyCode_RightShift;
  case kVK_Return:       return CBN_Win_KeyCode_Return;
  case kVK_UpArrow:      return CBN_Win_KeyCode_UpArrow;
  case kVK_DownArrow:    return CBN_Win_KeyCode_DownArrow;
  case kVK_LeftArrow:    return CBN_Win_KeyCode_LeftArrow;
  case kVK_RightArrow:   return CBN_Win_KeyCode_RightArrow;
    // `CBN_Win_KeyCode_RightMeta` not supported
  default:
    CARBON_UNREACHABLE;
    return CBN_Win_KeyCode_Count;
  }
}
