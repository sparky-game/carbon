#ifdef __cplusplus

struct CBN_Mesh : CBN_Mesh_t {
  static cbn::Opt<CBN_Mesh> make(const char *file) {
    CBN_Mesh m = {};
    if (!carbon_mesh_create_from_file(&m, file)) return {};
    return m;
  }

  void Free(void) {
    carbon_mesh_destroy(this);
  }

  u32 CRC32(void) const;
};

#endif
