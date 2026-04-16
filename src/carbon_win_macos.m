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
- (void)mouseEntered:(NSEvent *)e {
  if (!carbon_win__cursor_visible) [NSCursor hide];
}

- (void)mouseExited:(NSEvent *)e {
  if (!carbon_win__cursor_visible) [NSCursor unhide];
}
@end

@interface CBNDelegate : NSObject<NSWindowDelegate>
@end

@implementation CBNDelegate
- (BOOL)windowShouldClose:(NSWindow *)win {
  carbon_win__should_close = true;
  return NO;
}
@end

static NSApplication *carbon_win__app;
static NSWindow *carbon_win__window;
static CBNDelegate *carbon_win__delegate;

static usz carbon_win__renderer_w, carbon_win__renderer_h;

static id<MTLDevice> carbon_win__mtl_device;
static id<MTLCommandQueue> carbon_win__mtl_queue;
static CAMetalLayer *carbon_win__mtl_layer;
static id<MTLRenderPipelineState> carbon_win__mtl_pipeline;
static id<MTLTexture> carbon_win__mtl_texture;
static id<MTLBuffer> carbon_win__mtl_buffer;

CBN_Vec2 carbon_win_get_mouse_position(void) {
  NSPoint m = [carbon_win__window mouseLocationOutsideOfEventStream];
  NSSize s = [carbon_win__window contentView].frame.size;
  m.y = s.height - m.y;  // +y must go downwards... Apple, what's wrong with y'all?
  f32 sf_w = (f32)carbon_win__renderer_w/s.width, sf_h = (f32)carbon_win__renderer_h/s.height;  
  usz x = carbon_math_clamp(m.x * sf_w, 0, carbon_win__renderer_w - 1);
  usz y = carbon_math_clamp(m.y * sf_h, 0, carbon_win__renderer_h - 1);
  return carbon_math_vec2(x, y);
}

void carbon_win_set_mouse_visibility(bool visible) {
  carbon_win__cursor_visible = visible;
}

void carbon_win_set_border_visibility(bool visible) {
  if (visible) carbon_win__window.styleMask |=  NSWindowStyleMaskTitled;
  else         carbon_win__window.styleMask &= ~NSWindowStyleMaskTitled;
}

void carbon_win_set_fullscreen(bool yn) {
  if (yn != !!(carbon_win__window.styleMask & NSWindowStyleMaskFullScreen)) {
    [carbon_win__window toggleFullScreen:nil];
  }
}

CBNINL void carbon_win__renderer_upload_texture(void) {
  if (carbon_win__mtl_buffer)  [carbon_win__mtl_buffer  release];
  if (carbon_win__mtl_texture) [carbon_win__mtl_texture release];
  NSUInteger bytes_per_row = carbon_win__renderer_w * 4;
  NSUInteger total_bytes   = carbon_win__renderer_h * bytes_per_row;
  carbon_win__mtl_buffer = [carbon_win__mtl_device newBufferWithLength:total_bytes
                                                               options:MTLResourceStorageModeShared];
  MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                  width:carbon_win__renderer_w
                                                                                 height:carbon_win__renderer_h
                                                                              mipmapped:NO];
  desc.usage       = MTLTextureUsageShaderRead;
  desc.storageMode = MTLStorageModeShared;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  desc.swizzle = MTLTextureSwizzleChannelsMake(MTLTextureSwizzleAlpha,
                                               MTLTextureSwizzleBlue,
                                               MTLTextureSwizzleGreen,
                                               MTLTextureSwizzleRed);
#endif
  carbon_win__mtl_texture = [carbon_win__mtl_buffer newTextureWithDescriptor:desc
                                                                      offset:0
                                                                 bytesPerRow:bytes_per_row];
}

