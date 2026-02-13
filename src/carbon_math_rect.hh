#ifdef __cplusplus

struct CBN_Rect : CBN_Rect_t {
  constexpr CBN_Rect(f32 _x, f32 _y, f32 _w, f32 _h) : CBN_Rect_t{_x, _y, _w, _h} {}
  constexpr CBN_Rect(void) : CBN_Rect(0, 0, 0, 0) {}
  constexpr CBN_Rect(CBN_Vec2 v, f32 _w, f32 _h) : CBN_Rect(v.x, v.y, _w, _h) {}
  constexpr CBN_Rect(f32 _x, f32 _y, f32 sz) : CBN_Rect(_x, _y, sz, sz) {}
  constexpr CBN_Rect(CBN_Vec2 v, f32 sz) : CBN_Rect(v.x, v.y, sz) {}

  bool operator==(const CBN_Rect &r) const {
    return x == r.x && y == r.y && w == r.w && h == r.h;
  }

  bool Contains(const CBN_Vec2 &p) const {
    return carbon_math_rect_contains(*this, p);
  }

  bool Overlaps(const CBN_Rect &r) const {
    return carbon_math_rect_overlaps(*this, r);
  }

  CBN_Rect Intersection(const CBN_Rect &r) const {
    return carbon_math_rect_intersection(*this, r);
  }

  void Scale(const f32 s) {
    carbon_math_rect_scale(this, s);
  }
};

#endif
