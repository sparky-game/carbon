// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

export function make_env(...envs) {
  return new Proxy(envs, {
    get(target, prop, receiver) {
      for (let env of envs) {
        if (env.hasOwnProperty(prop)) return env[prop];
      }
      return (...args) => { throw new Error(`NOT IMPLEMENTED: ${prop} (${args})`); };
    }
  });
}

export let wasm = null;

export async function init(wasmFile) {
  wasm = await WebAssembly.instantiateStreaming(fetch(wasmFile), {
    env: make_env({
      puts: puts
    })
  });
}

export function puts(ptr) {
  const bytes = new Uint8Array(wasm.instance.exports.memory.buffer);
  let str = "";
  while (bytes[ptr] !== 0) str += String.fromCharCode(bytes[ptr++]);
  console.log(str);
}
