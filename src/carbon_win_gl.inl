// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#include <GL/gl.h>

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER        0x8B31
#define GL_FRAGMENT_SHADER      0x8B30
#define GL_COMPILE_STATUS       0x8B81
#define GL_LINK_STATUS          0x8B82
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_TEXTURE0             0x84C0
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_STREAM_DRAW         0x88E0
#define GL_WRITE_ONLY          0x88B9
typedef isz GLsizeiptr;
#endif
#ifndef GL_UNIFORM_BUFFER
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_DYNAMIC_DRAW   0x88E8
typedef isz GLintptr;
#endif
#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER      0x812D
#define GL_TEXTURE_BORDER_COLOR 0x1004
#endif

#define CARBON_WIN__GL_PBO_COUNT 2

typedef char GLchar;

#define CARBON_WIN__GL_PROCS                                            \
  x(GLuint, glCreateShader, GLenum)                                     \
  x(void, glShaderSource, GLuint, GLsizei, const GLchar **, const GLint *) \
  x(void, glCompileShader, GLuint)                                      \
  x(GLuint, glCreateProgram, void)                                      \
  x(void, glAttachShader, GLuint, GLuint)                               \
  x(void, glLinkProgram, GLuint)                                        \
  x(void, glUseProgram, GLuint)                                         \
  x(void, glGenVertexArrays, GLsizei, GLuint *)                         \
  x(void, glBindVertexArray, GLuint)                                    \
  x(void, glGetShaderiv, GLuint, GLenum, GLint *)                       \
  x(void, glGetProgramiv, GLuint, GLenum, GLint *)                      \
  x(void, glGetShaderInfoLog, GLuint, GLsizei, GLsizei *, GLchar *)     \
  x(void, glGetProgramInfoLog, GLuint, GLsizei, GLsizei *, GLchar *)    \
  x(void, glGenBuffers, GLsizei, GLuint *)                              \
  x(void, glBindBuffer, GLenum, GLuint)                                 \
  x(void, glBufferData, GLenum, GLsizeiptr, const void *, GLenum)       \
  x(void *, glMapBuffer, GLenum, GLenum)                                \
  x(GLboolean, glUnmapBuffer, GLenum)                                   \
  x(void, glBufferSubData, GLenum, GLintptr, GLsizeiptr, const void *)  \
  x(void, glBindBufferBase, GLenum, GLuint, GLuint)                     \
  x(GLuint, glGetUniformBlockIndex, GLuint, const GLchar *)             \
  x(void, glUniformBlockBinding, GLuint, GLuint, GLuint)

