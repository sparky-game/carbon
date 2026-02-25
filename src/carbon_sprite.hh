#ifdef __cplusplus

struct CBN_Sprite : CBN_Sprite_t {  
  static CBN_Sprite make(const CBN_Image &img) {
    return carbon_sprite_from_img(&img);
  }

  void Free(void) {
    carbon_sprite_destroy(this);
  }
};

#endif
