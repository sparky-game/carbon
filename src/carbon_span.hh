#ifdef __cplusplus

struct CBN_Span : CBN_Span_t {
  constexpr CBN_Span(void) : CBN_Span_t{nullptr, 0} {}
  constexpr CBN_Span(auto *p, usz sz) : CBN_Span_t{(u8 *)p, sz} {}
};

#endif
