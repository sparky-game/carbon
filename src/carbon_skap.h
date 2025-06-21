/*
**  $$======================================$$
**  ||       Sparky Asset Pack (SKAP)       ||
**  $$======================================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_SKAP_DECL_FILE_MAX_LINE_LEN 256
#define CARBON_SKAP_DECL_FILE_MAX_PATH_LEN 127
#define CARBON_SKAP_DECL_FILE_MAX_TYPE_LEN 63

typedef enum {
  CARBON_SKAP_ASSET_TYPE_IMAGE,
  CARBON_SKAP_ASSET_TYPE_COUNT
} CBN_SKAP_AssetType;

typedef struct {
  CBN_SKAP_AssetType type;
  union {
    CBN_Image_Metadata as_img;
  };
} CBN_SKAP_AssetMetadata;

typedef struct {
  char name[CARBON_SKAP_DECL_FILE_MAX_LINE_LEN];
  usz blob_offset;
  usz blob_size;
  u32 checksum;
  CBN_SKAP_AssetMetadata metadata;
} CBN_SKAP_AssetIdx;

typedef struct {
  char path[CARBON_SKAP_DECL_FILE_MAX_PATH_LEN + 1];
  char type[CARBON_SKAP_DECL_FILE_MAX_TYPE_LEN + 1];
  CBN_StrList assets;
} CBN_SKAP_AssetGroup;

typedef struct {
  char signature[4];
  u8 fmt_ver;
  u64 build_ver;
} CBN_SKAP_Header;

typedef struct CBN_SKAP {
  FILE *fd;
  usz blob_section_start_pos;
  CBN_SKAP_Header header;
  usz type_counters[CARBON_SKAP_ASSET_TYPE_COUNT];
  CBN_HashMap idxs[CARBON_SKAP_ASSET_TYPE_COUNT];
#ifdef __cplusplus
  /**
   * @brief carbon_skap_create
   * @param decl The filename of the DECL file in disk.
   * @param skap The filename that the resulting SKAP file should have in disk.
   * @return A boolean value indicating whether it created the SKAP successfully or not.
   */
  static u8 Create(const char *decl, const char *skap);
  /**
   * @brief carbon_skap_open
   * @param skap The filename of the SKAP in disk.
   * @return The handle info object of the opened SKAP.
   */
  static Opt<CBN_SKAP> make(const char *skap);
  /**
   * @brief carbon_skap_close
   */
  void Free(void);
  /**
   * @brief carbon_skap_print
   */
  void Print(void) const;
  /**
   * @brief carbon_skap_lookup
   */
  template <typename T>
  Opt<T> Lookup(const char *asset_name) const;
  /**
   * @brief carbon_skap_count
   * @return The number of assets stored in the opened SKAP.
   */
  usz Count(void) const;
  /**
   * @brief carbon_skap_count_of
   * @return The number of assets of the specified type stored in the opened SKAP.
   */
  template <typename T>
  usz CountOf(void) const;
private:
  template <typename T>
  static constexpr CBN_SKAP_AssetType GetAssetType(void);
#endif
} CBN_SKAP;

/**
 * @brief Creates a SKAP from a declarations (DECL) file.
 *
 * A DECL file is a custom text format, which enables an ergonomic and convinient way of
 * listing and declaring all asset files wanted to package in a SKAP (as a shipping method).
 * Full format specification: <https://gist.github.com/iWas-Coder/bc6269afb2b191d1e78faf6a7ec52c54>.
 *
 * @param decl The filename of the DECL file in disk.
 * @param skap The filename that the resulting SKAP file should have in disk.
 * @return A boolean value indicating whether it created the SKAP successfully or not.
 */
CARBON_API u8 carbon_skap_create(const char *decl, const char *skap);

/**
 * @brief Opens a SKAP from disk.
 *
 * This procedure doesn't load the entire SKAP, just the indices and its metadata and global info.
 * To obtain any stored asset from it, it must be done so by requesting it via the `carbon_skap_lookup`
 * function call, providing all necessary info to be able to locate it (i.e. type and name).
 *
 * @param skap The filename of the SKAP in disk.
 * @param out_handle The handle info object of the opened SKAP (output argument pointer).
 * @return A boolean value indicating whether it opened the SKAP successfully or not.
 */
CARBON_API u8 carbon_skap_open(const char *skap, CBN_SKAP *out_handle);

/**
 * @brief Closes a previously opened SKAP.
 * @param handle The handle info object of the opened SKAP.
 */
CARBON_API void carbon_skap_close(CBN_SKAP *handle);

/**
 * @brief Prints out the metadata or global info of a SKAP.
 * @param handle The handle info object of the opened SKAP.
 */
CARBON_API void carbon_skap_print(const CBN_SKAP *handle);

/**
 * @brief Requests the retrieval or acquisition of an asset from a SKAP.
 * @param handle The handle info object of the opened SKAP.
 * @param asset_type The type of the requested asset.
 * @param asset_name The name of the requested asset.
 * @param out_blob The retrieved asset from the SKAP (output argument pointer).
 * @return A boolean value indicating whether it retrieved the requested asset from the SKAP successfully or not.
 */
CARBON_API u8 carbon_skap_lookup(const CBN_SKAP *handle, const CBN_SKAP_AssetType asset_type, const char *asset_name, void *out_blob);

/**
 * @brief Counts how many assets are stored in a SKAP.
 * @param handle The handle info object of the opened SKAP.
 * @return The number of assets stored in the opened SKAP.
 */
CARBON_API usz carbon_skap_count(const CBN_SKAP *handle);

/**
 * @brief Counts how many assets of a specific type are stored in a SKAP.
 * @param handle The handle info object of the opened SKAP.
 * @param type The asset type you're interested in.
 * @return The number of assets of the specified type stored in the opened SKAP.
 */
CARBON_API usz carbon_skap_count_of(const CBN_SKAP *handle, const CBN_SKAP_AssetType type);

// Because `CBN_SKAP` has some templated member functions, their definitions
// need to be available at compile-time, not link-time; thus, we define them
// here for the template instantiation to work properly.
#ifdef __cplusplus
/**
 * @brief CBN_SKAP.Lookup<T>
 */
template <typename T>
Opt<T> CBN_SKAP::Lookup(const char *asset_name) const {
  T asset;
  if (!carbon_skap_lookup(this, GetAssetType<T>(), asset_name, &asset)) return {};
  return asset;
}
/**
 * @brief CBN_SKAP.CountOf<T>
 */
template <typename T>
usz CBN_SKAP::CountOf(void) const {
  return carbon_skap_count_of(this, GetAssetType<T>());
}
/**
 * @brief CBN_SKAP::GetAssetType<T>
 */
template <typename T>
constexpr CBN_SKAP_AssetType CBN_SKAP::GetAssetType(void) {
  if constexpr (CARBON_TYPE_IS_SAME(T, CBN_Image)) return CARBON_SKAP_ASSET_TYPE_IMAGE;
  else {
    CARBON_STATIC_ASSERT(std::always_false<T>::value, "type T is not a valid asset type");
    return CARBON_SKAP_ASSET_TYPE_COUNT;
  }
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
