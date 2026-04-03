// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

static Display *carbon_win__display;
static Window carbon_win__window;
static Atom carbon_win__wm_delete;
static GLXContext carbon_win__glx_ctx;

static i32 carbon_win__glx_attrs[] = {
  GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None
};

static GLuint carbon_win__gl_tex;
static GLuint carbon_win__gl_vao;

static usz carbon_win__renderer_w, carbon_win__renderer_h;

//////////// GL functions ////////////
#define CARBON_WIN__GL_PROCS                                            \
  x(GLuint, glCreateShader, GLenum)                                     \
  x(void, glShaderBinary, GLsizei, const GLuint *, GLenum, const void *, GLsizei) \
  x(void, glSpecializeShader, GLuint, const GLchar *, GLuint, const GLuint *, const GLuint *) \
  x(GLuint, glCreateProgram, void)                                      \
  x(void, glAttachShader, GLuint, GLuint)                               \
  x(void, glLinkProgram, GLuint)                                        \
  x(void, glUseProgram, GLuint)                                         \
  x(void, glGenVertexArrays, GLsizei, GLuint *)                         \
  x(void, glBindVertexArray, GLuint)                                    \
  x(void, glGetShaderiv, GLuint, GLenum, GLint *)                       \
  x(void, glGetProgramiv, GLuint, GLenum, GLint *)                      \
  x(void, glGetProgramInfoLog, GLuint, GLsizei, GLsizei *, GLchar *)

