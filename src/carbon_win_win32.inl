// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#include <windowsx.h>
#include <GL/gl.h>

#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB    0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB    0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB     0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

typedef HGLRC(WINAPI *wglCreateContextAttribsARB_t)(HDC, HGLRC, const int*);

static HWND carbon_win__hwnd;
static HDC carbon_win__hdc;
static HGLRC carbon_win__hglrc;

#include "carbon_win_gl.inl"

CBNINL void *carbon_win__gl_func_loader(const char *name) {
  return wglGetProcAddress(name);
}

CBNINL CBN_Vec2 carbon_win__get_window_size(void) {
  RECT r;
  GetClientRect(carbon_win__hwnd, &r);
  return carbon_math_vec2(r.right - r.left, r.bottom - r.top);
}

CBN_Vec2 carbon_win_get_mouse_position(void) {
  POINT p;
  GetCursorPos(&p);
  ScreenToClient(carbon_win__hwnd, &p);
  CBN_Vec2 s = carbon_win__get_window_size();
  f32 sf_w = (f32)carbon_win__renderer_w/s.x, sf_h = (f32)carbon_win__renderer_h/s.y;
  usz x = carbon_math_clamp(p.x * sf_w, 0, carbon_win__renderer_w - 1);
  usz y = carbon_math_clamp(p.y * sf_h, 0, carbon_win__renderer_h - 1);
  return carbon_math_vec2(x, y);
}

CBNINL void carbon_win__renderer_init(usz w, usz h) {
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), 1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0,
    PFD_MAIN_PLANE, 0, 0, 0, 0
  };
  int pf = ChoosePixelFormat(carbon_win__hdc, &pfd);
  SetPixelFormat(carbon_win__hdc, pf, &pfd);
  HGLRC dummy = wglCreateContext(carbon_win__hdc);
  wglMakeCurrent(carbon_win__hdc, dummy);
  wglCreateContextAttribsARB_t wglCreateContextAttribsARB = (wglCreateContextAttribsARB_t)wglGetProcAddress("wglCreateContextAttribsARB");
  CBN_ASSERT(wglCreateContextAttribsARB && "WGL_ARB_create_context extension not supported");
  int attrs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };
  carbon_win__hglrc = wglCreateContextAttribsARB(carbon_win__hdc, 0, attrs);
  if (!carbon_win__hglrc) {
    CBN_ERROR("Win32 error code: %lu", GetLastError());
  } CBN_ASSERT(carbon_win__hglrc && "wglCreateContextAttribsARB failed");
  wglMakeCurrent(carbon_win__hdc, 0);
  wglDeleteContext(dummy);
  wglMakeCurrent(carbon_win__hdc, carbon_win__hglrc);
  carbon_win__gl_init(w, h);
}

CBNINL CBN_Win_KeyCode carbon_win__map_key_code(WPARAM key) {
  if (key >=   'A' && key <=    'Z') return CBN_Win_KeyCode_A    + (key -   'A');
  if (key >=   '0' && key <=    '9') return CBN_Win_KeyCode_Zero + (key -   '0');
  if (key >= VK_F1 && key <= VK_F12) return CBN_Win_KeyCode_F1   + (key - VK_F1);
  switch (key) {
  case VK_OEM_3:    return CBN_Win_KeyCode_BackQuote;
  case VK_ESCAPE:   return CBN_Win_KeyCode_Escape;
  case VK_TAB:      return CBN_Win_KeyCode_Tab;
  case VK_CAPITAL:  return CBN_Win_KeyCode_CapsLock;
  case VK_LSHIFT:   return CBN_Win_KeyCode_LeftShift;
  case VK_LCONTROL: return CBN_Win_KeyCode_LeftCtrl;
  case VK_LWIN:     return CBN_Win_KeyCode_LeftMeta;
  case VK_LMENU:    return CBN_Win_KeyCode_LeftAlt;
  case VK_SPACE:    return CBN_Win_KeyCode_Space;
  case VK_RMENU:    return CBN_Win_KeyCode_RightAlt;
  case VK_RWIN:     return CBN_Win_KeyCode_RightMeta;
  case VK_RCONTROL: return CBN_Win_KeyCode_RightCtrl;
  case VK_RSHIFT:   return CBN_Win_KeyCode_RightShift;
  case VK_RETURN:   return CBN_Win_KeyCode_Return;
  case VK_UP:       return CBN_Win_KeyCode_UpArrow;
  case VK_DOWN:     return CBN_Win_KeyCode_DownArrow;
  case VK_LEFT:     return CBN_Win_KeyCode_LeftArrow;
  case VK_RIGHT:    return CBN_Win_KeyCode_RightArrow;
  default:          return CBN_Win_KeyCode_Count;
  }
}

