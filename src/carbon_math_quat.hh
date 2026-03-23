#ifdef __cplusplus

struct CBN_Quat : CBN_Quat_t {
  using CBN_Quat_t::CBN_Quat_t;
  constexpr CBN_Quat(void) : CBN_Quat_t(0, 0, 0, 1) {}

  CBN_Quat operator*(const CBN_Quat &q) const {
    return carbon_math_quat_mult(*this, q);
  }
};

#endif
