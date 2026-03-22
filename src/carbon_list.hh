#ifdef __cplusplus

template <typename T>
struct CBN_List_tt : CBN_List_t {
  using value_type = T;
  using iterator = value_type *;

  static CBN_List_tt New(void) {
    auto l = carbon_list_create(sizeof(value_type));
    return *(CBN_List_tt *) &l;
  }

  void Free(void) {
    carbon_list_destroy((CBN_List *) this);
  }

  value_type Front(void) const {
    value_type x;
    carbon_list_front((CBN_List *) this, &x);
    return x;
  }

  value_type Back(void) const {
    value_type x;
    carbon_list_back((CBN_List *) this, &x);
    return x;
  }

  void Push(const value_type &value) {
    carbon_list_push((CBN_List *) this, (void *) &value);
  }

  value_type PopFront(void) {
    value_type x;
    carbon_list_pop_front((CBN_List *) this, &x);
    return x;
  }

  value_type PopBack(void) {
    value_type x;
    carbon_list_pop_back((CBN_List *) this, &x);
    return x;
  }

  isz Find(const value_type &value) const {
    return carbon_list_find((CBN_List *) this, (void *) &value);
  }

  void Remove(usz idx) {
    carbon_list_remove((CBN_List *) this, idx);
  }

  void ShrinkToFit(void) {
    carbon_list_shrink_to_fit((CBN_List *) this);
  }

  iterator begin(void) const {
    return (iterator) items;
  }

  iterator end(void) const {
    return (iterator) items + size;
  }

  value_type &operator[](usz idx) {
    return carbon_list_at(value_type, *this, idx);
  }
};

#endif
