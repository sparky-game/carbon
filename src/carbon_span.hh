#ifdef __cplusplus

struct CBN_Span : CBN_Span_t {
  CBN_Span(void) : CBN_Span_t{} {}
  CBN_Span(auto *p, usz sz) : CBN_Span(carbon_span_from_buf(p, sz)) {}
  CBN_Span(const CBN_List &l) : CBN_Span(carbon_span_from_list(&l)) {}
};

#endif
