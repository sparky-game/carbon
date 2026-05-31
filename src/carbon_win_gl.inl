// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER               0x8B31
#define GL_FRAGMENT_SHADER             0x8B30
#define GL_COMPILE_STATUS              0x8B81
#define GL_LINK_STATUS                 0x8B82
#define GL_SHADER_BINARY_FORMAT_SPIR_V 0x9551
#define GL_UNSIGNED_INT_8_8_8_8        0x8035
#define GL_TEXTURE0                    0x84C0
#endif

typedef char GLchar;

static GLuint carbon_win__gl_tex;
static GLuint carbon_win__gl_vao;

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
  x(void, glGetProgramInfoLog, GLuint, GLsizei, GLsizei *, GLchar *)    \
  x(void, glActiveTexture, GLenum)

#define x(ret, name, ...)                       \
  typedef ret (*name ## _t)(__VA_ARGS__);       \
  static name ## _t name;
CARBON_WIN__GL_PROCS;
#undef x

CBNINL void *carbon_win__gl_func_loader(const char *name);

CBNINL void carbon_win__gl_load_funcs(void) {
#define x(ret, name, ...)                                 \
  name = (name ## _t) carbon_win__gl_func_loader(#name);  \
  CBN_ASSERT(name && #name " failed to load");
  CARBON_WIN__GL_PROCS;
#undef x
}

CBNINL void carbon_win__gl_init(usz w, usz h) {
  carbon_win__renderer_w = w;
  carbon_win__renderer_h = h;
  carbon_win__gl_load_funcs();
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
               carbon_win__renderer_w,
               carbon_win__renderer_h,
               0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 0);
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
}

// Local Variables:
// mode: c
// End:
