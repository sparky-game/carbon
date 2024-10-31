// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

char *carbon_crypto_base64_encode(const u8 *in, const usz in_size, usz *out_size) {
  static const u8 charset[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
  };
  static const u8 modset[] = {0, 2, 1};
  *out_size = 4 * ((in_size + 2) / 3);
  char *out = CARBON_MALLOC(*out_size);
  if (!out) {
    *out_size = 0;
    return 0;
  }
  for (usz i = 0, j = 0; i < in_size;) {
    u32 A = (i < in_size) ? (u8) in[i++] : 0;
    u32 B = (i < in_size) ? (u8) in[i++] : 0;
    u32 C = (i < in_size) ? (u8) in[i++] : 0;
    u32 t = (A << 0x10) + (B << 0x08) + C;
    out[j++] = charset[(t >> 3*6) & 0x3F];
    out[j++] = charset[(t >> 2*6) & 0x3F];
    out[j++] = charset[(t >> 1*6) & 0x3F];
    out[j++] = charset[(t >> 0*6) & 0x3F];
  }
  for (usz i = 0; i < modset[in_size % 3]; ++i) out[*out_size - i - 1] = '=';
  return out;
}

u8 *carbon_crypto_base64_decode(const u8 *in, usz *out_size) {
  static const u8 codeset[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0,
    0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33,
    34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
  };
  usz out_sz = 0;
  for (usz i = 0; in[4 * i]; ++i) {
    if (in[4*i + 3] == '=') {
      if (in[4*i + 2] == '=') ++out_sz;
      else out_sz += 2;
    }
    else out_sz += 3;
  }
  u8 *out = CARBON_MALLOC(out_sz);
  for (usz i = 0; i < out_sz / 3; ++i) {
    u8 A = codeset[in[4*i + 0]];
    u8 B = codeset[in[4*i + 1]];
    u8 C = codeset[in[4*i + 2]];
    u8 D = codeset[in[4*i + 3]];
    out[3*i + 0] = (A << 2) | (B >> 4);
    out[3*i + 1] = (B << 4) | (C >> 2);
    out[3*i + 2] = (C << 6) | (D >> 0);
  }
  if (out_sz % 3 == 1) {
    i32 n = out_sz / 3;
    u8 A = codeset[in[4*n + 0]];
    u8 B = codeset[in[4*n + 1]];
    out[out_sz - 1] = (A << 2) | (B >> 4);
  }
  else if (out_sz % 3 == 2) {
    i32 n = out_sz / 3;
    u8 A = codeset[in[4*n + 0]];
    u8 B = codeset[in[4*n + 1]];
    u8 C = codeset[in[4*n + 2]];
    out[out_sz - 2] = (A << 2) | (B >> 4);
    out[out_sz - 1] = (B << 4) | (C >> 2);
  }
  *out_size = out_sz;
  return out;
}
