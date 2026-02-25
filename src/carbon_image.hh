#ifdef __cplusplus

struct CBN_Image : CBN_Image_t {
  static CBN_Image FromFile(const char *file) {
    return carbon_image_read_from_file(file);
  }

  static CBN_Image FromCanvas(const CBN_DrawCanvas &dc) {
    return carbon_image_from_canvas(&dc);
  }
  
  void Free(void) {
    carbon_image_destroy(this);
  }

  bool Save(CBN_Image_Format fmt, const char *file) const {
    return carbon_image_write_to_file(this, fmt, file);
  }
};

#endif
