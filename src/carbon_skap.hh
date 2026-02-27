#ifdef __cplusplus

template <typename T>
struct CBN_SKAP_AssetTrait;

template <>
struct CBN_SKAP_AssetTrait<CBN_Image>
  : cbn::meta::Constant<CBN_SKAP_AssetType, CARBON_SKAP_ASSET_TYPE_IMAGE>
{};

template <>
struct CBN_SKAP_AssetTrait<CBN_Binary>
  : cbn::meta::Constant<CBN_SKAP_AssetType, CARBON_SKAP_ASSET_TYPE_BINARY>
{};

template <>
struct CBN_SKAP_AssetTrait<CBN_Mesh>
  : cbn::meta::Constant<CBN_SKAP_AssetType, CARBON_SKAP_ASSET_TYPE_MESH>
{};

template <typename T>
concept CBN_SKAP_ValidAssetType = requires {
  {CBN_SKAP_AssetTrait<T>::value} -> cbn::meta::ConvertibleTo<CBN_SKAP_AssetType>;
};

struct CBN_SKAP : CBN_SKAP_t {
  static bool Create(const char *decl, const char *skap) {
    return carbon_skap_create(decl, skap);
  }

  static cbn::Opt<CBN_SKAP> Open(const char *skap) {
    CBN_SKAP handle;
    if (!carbon_skap_open(skap, &handle)) return {};
    return handle;
  }

  void Free(void) {
    carbon_skap_close(this);
  }

  void Print(void) const {
    carbon_skap_print(this);
  }

  template <typename T>
  cbn::Opt<T> Lookup(const char *asset_name) const {
    T asset;
    if (!carbon_skap_lookup(this, GetAssetType<T>(), asset_name, &asset)) return {};
    return asset;
  }

  usz Count(void) const {
    return carbon_skap_count(this);
  }

  template <typename T>
  usz CountOf(void) const {
    return carbon_skap_count_of(this, GetAssetType<T>());
  }

  cbn::Opt<CBN_Sprite_UID> LoadSprite(const char *name) const {
    CBN_Sprite_UID uid;
    if (!carbon_sprite_manager_load_from_skap(name, this, &uid)) return {};
    return uid;
  }

  cbn::Opt<CBN_Mesh_UID> LoadMesh(const char *name) const {
    CBN_Mesh_UID uid;
    if (!carbon_mesh_manager_load_from_skap(name, this, &uid)) return {};
    return uid;
  }

  cbn::Opt<CBN_Audio_UID> LoadAudio(const char *name) const {
    CBN_Audio_UID uid;
    if (!carbon_audio_load_from_skap(name, this, &uid)) return {};
    return uid;
  }

  cbn::Opt<CBN_Audio_UID> LoadAudioStream(const char *name) const {
    CBN_Audio_UID uid;
    if (!carbon_audio_load_stream_from_skap(name, this, &uid)) return {};
    return uid;
  }
  
private:
  template <CBN_SKAP_ValidAssetType T>
  static consteval CBN_SKAP_AssetType GetAssetType(void) {
    return CBN_SKAP_AssetTrait<T>::value;
  }
};

#endif