CBNINL void carbon_win__create_system_menu(const char *title) {
  NSMenuItem *main_item = [NSMenuItem new];
  {
    NSMenu *menu = [NSMenu new];
    [menu addItem:main_item];
    [carbon_win__app setMainMenu:menu];
  }
  {
    NSMenu *menu = [[NSMenu alloc] initWithTitle:[NSString stringWithUTF8String:title]];
    NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:@"Quit"
                                                  action:@selector(terminate:)
                                           keyEquivalent:@"q"];
    [menu addItem:item];
    [main_item setSubmenu:menu];
  }
}

CBNINL void carbon_win__renderer_init(usz w, usz h) {
  carbon_win__renderer_w = w;
  carbon_win__renderer_h = h;
  {// Device, Queue and Layer
    NSView *view = [carbon_win__window contentView];
    carbon_win__mtl_device = MTLCreateSystemDefaultDevice();
    carbon_win__mtl_queue  = [carbon_win__mtl_device newCommandQueue];
    carbon_win__mtl_layer  = [CAMetalLayer layer];
    carbon_win__mtl_layer.device          = carbon_win__mtl_device;
    carbon_win__mtl_layer.pixelFormat     = MTLPixelFormatBGRA8Unorm;
    carbon_win__mtl_layer.framebufferOnly = NO;
    view.wantsLayer = YES;
    view.layer      = carbon_win__mtl_layer;
  }
  id<MTLFunction> vert_fn, frag_fn;
  {// MetalLib
    dispatch_data_t data = dispatch_data_create(carbon_win_shader_metallib,
                                                carbon_win_shader_metallib_len,
                                                0,
                                                DISPATCH_DATA_DESTRUCTOR_DEFAULT);
    NSError *err = nil;
    id<MTLLibrary> lib = [carbon_win__mtl_device newLibraryWithData:data error:&err];
    dispatch_release(data);
    NSCAssert(lib, @"Failed to load metallib: %@", err);
    vert_fn = [lib newFunctionWithName:@"vert"];
    frag_fn = [lib newFunctionWithName:@"frag"];
    [lib release];
  }
  {// Pipeline
    MTLRenderPipelineDescriptor *desc = [MTLRenderPipelineDescriptor new];
    desc.vertexFunction = vert_fn;
    desc.fragmentFunction = frag_fn;
    desc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    NSError *err = nil;
    carbon_win__mtl_pipeline = [carbon_win__mtl_device newRenderPipelineStateWithDescriptor:desc error:&err];
    NSCAssert(carbon_win__mtl_pipeline, @"Failed to create pipeline: %@", err);
    [desc release];
  }
  [vert_fn release];
  [frag_fn release];
  carbon_win__renderer_upload_texture();
}

CBNINL void carbon_win__create_window(usz w, usz h, const char *title) {
  carbon_win__app = [NSApplication sharedApplication];
  [carbon_win__app setActivationPolicy:NSApplicationActivationPolicyRegular];
  carbon_win__window = [[NSWindow alloc]
                         initWithContentRect:NSMakeRect(0, 0, w, h)
                                   styleMask:NSWindowStyleMaskTitled
                         | NSWindowStyleMaskClosable
                         | NSWindowStyleMaskResizable
                                     backing:NSBackingStoreBuffered
                                       defer:NO];
  {// View creation
    CBNView *view = [[CBNView alloc] initWithFrame:NSMakeRect(0, 0, w, h)];
    NSTrackingArea *tracking = [[NSTrackingArea alloc]
                                 initWithRect:[view bounds]
                                      options:NSTrackingMouseEnteredAndExited
                                 | NSTrackingActiveInKeyWindow
                                 | NSTrackingInVisibleRect
                                        owner:view
                                     userInfo:nil];
    [view addTrackingArea:tracking];
    [carbon_win__window setContentView:view];
  }
  [carbon_win__window setTitle:[NSString stringWithUTF8String:title]];
  [carbon_win__window center];
  carbon_win__create_system_menu(title);
  [carbon_win__window makeKeyAndOrderFront:nil];
  [carbon_win__app activateIgnoringOtherApps:YES];
  carbon_win__delegate = [CBNDelegate new];
  [carbon_win__window setDelegate:carbon_win__delegate];
  carbon_win__renderer_init(w, h);
}