#define x(ret, name, ...)                       \
  typedef ret (*name ## _t)(__VA_ARGS__);       \
  static name ## _t name;
CARBON_WIN__GL_PROCS;
#ifdef _WIN32
x(void, glActiveTexture, GLenum);
#endif
#undef x

CBNINL void *carbon_win__gl_func_loader(const char *name);

static GLuint carbon_win__gl_tex;
static GLuint carbon_win__gl_vao;
static GLuint carbon_win__gl_pbo[CARBON_WIN__GL_PBO_COUNT];
static usz carbon_win__gl_pbo_idx;
static GLuint carbon_win__gl_ubo_postfx;

CBNINL void carbon_win__gl_load_funcs(void) {
#define x(ret, name, ...)                                 \
  name = (name ## _t) carbon_win__gl_func_loader(#name);  \
  CBN_ASSERT(name && #name " failed to load");
  CARBON_WIN__GL_PROCS;
#ifdef _WIN32
  x(void, glActiveTexture, GLenum);
#endif
#undef x
}

CBNINL void carbon_win__gl_init(usz w, usz h) {
  carbon_win__renderer_w = w;
  carbon_win__renderer_h = h;
  carbon_win__gl_load_funcs();
  GLuint vert = glCreateShader(GL_VERTEX_SHADER); {
    const GLchar *src = (const GLchar *)__src_carbon_win_shader_vert_glsl;
    const GLint len = (GLint)__src_carbon_win_shader_vert_glsl_len;
    glShaderSource(vert, 1, &src, &len);
    glCompileShader(vert);
    GLint ok;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &ok);
    if (!ok) {
      char log[512];
      glGetShaderInfoLog(vert, 512, 0, log);
      CBN_ERROR("vert compile error: %s", log);
      CARBON_UNREACHABLE;
    }
  }
  GLuint frag = glCreateShader(GL_FRAGMENT_SHADER); {
    const GLchar *src = (const GLchar *)__src_carbon_win_shader_frag_glsl;
    const GLint len = (GLint)__src_carbon_win_shader_frag_glsl_len;
    glShaderSource(frag, 1, &src, &len);
    glCompileShader(frag);
    GLint ok;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &ok);
    if (!ok) {
      char log[512];
      glGetShaderInfoLog(frag, 512, 0, log);
      CBN_ERROR("frag compile error: %s", log);
      CARBON_UNREACHABLE;
    }
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
  } glUseProgram(prog);
  {// VAO
    glGenVertexArrays(1, &carbon_win__gl_vao);
    glBindVertexArray(carbon_win__gl_vao);
  }
  {// Texture
    glGenTextures(1, &carbon_win__gl_tex);
    glBindTexture(GL_TEXTURE_2D, carbon_win__gl_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const f32 c[] = {0, 0, 0, 1};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, c);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 carbon_win__renderer_w,
                 carbon_win__renderer_h,
                 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 0);
  }
  {// PBO
    glGenBuffers(CARBON_WIN__GL_PBO_COUNT, carbon_win__gl_pbo);
    for (usz i = 0; i < CARBON_WIN__GL_PBO_COUNT; ++i) {
      glBindBuffer(GL_PIXEL_UNPACK_BUFFER, carbon_win__gl_pbo[i]);
      glBufferData(GL_PIXEL_UNPACK_BUFFER, w*h*4, 0, GL_STREAM_DRAW);
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }
  {// UBO
    glUniformBlockBinding(prog, glGetUniformBlockIndex(prog, "PostFX"), 0);
    glGenBuffers(1, &carbon_win__gl_ubo_postfx);
    glBindBuffer(GL_UNIFORM_BUFFER, carbon_win__gl_ubo_postfx);
    glBufferData(GL_UNIFORM_BUFFER, 4*4*sizeof(f32), 0, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, carbon_win__gl_ubo_postfx);
  }
}

CBNINL void carbon_win__gl_render(const u32 *pixels, usz w, usz h) {
  if (w != carbon_win__renderer_w || h != carbon_win__renderer_h) {
    carbon_win__renderer_w = w;
    carbon_win__renderer_h = h;
    glViewport(0, 0, carbon_win__renderer_w, carbon_win__renderer_h);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 carbon_win__renderer_w,
                 carbon_win__renderer_h,
                 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 0);
    for (usz i = 0; i < CARBON_WIN__GL_PBO_COUNT; ++i) {
      glBindBuffer(GL_PIXEL_UNPACK_BUFFER, carbon_win__gl_pbo[i]);
      glBufferData(GL_PIXEL_UNPACK_BUFFER, w*h*4, 0, GL_STREAM_DRAW);
    }
  }
  const usz idx = carbon_win__gl_pbo_idx;
  const usz next = (idx + 1) % CARBON_WIN__GL_PBO_COUNT;
  carbon_win__gl_pbo_idx = next;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, carbon_win__gl_tex);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, carbon_win__gl_pbo[idx]);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                  carbon_win__renderer_w,
                  carbon_win__renderer_h,
                  GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                  0);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, carbon_win__gl_pbo[next]);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, carbon_win__renderer_w * carbon_win__renderer_h * 4, 0, GL_STREAM_DRAW);
  void *p = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  if (p) {
    carbon_memory_copy(p, pixels, carbon_win__renderer_w * carbon_win__renderer_h * 4);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  }
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  const f32 fx[4*4] = {
    carbon_win__postfx.barrel_distortion.yn,
    carbon_win__postfx.barrel_distortion.intensity,
    0, 0,
    carbon_win__postfx.chromatic_aberration.yn,
    carbon_win__postfx.chromatic_aberration.intensity,
    carbon_win__postfx.chromatic_aberration.edge_fade,
    0,
    carbon_win__postfx.scanlines.yn,
    carbon_win__postfx.scanlines.density,
    carbon_win__postfx.scanlines.opacity,
    0,
    carbon_win__postfx.vignette.yn,
    carbon_win__postfx.vignette.radius,
    carbon_win__postfx.vignette.smoothness,
    carbon_win__postfx.vignette.intensity
  };
  glBindBuffer(GL_UNIFORM_BUFFER, carbon_win__gl_ubo_postfx);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(fx), fx);
  glBindVertexArray(carbon_win__gl_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Local Variables:
// mode: c
// End:
