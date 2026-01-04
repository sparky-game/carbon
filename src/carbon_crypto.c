// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

#define CARBON_CRYPTO_SHA1_MAX_HEX_CSTRS      4
#define CARBON_CRYPTO_SHA256_MAX_HEX_CSTRS    4
#define CARBON_CRYPTO_KECCAK256_MAX_HEX_CSTRS 4

#define CARBON_CRYPTO_SHA1__ROTATE_LEFT(x, y)         (((x) << (y)) | ((x) >> (32 - (y))))
#define CARBON_CRYPTO_SHA256__ROTLEFT(a, b)           (((a) << (b)) | ((a) >> (32 - (b))))
#define CARBON_CRYPTO_SHA256__ROTRIGHT(a, b)          (((a) >> (b)) | ((a) << (32 - (b))))
#define CARBON_CRYPTO_SHA256__CH(x, y, z)             (((x) & (y)) ^ (~(x) & (z)))
#define CARBON_CRYPTO_SHA256__MAJ(x, y, z)            (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define CARBON_CRYPTO_SHA256__EP0(x)                  (CARBON_CRYPTO_SHA256__ROTRIGHT(x, 2) ^ CARBON_CRYPTO_SHA256__ROTRIGHT(x, 13) ^ CARBON_CRYPTO_SHA256__ROTRIGHT(x, 22))
#define CARBON_CRYPTO_SHA256__EP1(x)                  (CARBON_CRYPTO_SHA256__ROTRIGHT(x, 6) ^ CARBON_CRYPTO_SHA256__ROTRIGHT(x, 11) ^ CARBON_CRYPTO_SHA256__ROTRIGHT(x, 25))
#define CARBON_CRYPTO_SHA256__SIG0(x)                 (CARBON_CRYPTO_SHA256__ROTRIGHT(x, 7) ^ CARBON_CRYPTO_SHA256__ROTRIGHT(x, 18) ^ ((x) >> 3))
#define CARBON_CRYPTO_SHA256__SIG1(x)                 (CARBON_CRYPTO_SHA256__ROTRIGHT(x, 17) ^ CARBON_CRYPTO_SHA256__ROTRIGHT(x, 19) ^ ((x) >> 10))
#define CARBON_CRYPTO_KECCAK256__IS_ALIGNED_64(p)     (0 == ((uptr) (p) & 7))
#define CARBON_CRYPTO_KECCAK256__ROTATE_LEFT_64(x, y) ((x) << (y) ^ ((x) >> (64 - (y))))

typedef struct {
  u8 data[64];
  u32 state[8];
  usz datalen;
  usz bitlen;
} CBN_SHA256_CTX;

typedef struct {
  u64 hash[25];
  u64 msg[24];
  u16 rest;
} CBN_Keccak256_CTX;

static const u32 carbon_crypto_sha256__k[] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

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
  char *out = (char *) carbon_memory_alloc(*out_size);
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
  u8 *out = (u8 *) carbon_memory_alloc(out_sz);
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

CBNINL u32 carbon_crypto_crc32__gf2_mult(const u32 *m, u32 v) {
  u32 s = 0;
  for (u32 i = 0; v; ++i, v >>= 1) {
    if (v & 1) s ^= m[i];
  }
  return s;
}

CBNINL void carbon_crypto_crc32__gf2_square(u32 *dest, const u32 *src) {
  for (u32 i = 0; i < 32; ++i) dest[i] = carbon_crypto_crc32__gf2_mult(src, src[i]);
}

u32 carbon_crypto_crc32_combine(u32 c1, u32 c2, usz n2) {
  u32 odd[32], even[32];
  odd[0] = 0xedb88320;
  for (u32 i = 1; i < 32; ++i) odd[i] = 1 << (i - 1);
  carbon_crypto_crc32__gf2_square(even, odd);
  u32 c = c1;
  while (n2) {
    carbon_crypto_crc32__gf2_square(odd, even);
    if (n2 & 1) c = carbon_crypto_crc32__gf2_mult(odd, c);
    n2 >>= 1;
    if (!n2) break;
    carbon_crypto_crc32__gf2_square(even, odd);
    if (n2 & 1) c = carbon_crypto_crc32__gf2_mult(even, c);
    n2 >>= 1;
  }
  c ^= c2;
  return c;
}

u64 carbon_crypto_djb2(const char *in) {
  u64 hash = 5381;
  i32 c = 0;
  while ((c = *in++)) hash = ((hash << 5) + hash) + c;
  return hash;
}

