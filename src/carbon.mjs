// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

function make_env(...envs) {
  return new Proxy(envs, {
    get(target, prop, receiver) {
      for (let env of envs) {
        if (env.hasOwnProperty(prop)) return env[prop];
      }
      return (...args) => { throw new Error(`NOT IMPLEMENTED: ${prop} (${args})`); };
    }
  });
}

function cstr_len(buffer, ptr) {
  const mem = new Uint8Array(buffer);
  let len = 0;
  while (mem[ptr]) {
    ++len;
    ++ptr;
  }
  return len;
}

function ptr_as_cstr(buffer, ptr) {
  const bytes = new Uint8Array(buffer, ptr, cstr_len(buffer, ptr));
  return new TextDecoder().decode(bytes);
}

function todo() { throw new Error("NOT IMPLEMENTED"); }

export async function run(wasm_file) {
  let max_fps = null;
  const wasm = await WebAssembly.instantiateStreaming(fetch(wasmFile), {
    env: make_env({
      // char *carbon_string_fmt(const char *s, ...);
      carbon_string_fmt: (s, ...args) => {
        todo();
      },

      // void carbon_log_print(FILE *stream, const char *fmt, ...);
      carbon_log_print: (stream, fmt, ...args) => {
        console.log(fmt);
        todo();
      },

      // void carbon_assert_abort(const char *expr, const char *file, u32 line, const char *func);
      carbon_assert_abort: (expr, file, line, func) => {
        todo();
      },

      // void carbon_win_open(u16 width, u16 height, const char *title);
      carbon_win_open: (width, height, title) => {
        // ctx.canvas.width = width;
        // ctx.canvas.height = height;
        // const buffer = wasm.exports.memory.buffer;
        // document.title = ptr_as_cstr(buffer, title);
        todo();
      },

      // void carbon_win_close(void);
      carbon_win_close: () => {},

      // u8 carbon_win_shouldclose(void);
      carbon_win_shouldclose: () => { return false; },

      // void carbon_win_set_max_fps(u32 fps);
      carbon_win_set_max_fps: (fps) => { max_fps = fps; },

      // f64 carbon_win_get_deltatime(void);
      carbon_win_get_deltatime: () => {
        todo();
      },

      // void carbon_win_update(CBN_DrawCanvas dc);
      carbon_win_update: (dc) => {
        todo();
      }
    })
  });
  wasm.instance.exports.main();
}