CBNINL void carbon_win__renderer_shutdown(void) {
  [carbon_win__mtl_buffer   release];
  [carbon_win__mtl_texture  release];
  [carbon_win__mtl_pipeline release];
  [carbon_win__mtl_layer    release];
  [carbon_win__mtl_queue    release];
  [carbon_win__mtl_device   release];
}

CBNINL void carbon_win__destroy_window(void) {
  carbon_win__renderer_shutdown();
  [carbon_win__window setDelegate:nil];
  [carbon_win__delegate release];
  [carbon_win__window close];
  carbon_win__delegate = nil;
  carbon_win__window = nil;
  carbon_win__app = nil;
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
    // `CBN_Win_KeyCode_RightMeta` not supported
  case kVK_Return:       return CBN_Win_KeyCode_Return;
  case kVK_UpArrow:      return CBN_Win_KeyCode_UpArrow;
  case kVK_DownArrow:    return CBN_Win_KeyCode_DownArrow;
  case kVK_LeftArrow:    return CBN_Win_KeyCode_LeftArrow;
  case kVK_RightArrow:   return CBN_Win_KeyCode_RightArrow;
  default:               return CBN_Win_KeyCode_Count;
  }
}

CBNINL bool carbon_win__poll_event(void) {
  NSEvent *e = [carbon_win__app nextEventMatchingMask:NSEventMaskAny
                                            untilDate:[NSDate distantPast]
                                               inMode:NSDefaultRunLoopMode
                                              dequeue:YES];
  if (!e) return false;
  switch ([e type]) {
  case NSEventTypeKeyUp:
  case NSEventTypeKeyDown: {
    u16 s = [e keyCode];
    CBN_Win_KeyCode k = carbon_win__map_key_code(s);
    if (k != CBN_Win_KeyCode_Count) {
      carbon_win__keys[k] = [e type] == NSEventTypeKeyDown;
    }
  } break;
  case NSEventTypeFlagsChanged: {
    NSEventModifierFlags f = [e modifierFlags];
    carbon_win__keys[CBN_Win_KeyCode_LeftCtrl]  = !!(f & NSEventModifierFlagControl);
    carbon_win__keys[CBN_Win_KeyCode_LeftShift] = !!(f & NSEventModifierFlagShift);
    carbon_win__keys[CBN_Win_KeyCode_LeftAlt]   = !!(f & NSEventModifierFlagOption);
    carbon_win__keys[CBN_Win_KeyCode_LeftMeta]  = !!(f & NSEventModifierFlagCommand);
    carbon_win__keys[CBN_Win_KeyCode_CapsLock]  = !!(f & NSEventModifierFlagCapsLock);
  } break;
  case NSEventTypeLeftMouseUp:
  case NSEventTypeLeftMouseDown:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Left] = [e type] == NSEventTypeLeftMouseDown;
    break;
  case NSEventTypeRightMouseUp:
  case NSEventTypeRightMouseDown:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Right] = [e type] == NSEventTypeRightMouseDown;
    break;
  case NSEventTypeOtherMouseUp:
  case NSEventTypeOtherMouseDown:
    if ([e buttonNumber] == 2) {
      carbon_win__mouse_buttons[CBN_Win_MouseButton_Middle] = [e type] == NSEventTypeOtherMouseDown;
    }
    break;
  default: break;
  }
  [carbon_win__app sendEvent:e];
  return true;
}

CBNINL void carbon_win__renderer_present(const u32 *pixels, usz w, usz h) {
  if (w != carbon_win__renderer_w || h != carbon_win__renderer_h) {
    carbon_win__renderer_w = w;
    carbon_win__renderer_h = h;
    carbon_win__renderer_upload_texture();
  }
  carbon_memory_copy(carbon_win__mtl_buffer.contents, pixels, carbon_win__renderer_w * carbon_win__renderer_h * 4);
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
