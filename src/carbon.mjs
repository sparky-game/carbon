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

function TODO() { throw new Error("NOT IMPLEMENTED"); }

export let wasm = null;

export async function init(wasmFile) {
  wasm = await WebAssembly.instantiateStreaming(fetch(wasmFile), {
    env: make_env({
      // char *carbon_string_fmt(const char *s, ...);
      carbon_string_fmt: (s, ...args) => {
        TODO();
      },
      // void carbon_log_print(FILE *stream, const char *fmt, ...);
      carbon_log_print: (stream, fmt, ...args) => {
        TODO();
      }
    })
  });
}
