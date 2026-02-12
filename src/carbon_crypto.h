/*
  $$====================$$
  ||       Crypto       ||
  $$====================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Encodes binary data to a newly-allocated Base64 string.
 *
 * The caller of this function is responsible for freeing this memory using `CBN_FREE`
 * or equivalent.
 *
 * @param in The input binary data to be encoded.
 * @param in_size The size of the input binary data in bytes.
 * @param out_size The size of the resulting Base64 string (output argument pointer).
 * @return The resulting Base64 string.
 */
CBNDEF char *carbon_crypto_base64_encode(const u8 *in, const usz in_size, usz *out_size);
CBNDEF_FN(cbn::crypto::b64, Encode, carbon_crypto_base64_encode);

/**
 * @brief Decodes a Base64 string to newly-allocated binary data buffer.
 *
 * The caller of this function is responsible for freeing this memory using `CBN_FREE`
 * or equivalent.
 *
 * @param in The input Base64 string to be decoded.
 * @param out_size The size of the resulting binary data in bytes (output argument pointer).
 * @return The resulting binary data buffer.
 */
CBNDEF u8 *carbon_crypto_base64_decode(const u8 *in, usz *out_size);
CBNDEF_FN(cbn::crypto::b64, Decode, carbon_crypto_base64_decode);

/**
 * @brief Computes the CRC32 checksum of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @return The 32-bit CRC32 checksum of the input binary data.
 */
CBNDEF u32 carbon_crypto_crc32(const u8 *in, const usz in_size);
CBNDEF_FN(cbn::crypto::crc32, Compute, carbon_crypto_crc32);

/**
 * @brief Combines two CRC32 checksums into one.
 * @param c1 The first checksum.
 * @param c2 The second checksum.
 * @param n2 The size of the data used to compute `c2`.
 * @return The 32-bit CRC32 checksum result of combining `c1` and `c2`.
 */
CBNDEF u32 carbon_crypto_crc32_combine(u32 c1, u32 c2, usz n2);
CBNDEF_FN(cbn::crypto::crc32, Combine, carbon_crypto_crc32_combine);

/**
 * @brief Computes the DJB2 non-cryptographic hash of the given null-terminated string.
 * @param in The input string.
 * @return The 64-bit non-cryptographic hash of the input string.
 */
CBNDEF u64 carbon_crypto_djb2(const char *in);
CBNDEF_FN(cbn::crypto, DJB2, carbon_crypto_djb2);

/**
 * @brief Computes the SHA-1 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @param out The resulting SHA-1 hash as a 20-byte buffer (output argument pointer).
 */
CBNDEF void carbon_crypto_sha1(const u8 *in, const usz in_size, u8 *out);
CBNDEF_FN(cbn::crypto::sha1, Compute, carbon_crypto_sha1);

/**
 * @brief Converts a 20-byte buffer containing a SHA-1 hash to its HEX string representation.
 *
 * The resulting HEX string buffer (`out_cstr`) must be at least 41 characters long (20*2 + 1).
 *
 * @param hash The SHA-1 hash as a 20-byte buffer.
 * @param out_cstr The resulting HEX string representation of the SHA-1 hash (output argument pointer).
 */
CBNDEF void carbon_crypto_sha1_to_hex_cstr(const u8 *hash, char *out_cstr);
CBNDEF_FN(cbn::crypto::sha1, ToHexString, carbon_crypto_sha1_to_hex_cstr);

/**
 * @brief Computes the SHA-1 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @return The resulting SHA-1 hash as a HEX string.
 */
CBNDEF char *carbon_crypto_sha1_as_hex_cstr(const u8 *in, const usz in_size);
CBNDEF_FN(cbn::crypto::sha1, AsHexString, carbon_crypto_sha1_as_hex_cstr);

/**
 * @brief Computes the SHA-256 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @param out The resulting SHA-256 hash as a 32-byte buffer (output argument pointer).
 */
CBNDEF void carbon_crypto_sha256(const u8 *in, const usz in_size, u8 *out);
CBNDEF_FN(cbn::crypto::sha256, Compute, carbon_crypto_sha256);

/**
 * @brief Converts a 32-byte buffer containing a SHA-256 hash to its HEX string representation.
 *
 * The resulting HEX string buffer (`out_cstr`) must be at least 65 characters long (32*2 + 1).
 *
 * @param hash The SHA-256 hash as a 32-byte buffer.
 * @param out_cstr The resulting HEX string representation of the SHA-256 hash (output argument pointer).
 */
CBNDEF void carbon_crypto_sha256_to_hex_cstr(const u8 *hash, char *out_cstr);
CBNDEF_FN(cbn::crypto::sha256, ToHexString, carbon_crypto_sha256_to_hex_cstr);

/**
 * @brief Computes the SHA-256 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @return The resulting SHA-256 hash as a HEX string.
 */
CBNDEF char *carbon_crypto_sha256_as_hex_cstr(const u8 *in, const usz in_size);
CBNDEF_FN(cbn::crypto::sha256, AsHexString, carbon_crypto_sha256_as_hex_cstr);

/**
 * @brief Computes the Keccak-256 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @param out The resulting Keccak-256 hash as a 32-byte buffer (output argument pointer).
 */
CBNDEF void carbon_crypto_keccak256(const u8 *in, const usz in_size, u8 *out);
CBNDEF_FN(cbn::crypto::keccak256, Compute, carbon_crypto_keccak256);

/**
 * @brief Converts a 32-byte buffer containing a Keccak-256 hash to its HEX string representation.
 *
 * The resulting HEX string buffer (`out_cstr`) must be at least 65 characters long (32*2 + 1).
 *
 * @param hash The Keccak-256 hash as a 32-byte buffer.
 * @param out_cstr The resulting HEX string representation of the Keccak-256 hash (output argument pointer).
 */
CBNDEF void carbon_crypto_keccak256_to_hex_cstr(const u8 *hash, char *out_cstr);
CBNDEF_FN(cbn::crypto::keccak256, ToHexString, carbon_crypto_keccak256_to_hex_cstr);

/**
 * @brief Computes the Keccak-256 hash of the given binary data.
 * @param in The input binary data.
 * @param in_size The size of the input binary data in bytes.
 * @return The resulting Keccak-256 hash as a HEX string.
 */
CBNDEF char *carbon_crypto_keccak256_as_hex_cstr(const u8 *in, const usz in_size);
CBNDEF_FN(cbn::crypto::keccak256, AsHexString, carbon_crypto_keccak256_as_hex_cstr);