#define x(ret, name, ...)                       \
  typedef ret (*name ## _t)(__VA_ARGS__);       \
  static name ## _t name;
CARBON_WIN__GL_PROCS;
#undef x

CBNINL void carbon_win__renderer_gl_load_funcs(void) {
#define x(ret, name, ...)                                         \
  name = (name ## _t) glXGetProcAddress((const GLubyte *) #name); \
  CBN_ASSERT(name && #name " failed to load");
  CARBON_WIN__GL_PROCS;
#undef x
}
//////////////////////////////////////

CBN_Vec2 carbon_win_get_mouse_position(void) {
  // ...
  return carbon_math_vec2(0, 0);
}

void carbon_win_set_mouse_visibility(bool visible) {
  // ...
}

void carbon_win_set_border_visibility(bool visible) {
  // ...
}

void carbon_win_set_fullscreen(bool yn) {
  // ...
}

CBNINL void carbon_win__renderer_init(usz w, usz h) {
  carbon_win__renderer_w = w;
  carbon_win__renderer_h = h;
  {// GLX context
    i32 scr = DefaultScreen(carbon_win__display);
    XVisualInfo *vi = glXChooseVisual(carbon_win__display, scr, carbon_win__glx_attrs);
    CBN_ASSERT(vi && "glXChooseVisual failed");
    carbon_win__glx_ctx = glXCreateContext(carbon_win__display, vi, 0, GL_TRUE);
    CBN_ASSERT(carbon_win__glx_ctx && "glXCreateContext failed");
    glXMakeCurrent(carbon_win__display, carbon_win__window, carbon_win__glx_ctx);
    XFree(vi);
  }
  carbon_win__renderer_gl_load_funcs();
  {// GLSL shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER); {
      glShaderBinary(1, &vert,
                     GL_SHADER_BINARY_FORMAT_SPIR_V,
                     carbon_win_shader_vert_spv,
                     carbon_win_shader_vert_spv_len);
      glSpecializeShader(vert, "main", 0, 0, 0);
      GLint ok;
      glGetShaderiv(vert, GL_COMPILE_STATUS, &ok);
      CBN_ASSERT(ok && "vert specialization failed");
    }
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER); {
      glShaderBinary(1, &frag,
                     GL_SHADER_BINARY_FORMAT_SPIR_V,
                     carbon_win_shader_frag_spv,
                     carbon_win_shader_frag_spv_len);
      glSpecializeShader(frag, "main", 0, 0, 0);
      GLint ok;
      glGetShaderiv(frag, GL_COMPILE_STATUS, &ok);
      CBN_ASSERT(ok && "frag specialization failed");
    }
    GLuint prog = glCreateProgram(); {
      glAttachShader(prog, vert);
      glAttachShader(prog, frag);
      glLinkProgram(prog);
      GLint ok;
      glGetProgramiv(prog, GL_LINK_STATUS, &ok);
      if (!ok) {
        char log[512];
        glGetProgramInfoLog(prog, 512, 0, log);
        CBN_ERROR("prog link error: %s", log);
        CARBON_UNREACHABLE;
      }
    }
    glUseProgram(prog);
    glGenVertexArrays(1, &carbon_win__gl_vao);
    glBindVertexArray(carbon_win__gl_vao);
    glGenTextures(1, &carbon_win__gl_tex);
    glBindTexture(GL_TEXTURE_2D, carbon_win__gl_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 carbon_win__renderer_w,
                 carbon_win__renderer_h,
                 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 0);
  }
}

CBNINL void carbon_win__create_window(usz w, usz h, const char *title) {
  carbon_win__display = XOpenDisplay(0);
  CBN_ASSERT(carbon_win__display && "XOpenDisplay failed");
  {// Window w/GLX
    i32 scr = DefaultScreen(carbon_win__display);
    XVisualInfo *vi = glXChooseVisual(carbon_win__display, scr, carbon_win__glx_attrs);
    CBN_ASSERT(vi && "glXChooseVisual failed");
    Colormap cm = XCreateColormap(carbon_win__display,
                                  RootWindow(carbon_win__display, scr),
                                  vi->visual, AllocNone);
    XSetWindowAttributes swa = {0};
    swa.colormap = cm;
    swa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;
    carbon_win__window = XCreateWindow(carbon_win__display,
                                       RootWindow(carbon_win__display, scr),
                                       0, 0, w, h, 0,
                                       vi->depth,
                                       InputOutput,
                                       vi->visual,
                                       CWColormap | CWEventMask,
                                       &swa);
    XFree(vi);
  }
  XStoreName(carbon_win__display, carbon_win__window, title);
  carbon_win__wm_delete = XInternAtom(carbon_win__display, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(carbon_win__display, carbon_win__window, &carbon_win__wm_delete, 1);
  XMapWindow(carbon_win__display, carbon_win__window);
  carbon_win__renderer_init(w, h);
}

CBNINL void carbon_win__renderer_shutdown(void) {
  glXMakeCurrent(carbon_win__display, None, 0);
  glXDestroyContext(carbon_win__display, carbon_win__glx_ctx);
  carbon_win__glx_ctx = 0;
}

CBNINL void carbon_win__destroy_window(void) {
  carbon_win__renderer_shutdown();
  XDestroyWindow(carbon_win__display, carbon_win__window);
  XCloseDisplay(carbon_win__display);
  carbon_win__window = 0;
  carbon_win__display = 0;
}

CBNINL CBN_Win_KeyCode carbon_win__map_key_code(KeySym key) {
  // </usr/include/X11/keysymdef.h>
  // <https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h>
  switch (key) {
  case XK_a:         return CBN_Win_KeyCode_A;
  case XK_b:         return CBN_Win_KeyCode_B;
  case XK_c:         return CBN_Win_KeyCode_C;
  case XK_d:         return CBN_Win_KeyCode_D;
  case XK_e:         return CBN_Win_KeyCode_E;
  case XK_f:         return CBN_Win_KeyCode_F;
  case XK_g:         return CBN_Win_KeyCode_G;
  case XK_h:         return CBN_Win_KeyCode_H;
  case XK_i:         return CBN_Win_KeyCode_I;
  case XK_j:         return CBN_Win_KeyCode_J;
  case XK_k:         return CBN_Win_KeyCode_K;
  case XK_l:         return CBN_Win_KeyCode_L;
  case XK_m:         return CBN_Win_KeyCode_M;
  case XK_n:         return CBN_Win_KeyCode_N;
  case XK_o:         return CBN_Win_KeyCode_O;
  case XK_p:         return CBN_Win_KeyCode_P;
  case XK_q:         return CBN_Win_KeyCode_Q;
  case XK_r:         return CBN_Win_KeyCode_R;
  case XK_s:         return CBN_Win_KeyCode_S;
  case XK_t:         return CBN_Win_KeyCode_T;
  case XK_u:         return CBN_Win_KeyCode_U;
  case XK_v:         return CBN_Win_KeyCode_V;
  case XK_w:         return CBN_Win_KeyCode_W;
  case XK_x:         return CBN_Win_KeyCode_X;
  case XK_y:         return CBN_Win_KeyCode_Y;
  case XK_z:         return CBN_Win_KeyCode_Z;
  case XK_0:         return CBN_Win_KeyCode_Zero;
  case XK_1:         return CBN_Win_KeyCode_One;
  case XK_2:         return CBN_Win_KeyCode_Two;
  case XK_3:         return CBN_Win_KeyCode_Three;
  case XK_4:         return CBN_Win_KeyCode_Four;
  case XK_5:         return CBN_Win_KeyCode_Five;
  case XK_6:         return CBN_Win_KeyCode_Six;
  case XK_7:         return CBN_Win_KeyCode_Seven;
  case XK_8:         return CBN_Win_KeyCode_Eight;
  case XK_9:         return CBN_Win_KeyCode_Nine;
  case XK_grave:     return CBN_Win_KeyCode_BackQuote;
  case XK_F1:        return CBN_Win_KeyCode_F1;
  case XK_F2:        return CBN_Win_KeyCode_F2;
  case XK_F3:        return CBN_Win_KeyCode_F3;
  case XK_F4:        return CBN_Win_KeyCode_F4;
  case XK_F5:        return CBN_Win_KeyCode_F5;
  case XK_F6:        return CBN_Win_KeyCode_F6;
  case XK_F7:        return CBN_Win_KeyCode_F7;
  case XK_F8:        return CBN_Win_KeyCode_F8;
  case XK_F9:        return CBN_Win_KeyCode_F9;
  case XK_F10:       return CBN_Win_KeyCode_F10;
  case XK_F11:       return CBN_Win_KeyCode_F11;
  case XK_F12:       return CBN_Win_KeyCode_F12;
  case XK_Escape:    return CBN_Win_KeyCode_Escape;
  case XK_Tab:       return CBN_Win_KeyCode_Tab;
  case XK_Caps_Lock: return CBN_Win_KeyCode_CapsLock;
  case XK_Shift_L:   return CBN_Win_KeyCode_LeftShift;
  case XK_Control_L: return CBN_Win_KeyCode_LeftCtrl;
  case XK_Super_L:   return CBN_Win_KeyCode_LeftMeta;
  case XK_Alt_L:     return CBN_Win_KeyCode_LeftAlt;
  case XK_space:     return CBN_Win_KeyCode_Space;
  case XK_Alt_R:     return CBN_Win_KeyCode_RightAlt;
  case XK_Control_R: return CBN_Win_KeyCode_RightCtrl;
  case XK_Shift_R:   return CBN_Win_KeyCode_RightShift;
  case XK_Super_R:   return CBN_Win_KeyCode_RightMeta;
  case XK_Return:    return CBN_Win_KeyCode_Return;
  case XK_Up:        return CBN_Win_KeyCode_UpArrow;
  case XK_Down:      return CBN_Win_KeyCode_DownArrow;
  case XK_Left:      return CBN_Win_KeyCode_LeftArrow;
  case XK_Right:     return CBN_Win_KeyCode_RightArrow;
  default:           return CBN_Win_KeyCode_Count;
  }
}

CBNINL bool carbon_win__poll_event(void) {
  if (!XPending(carbon_win__display)) return false;
  XEvent e;
  XNextEvent(carbon_win__display, &e);
  switch (e.type) {
  case ClientMessage:
    if ((Atom)e.xclient.data.l[0] == carbon_win__wm_delete) {
      carbon_win__should_close = true;
    }
    break;
  case ConfigureNotify:
    glViewport(0, 0, e.xconfigure.width, e.xconfigure.height);
    break;
  case KeyRelease:
  case KeyPress: {
    KeySym s = XLookupKeysym(&e.xkey, 0);
    CBN_Win_KeyCode k = carbon_win__map_key_code(s);
    if (k != CBN_Win_KeyCode_Count) {
      carbon_win__keys[k] = e.type == KeyPress;
    }
  } break;
  case ButtonRelease:
  case ButtonPress: {
    bool pressed = e.type == ButtonPress;
    switch (e.xbutton.button) {
    case Button1:
      carbon_win__mouse_buttons[CBN_Win_MouseButton_Left] = pressed;
      break;
    case Button2:
      carbon_win__mouse_buttons[CBN_Win_MouseButton_Middle] = pressed;
      break;
    case Button3:
      carbon_win__mouse_buttons[CBN_Win_MouseButton_Right] = pressed;
      break;
    default: break;
    }
  } break;
  default: break;
  }
  return true;
}

CBNINL void carbon_win__renderer_present(const u32 *pixels, usz w, usz h) {
  if (w != carbon_win__renderer_w || h != carbon_win__renderer_h) {
    carbon_win__renderer_w = w;
    carbon_win__renderer_h = h;
    glViewport(0, 0, carbon_win__renderer_w, carbon_win__renderer_h);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 carbon_win__renderer_w,
                 carbon_win__renderer_h,
                 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 0);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, carbon_win__gl_tex);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                  carbon_win__renderer_w,
                  carbon_win__renderer_h,
                  GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                  pixels);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(carbon_win__gl_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glXSwapBuffers(carbon_win__display, carbon_win__window);
}

// Local Variables:
// mode: c
// End:
