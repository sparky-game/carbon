// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

CBN_Span carbon_span_from_buf(void *data, usz size) {
  return (CBN_Span){data, size};
}

CBN_Span carbon_span_from_list(const CBN_List *l) {
  return carbon_span_from_buf(l->items, l->size);
}

CBN_Span carbon_span_subspan(CBN_Span s, usz offset, usz count) {
  CBN_ASSERT(offset <= s.size && "Span offset out of bounds");
  CBN_ASSERT(count <= s.size - offset && "Span count out of bounds");
  return carbon_span_from_buf(s.data + offset, count);
}
