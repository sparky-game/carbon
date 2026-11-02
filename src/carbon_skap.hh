// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.
#ifdef __cplusplus

template <typename T>
struct CBN_SKAP_AssetTrait;

template <>
struct CBN_SKAP_AssetTrait<CBN_Span>
  : cbn::meta::Constant<CBN_SKAP_AssetType, CARBON_SKAP_ASSET_TYPE_BINARY>
{};

template <>
struct CBN_SKAP_AssetTrait<CBN_Image>
  : cbn::meta::Constant<CBN_SKAP_AssetType, CARBON_SKAP_ASSET_TYPE_IMAGE>
{};

template <>
struct CBN_SKAP_AssetTrait<CBN_Mesh>
  : cbn::meta::Constant<CBN_SKAP_AssetType, CARBON_SKAP_ASSET_TYPE_MESH>
{};

template <>
struct CBN_SKAP_AssetTrait<CBN_Font>
  : cbn::meta::Constant<CBN_SKAP_AssetType, CARBON_SKAP_ASSET_TYPE_FONT>
{};

template <typename T>
concept CBN_SKAP_ValidAssetType = requires {
  {CBN_SKAP_AssetTrait<T>::value} -> cbn::meta::ConvertibleTo<CBN_SKAP_AssetType>;
};

struct CBN_SKAP : CBN_SKAP_t {
  static bool Create(const char *decl, const char *skap) { return carbon_skap_create(decl, skap); }

  static cbn::Opt<CBN_SKAP> Open(const char *skap) {
    return cbn::meta::OptWrap<CBN_SKAP>(carbon_skap_open, skap);
  }

  void Free(void) { carbon_skap_close(this); }

  void Print(void) const { carbon_skap_print(this); }

  template <CBN_SKAP_ValidAssetType T>
  cbn::Opt<T> Lookup(const char *name) const {
    return cbn::meta::OptWrap<T>(carbon_skap_lookup, this, GetAssetType<T>(), name);
  }

  usz Count(void) const { return carbon_skap_count(this); }

  template <CBN_SKAP_ValidAssetType T>
  usz CountOf(void) const { return carbon_skap_count_of(this, GetAssetType<T>()); }

  cbn::Opt<cbn::sprite_mgr::UID> LoadSprite(const char *name) const {
    return cbn::sprite_mgr::LoadFromSKAP(name, this);
  }

  cbn::Opt<cbn::mesh_mgr::UID> LoadMesh(const char *name) const {
    return cbn::mesh_mgr::LoadFromSKAP(name, this);
  }

  cbn::Opt<cbn::audio::UID> LoadAudio(const char *name, const cbn::audio::GID *gid = nullptr) const {
    return cbn::audio::LoadFromSKAP(name, this, gid);
  }
  
private:
  template <CBN_SKAP_ValidAssetType T>
  static consteval CBN_SKAP_AssetType GetAssetType(void) { return CBN_SKAP_AssetTrait<T>::value; }
};

#endif
