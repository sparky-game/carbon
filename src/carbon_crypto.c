// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

#define CARBON_CRYPTO_KECCAK256_MAX_HEX_CSTRS 4
#define CARBON_CRYPTO_KECCAK256__IS_ALIGNED_64(p) (0 == ((uptr) (p) & 7))
#define CARBON_CRYPTO_KECCAK256__ROTATE_LEFT_64(x, y) ((x) << (y) ^ ((x) >> (64 - (y))))

static const u8 carbon_crypto_keccak256__k[] = {
  1, 26, 94, 112, 31, 33, 121, 85, 14, 12, 53, 38, 63, 79, 93, 83, 82, 72, 22, 102, 121, 88, 33, 116,
  1,  6,  9,  22, 14, 20,   2, 12, 13, 19, 23, 15,  4, 24, 21,  8, 16,  5,  3,  18,  17, 11,  7,  10,
  1, 62, 28,  27, 36, 44,   6, 55, 20,  3, 10, 43, 25, 39, 41, 45, 15, 21,  8,  18,   2, 61, 56,  14
};

char *carbon_crypto_base64_encode(const u8 *in, const usz in_size, usz *out_size) {
  static const u8 charset[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
  };
  static const u8 modset[] = {0, 2, 1};
  *out_size = 4 * ((in_size + 2) / 3) + 1;
  char *out = (char *) CARBON_MALLOC(*out_size);
  CARBON_ASSERT(out && "failed to allocate memory");
  for (usz i = 0, j = 0; i < in_size;) {
    u32 A = (i < in_size) ? (u8) in[i++] : 0;
    u32 B = (i < in_size) ? (u8) in[i++] : 0;
    u32 C = (i < in_size) ? (u8) in[i++] : 0;
    u32 t = (A << 0x10) + (B << 0x08) + (C << 0x00);
    out[j++] = charset[(t >> 3*6) & 0x3f];
    out[j++] = charset[(t >> 2*6) & 0x3f];
    out[j++] = charset[(t >> 1*6) & 0x3f];
    out[j++] = charset[(t >> 0*6) & 0x3f];
  }
  for (usz i = 0; i < modset[in_size % 3]; ++i) out[*out_size - i - 2] = '=';
  out[*out_size - 1] = 0;
  --(*out_size);
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
  ++out_sz;
  u8 *out = (u8 *) CARBON_MALLOC(out_sz);
  CARBON_ASSERT(out && "failed to allocate memory");
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
  *out_size = out_sz - 1;
  return out;
}

u32 carbon_crypto_crc32(const u8 *in, const usz in_size) {
  static const u32 codeset[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
  };
  u32 out = ~0U;
  for (usz i = 0; i < in_size; ++i) {
    out = (out >> 8) ^ codeset[in[i] ^ (out & 0xff)];
  }
  return ~out;
}

CARBON_INLINE u64 carbon_crypto_keccak256__get_round_const(u8 round) {
  u64 result = 0;
  u8 k_round = carbon_crypto_keccak256__k[round];
  if (k_round & (1 << 6)) { result |= ((u64) 1 << 63); }
  if (k_round & (1 << 5)) { result |= ((u64) 1 << 31); }
  if (k_round & (1 << 4)) { result |= ((u64) 1 << 15); }
  if (k_round & (1 << 3)) { result |= ((u64) 1 << 7);  }
  if (k_round & (1 << 2)) { result |= ((u64) 1 << 3);  }
  if (k_round & (1 << 1)) { result |= ((u64) 1 << 1);  }
  if (k_round & (1 << 0)) { result |= ((u64) 1 << 0);  }
  return result;
}

CARBON_INLINE void carbon_crypto_keccak256__phase_theta(u64 *A) {
  u64 C[5], D[5];
  for (u8 i = 0; i < 5; ++i) {
    C[i] = A[i];
    for (u8 j = 5; j < 25; j += 5) C[i] ^= A[i + j];
  }
  for (u8 i = 0; i < 5; ++i) {
    D[i] = CARBON_CRYPTO_KECCAK256__ROTATE_LEFT_64(C[(i + 1) % 5], 1) ^ C[(i + 4) % 5];
  }
  for (u8 i = 0; i < 5; ++i) {
    for (u8 j = 0; j < 25; j += 5) {
      A[i + j] ^= D[i];
    }
  }
}