void carbon_crypto_sha1(const u8 *in, const usz in_size, u8 *out) {
  if (!out) {
    CBN_WARN("`out` is not a valid pointer, skipping computation");
    return;
  }
  u32 h[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};
  usz pad_size = ((in_size + 8) / 64 + 1) * 64;
  u8 *msg = (u8 *) carbon_memory_zeroed(pad_size);
  carbon_memory_copy(msg, in, in_size);
  msg[in_size] = 0x80;
  u64 bit_len = (u64) in_size * 8;
  for (usz i = 0; i < 8; ++i) {
    msg[pad_size - 8 + i] = (bit_len >> (56 - 8 * i)) & 0xff;
  }
  for (usz off = 0; off < pad_size; off += 64) {
    u32 w[80] = {0};
    for (usz i = 0; i < 16; ++i) {
      const u8 *d = msg + off + i*4;
      w[i] = ((u32) d[0] << 24) | ((u32) d[1] << 16) | ((u32) d[2] << 8) | ((u32) d[3] << 0);
    }
    for (usz i = 16; i < 80; ++i) {
      w[i] = CARBON_CRYPTO_SHA1__ROTATE_LEFT(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }
    u32 a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
    for (usz i = 0; i < 80; ++i) {
      u32 f, k;
      if (i < 20) {
        f = (b & c) | (~b & d);
        k = 0x5a827999;
      }
      else if (i < 40) {
        f = b ^ c ^ d;
        k = 0x6ed9eba1;
      }
      else if (i < 60) {
        f = (b & c) | (b & d) | (c & d);
        k = 0x8f1bbcdc;
      }
      else {
        f = b ^ c ^ d;
        k = 0xca62c1d6;
      }
      u32 tmp = CARBON_CRYPTO_SHA1__ROTATE_LEFT(a, 5) + f + e + k + w[i];
      e = d;
      d = c;
      c = CARBON_CRYPTO_SHA1__ROTATE_LEFT(b, 30);
      b = a;
      a = tmp;
    }
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
  }
  carbon_memory_free(msg);
  for (usz i = 0; i < CARBON_ARRAY_LEN(h); ++i) {
    out[i * 4 + 0] = (h[i] >> 24) & 0xff;
    out[i * 4 + 1] = (h[i] >> 16) & 0xff;
    out[i * 4 + 2] = (h[i] >> 8)  & 0xff;
    out[i * 4 + 3] = (h[i] >> 0)  & 0xff;
  }
}

void carbon_crypto_sha1_to_hex_cstr(const u8 *hash, char *out_cstr) {
  for (usz i = 0; i < 20; ++i) {
    snprintf(&out_cstr[i*2], 3, "%02x", hash[i]);
  }
}

char *carbon_crypto_sha1_as_hex_cstr(const u8 *in, const usz in_size) {
  static usz i = 0;
  static char xs[CARBON_CRYPTO_SHA1_MAX_HEX_CSTRS][20*2 + 1];
  char *x = xs[i];
  u8 hash[20] = {0};
  carbon_crypto_sha1(in, in_size, hash);
  carbon_crypto_sha1_to_hex_cstr(hash, x);
  ++i;
  if (i >= CARBON_CRYPTO_SHA1_MAX_HEX_CSTRS) i = 0;
  return x;
}

CBNINL void carbon_crypto_sha256__transform(CBN_SHA256_CTX *ctx, const u8 *data) {
  u32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
  for (i = 0, j = 0; i < 16; ++i, j += 4) {
    m[i] = ((u32) data[j + 0] << 24) | ((u32) data[j + 1] << 16) | ((u32) data[j + 2] << 8) | ((u32) data[j + 3] << 0);
  }
  for (; i < 64; ++i) {
    m[i] = CARBON_CRYPTO_SHA256__SIG1(m[i - 2]) + m[i - 7] + CARBON_CRYPTO_SHA256__SIG0(m[i - 15]) + m[i - 16];
  }
  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  e = ctx->state[4];
  f = ctx->state[5];
  g = ctx->state[6];
  h = ctx->state[7];
  for (i = 0; i < 64; ++i) {
    t1 = h + CARBON_CRYPTO_SHA256__EP1(e) + CARBON_CRYPTO_SHA256__CH(e, f, g) + carbon_crypto_sha256__k[i] + m[i];
    t2 = CARBON_CRYPTO_SHA256__EP0(a) + CARBON_CRYPTO_SHA256__MAJ(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }
  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
  ctx->state[5] += f;
  ctx->state[6] += g;
  ctx->state[7] += h;
}

CBNINL void carbon_crypto_sha256__update(CBN_SHA256_CTX *ctx, const u8 *data, const usz len) {
  for (u32 i = 0; i < len; ++i) {
    ctx->data[ctx->datalen] = data[i];
    ++ctx->datalen;
    if (ctx->datalen == 64) {
      carbon_crypto_sha256__transform(ctx, ctx->data);
      ctx->bitlen += 512;
      ctx->datalen = 0;
    }
  }
}

CBNINL void carbon_crypto_sha256__assemble(CBN_SHA256_CTX *ctx, u8 *hash) {
  u32 i = ctx->datalen;
  if (ctx->datalen < 56) {
    ctx->data[i++] = 0x80;
    while (i < 56) ctx->data[i++] = 0;
  }
  else {
    ctx->data[i++] = 0x80;
    while (i < 64) ctx->data[i++] = 0;
    carbon_crypto_sha256__transform(ctx, ctx->data);
    carbon_memory_set(ctx->data, 0, 56);
  }
  ctx->bitlen  += ctx->datalen * 8;
  ctx->data[63] = ctx->bitlen >> 0;
  ctx->data[62] = ctx->bitlen >> 8;
  ctx->data[61] = ctx->bitlen >> 16;
  ctx->data[60] = ctx->bitlen >> 24;
  ctx->data[59] = ctx->bitlen >> 32;
  ctx->data[58] = ctx->bitlen >> 40;
  ctx->data[57] = ctx->bitlen >> 48;
  ctx->data[56] = ctx->bitlen >> 56;
  carbon_crypto_sha256__transform(ctx, ctx->data);
  for (i = 0; i < 4; ++i) {
    hash[i + 0]  = (ctx->state[0] >> (24 - i * 8)) & 0xff;
    hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0xff;
    hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0xff;
    hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0xff;
    hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0xff;
    hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0xff;
    hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0xff;
    hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0xff;
  }
}

void carbon_crypto_sha256(const u8 *in, const usz in_size, u8 *out) {
  if (!out) {
    CBN_WARN("`out` is not a valid pointer, skipping computation");
    return;
  }
  CBN_SHA256_CTX ctx;
  carbon_memory_set(&ctx, 0, sizeof(ctx));
  ctx.state[0] = 0x6a09e667;
  ctx.state[1] = 0xbb67ae85;
  ctx.state[2] = 0x3c6ef372;
  ctx.state[3] = 0xa54ff53a;
  ctx.state[4] = 0x510e527f;
  ctx.state[5] = 0x9b05688c;
  ctx.state[6] = 0x1f83d9ab;
  ctx.state[7] = 0x5be0cd19;
  carbon_crypto_sha256__update(&ctx, in, in_size);
  carbon_crypto_sha256__assemble(&ctx, out);
}

void carbon_crypto_sha256_to_hex_cstr(const u8 *hash, char *out_cstr) {
  for (usz i = 0; i < 32; ++i) {
    snprintf(&out_cstr[i*2], 3, "%02x", hash[i]);
  }
}

char *carbon_crypto_sha256_as_hex_cstr(const u8 *in, const usz in_size) {
  static usz i = 0;
  static char xs[CARBON_CRYPTO_SHA256_MAX_HEX_CSTRS][32*2 + 1];
  char *x = xs[i];
  u8 hash[32] = {0};
  carbon_crypto_sha256(in, in_size, hash);
  carbon_crypto_sha256_to_hex_cstr(hash, x);
  ++i;
  if (i >= CARBON_CRYPTO_SHA256_MAX_HEX_CSTRS) i = 0;
  return x;
}

CBNINL u64 carbon_crypto_keccak256__get_round_const(u8 round) {
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

CBNINL void carbon_crypto_keccak256__phase_theta(u64 *A) {
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

CBNINL void carbon_crypto_keccak256__phase_pi(u64 *A) {
  u64 A1 = A[1];
  for (u8 i = 1; i < 24; ++i) {
    A[carbon_crypto_keccak256__k[24 + i - 1]] = A[carbon_crypto_keccak256__k[24 + i]];
  }
  A[10] = A1;
}

CBNINL void carbon_crypto_keccak256__phase_chi(u64 *A) {
  for (u8 i = 0; i < 25; i += 5) {
    u64 A0 = A[i + 0], A1 = A[i + 1];
    A[i + 0] ^= ~A1 & A[i + 2];
    A[i + 1] ^= ~A[i + 2] & A[i + 3];
    A[i + 2] ^= ~A[3 + i] & A[i + 4];
    A[i + 3] ^= ~A[i + 4] & A0;
    A[i + 4] ^= ~A0 & A1;
  }
}

CBNINL void carbon_crypto_keccak256__permutation(u64 *hash) {
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

CBNINL void carbon_crypto_keccak256__process_blk(u64 *hash, const u64 *blk) {
  for (u8 i = 0; i < 17; ++i) hash[i] ^= blk[i];
  carbon_crypto_keccak256__permutation(hash);
}

CBNINL void carbon_crypto_keccak256__update(CBN_Keccak256_CTX *ctx, const u8 *msg, u16 msg_size) {
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
    CBN_WARN("`out` is not a valid pointer, skipping computation");
    return;
  }
  CBN_Keccak256_CTX ctx;
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