LRESULT CALLBACK carbon_win__wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
  case WM_CLOSE:
    carbon_win__should_close = true;
    return 0;
  case WM_SIZE:
    if (carbon_win__hglrc) glViewport(0, 0, LOWORD(lp), HIWORD(lp));
    return 0;
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN: {
    CBN_Win_KeyCode k = carbon_win__map_key_code(wp);
    if (k != CBN_Win_KeyCode_Count) carbon_win__keys[k] = true;
    return 0;
  }
  case WM_KEYUP:
  case WM_SYSKEYUP: {
    CBN_Win_KeyCode k = carbon_win__map_key_code(wp);
    if (k != CBN_Win_KeyCode_Count) carbon_win__keys[k] = false;
    return 0;
  }
  case WM_LBUTTONDOWN:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Left] = true;
    return 0;
  case WM_LBUTTONUP:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Left] = false;
    return 0;
  case WM_RBUTTONDOWN:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Right] = true;
    return 0;
  case WM_RBUTTONUP:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Right] = false;
    return 0;
  case WM_MBUTTONDOWN:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Middle] = true;
    return 0;
  case WM_MBUTTONUP:
    carbon_win__mouse_buttons[CBN_Win_MouseButton_Middle] = false;
    return 0;
  case WM_MOUSEWHEEL:
    carbon_win__mouse_scroll += (f32)GET_WHEEL_DELTA_WPARAM(wp)/WHEEL_DELTA;
    return 0;
  }
  return DefWindowProcA(hwnd, msg, wp, lp);
}

CBNINL void carbon_win__create_window(usz w, usz h, const char *title) {
  HINSTANCE hinst = GetModuleHandleA(0);
  WNDCLASSEXA wc = {
    .cbSize = sizeof(WNDCLASSEXA),
    .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc = carbon_win__wndproc,
    .hInstance = hinst,
    .hCursor = LoadCursor(NULL, IDC_ARROW),
    .lpszClassName = "CarbonWinClass"
  };
  RegisterClassExA(&wc);
  DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
  RECT rect = {0, 0, w, h};
  AdjustWindowRect(&rect, style, FALSE);
  carbon_win__hwnd = CreateWindowExA(0, wc.lpszClassName, title, style,
                                     CW_USEDEFAULT, CW_USEDEFAULT,
                                     rect.right - rect.left, rect.bottom - rect.top,
                                     0, 0, hinst, 0);
  CBN_ASSERT(carbon_win__hwnd && "CreateWindowExA failed");
  carbon_win__hdc = GetDC(carbon_win__hwnd);
  carbon_win__renderer_init(w, h);
}

CBNINL void carbon_win__renderer_shutdown(void) {
  wglMakeCurrent(0, 0);
  if (carbon_win__hglrc) wglDeleteContext(carbon_win__hglrc);
  carbon_win__hglrc = 0;
}

CBNINL void carbon_win__destroy_window(void) {
  carbon_win__renderer_shutdown();
  if (carbon_win__hdc) ReleaseDC(carbon_win__hwnd, carbon_win__hdc);
  if (carbon_win__hwnd) DestroyWindow(carbon_win__hwnd);
  carbon_win__hdc = 0;
  carbon_win__hwnd = 0;
}

CBNINL void carbon_win__renderer_present(const u32 *pixels, usz w, usz h) {
  carbon_win__gl_render(pixels, w, h);
  SwapBuffers(carbon_win__hdc);
}

CBNINL bool carbon_win__poll_event(void) {
  MSG msg;
  if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) carbon_win__should_close = true;
    else {
      TranslateMessage(&msg);
      DispatchMessageA(&msg);
    }
    return true;
  }
  return false;
}

// Local Variables:
// mode: c
// End:
