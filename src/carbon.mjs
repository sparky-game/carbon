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
  const bytes = new Uint8Array(buffer);
  let len = 0;
  while (bytes[ptr]) {
    ++len;
    ++ptr;
  }
  return len;
}

function ptr_as_cstr(buffer, ptr) {
  const bytes = new Uint8Array(buffer, ptr, cstr_len(buffer, ptr));
  return new TextDecoder().decode(bytes);
}

function ptr_as_drawcanvas(buffer, ptr) {
  /*
    u32 *pixels; -> +0
    usz width;   -> +4
    usz height;  -> +8
    usz stride;  -> +12
  */
  const view = new DataView(buffer);
  const pixels_ptr = view.getUint32(ptr + 0, true);
  const width  = view.getUint32(ptr + 4, true);
  const height = view.getUint32(ptr + 8, true);
  const stride = view.getUint32(ptr + 12, true);
  const pixels = new Uint32Array(buffer, pixels_ptr, width * height);
  return { pixels, width, height, stride };
}

function todo() { throw new Error("NOT IMPLEMENTED"); }

export let wasm = null;
let max_fps = 60;

export async function load(wasm_file) {
  let max_fps = null;
  wasm = await WebAssembly.instantiateStreaming(fetch(wasm_file), {
    env: make_env({
      // char *carbon_string_fmt(const char *s, ...);
      carbon_string_fmt: (s, ...args) => {
        console.debug(s);  // This is ptr in WASM memory where the string is located
        const buffer = wasm.instance.exports.memory.buffer;
        console.debug(ptr_as_cstr(buffer, s));  // This is the string itself
        for (const arg of args) {
          console.debug(arg);
          console.debug(ptr_as_cstr(buffer, arg));
        }
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

      carbon_win_open: carbon_win_open,
      carbon_win_close: carbon_win_close,
      carbon_win_shouldclose: carbon_win_shouldclose,
      carbon_win_set_max_fps: carbon_win_set_max_fps,
      carbon_win_get_deltatime: carbon_win_get_deltatime,
      carbon_win_update: carbon_win_update
    })
  });
}

// void carbon_win_open(u16 width, u16 height, const char *title);
function carbon_win_open(width, height, title) {
  const ctx = document.getElementById("app").getContext("2d");
  const buffer = wasm.instance.exports.memory.buffer;
  ctx.canvas.width = width;
  ctx.canvas.height = height;
  document.title = ptr_as_cstr(buffer, title);
}

// void carbon_win_close(void);
function carbon_win_close() {}

// u8 carbon_win_shouldclose(void);
function carbon_win_shouldclose() { return false; }

// void carbon_win_set_max_fps(u32 fps);
function carbon_win_set_max_fps(fps) { max_fps = fps; }

// f64 carbon_win_get_deltatime(void);
function carbon_win_get_deltatime() { return 1.0 / max_fps; }

// void carbon_win_update(CBN_DrawCanvas dc);
function carbon_win_update(dc) {
  const ctx = document.getElementById("app").getContext("2d");
  const buffer = wasm.instance.exports.memory.buffer;
  const dc_obj = ptr_as_drawcanvas(buffer, dc);  
  const data = new Uint8ClampedArray(dc_obj.width * dc_obj.height * 4);
  for (let i = 0; i < dc_obj.pixels.length; ++i) {
    const color = dc_obj.pixels[i];
    const r = (color >> 24) & 0xff;
    const g = (color >> 16) & 0xff;
    const b = (color >> 8)  & 0xff;
    const a = (color >> 0)  & 0xff;
    data[i * 4 + 0] = r;
    data[i * 4 + 1] = g;
    data[i * 4 + 2] = b;
    data[i * 4 + 3] = a;
  }
  ctx.putImageData(new ImageData(data, dc_obj.stride), 0, 0);
  todo();  // TODO: if remove this, it doesn't loop properly and crashes the whole thing...
}