CARBON_INLINE void carbon_crypto_keccak256__phase_pi(u64 *A) {
  u64 A1 = A[1];
  for (u8 i = 1; i < 24; ++i) {
    A[carbon_crypto_keccak256__k[24 + i - 1]] = A[carbon_crypto_keccak256__k[24 + i]];
  }
  A[10] = A1;
}

CARBON_INLINE void carbon_crypto_keccak256__phase_chi(u64 *A) {
  for (u8 i = 0; i < 25; i += 5) {
    u64 A0 = A[i + 0], A1 = A[i + 1];
    A[i + 0] ^= ~A1 & A[i + 2];
    A[i + 1] ^= ~A[i + 2] & A[i + 3];
    A[i + 2] ^= ~A[3 + i] & A[i + 4];
    A[i + 3] ^= ~A[i + 4] & A0;
    A[i + 4] ^= ~A0 & A1;
  }
}

CARBON_INLINE void carbon_crypto_keccak256__permutation(u64 *hash) {
  for (u8 r = 0; r < 24; ++r) {
    carbon_crypto_keccak256__phase_theta(hash);
    for (u8 i = 1; i < 25; ++i) {
      hash[i] = CARBON_CRYPTO_KECCAK256__ROTATE_LEFT_64(hash[i], carbon_crypto_keccak256__k[48 + i - 1]);
    }
    carbon_crypto_keccak256__phase_pi(hash);
    carbon_crypto_keccak256__phase_chi(hash);
    *hash ^= carbon_crypto_keccak256__get_round_const(r);
  }
}

CARBON_INLINE void carbon_crypto_keccak256__process_blk(u64 *hash, const u64 *blk) {
  for (u8 i = 0; i < 17; ++i) hash[i] ^= blk[i];
  carbon_crypto_keccak256__permutation(hash);
}

CARBON_INLINE void carbon_crypto_keccak256__update(CBN_Keccak256 *ctx, const u8 *msg, u16 msg_size) {
  u16 idx = ctx->rest;
  ctx->rest = (ctx->rest + msg_size) % 136;
  if (idx) {
    u16 left = 136 - idx;
    carbon_memory_copy((u8 *) ctx->msg + idx, msg, CARBON_MIN(msg_size, left));
    if (msg_size < left) return;
    carbon_crypto_keccak256__process_blk(ctx->hash, ctx->msg);
    msg += left;
    msg_size -= left;
  }
  while (msg_size >= 136) {
    u64 *aligned_msg_blk;
    if (CARBON_CRYPTO_KECCAK256__IS_ALIGNED_64(msg)) aligned_msg_blk = (u64 *) msg;
    else {
      carbon_memory_copy(ctx->msg, msg, 136);
      aligned_msg_blk = ctx->msg;
    }
    carbon_crypto_keccak256__process_blk(ctx->hash, aligned_msg_blk);
    msg += 136;
    msg_size -= 136;
  }
  if (msg_size) carbon_memory_copy(ctx->msg, msg, msg_size);
}

void carbon_crypto_keccak256(const u8 *in, const usz in_size, u8 *out) {
  if (!out) {
    carbon_log_warn("`out` is not a valid pointer, skipping computation");
    return;
  }
  CBN_Keccak256 ctx;
  carbon_memory_set(&ctx, 0, sizeof(ctx));
  carbon_crypto_keccak256__update(&ctx, in, in_size);
  carbon_memory_set((u8 *) ctx.msg + ctx.rest, 0, 136 - ctx.rest);
  ((u8 *) ctx.msg)[ctx.rest] |= 0x01;
  ((u8 *) ctx.msg)[136 - 1]  |= 0x80;
  carbon_crypto_keccak256__process_blk(ctx.hash, ctx.msg);
  carbon_memory_copy(out, ctx.hash, 32);
}

void carbon_crypto_keccak256_to_hex_cstr(const u8 *hash, char *out_cstr) {
  for (usz i = 0; i < 32; ++i) {
    snprintf(&out_cstr[i*2], 3, "%02x", hash[i]);
  }
}

char *carbon_crypto_keccak256_as_hex_cstr(const u8 *in, const usz in_size) {
  static usz i = 0;
  static char xs[CARBON_CRYPTO_KECCAK256_MAX_HEX_CSTRS][32*2 + 1];
  char *x = xs[i];
  u8 hash[32] = {0};
  carbon_crypto_keccak256(in, in_size, hash);
  carbon_crypto_keccak256_to_hex_cstr(hash, x);
  ++i;
  if (i >= CARBON_CRYPTO_KECCAK256_MAX_HEX_CSTRS) i = 0;
  return x;
}
