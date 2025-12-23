// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus

template <typename T>
cbn::Opt<T> cbn::SKAP::Lookup(const char *asset_name) const {
  T asset;
  if (!carbon_skap_lookup(this, GetAssetType<T>(), asset_name, &asset)) return {};
  return asset;
}

template <typename T>
usz cbn::SKAP::CountOf(void) const {
  return carbon_skap_count_of(this, GetAssetType<T>());
}

template <CBN_SKAP_AssetType_t T>
consteval CBN_SKAP_AssetType cbn::SKAP::GetAssetType(void) {
  if constexpr      (cbn::meta::Same_v<T, CBN_Image>)  return CARBON_SKAP_ASSET_TYPE_IMAGE;
  else if constexpr (cbn::meta::Same_v<T, CBN_Binary>) return CARBON_SKAP_ASSET_TYPE_BINARY;
  else if constexpr (cbn::meta::Same_v<T, CBN_Mesh>)   return CARBON_SKAP_ASSET_TYPE_MESH;
  else static_assert(cbn::meta::AlwaysFalse_v<T>, "unsupported type");
}

#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
