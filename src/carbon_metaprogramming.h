/*
**  $$=============================$$
**  ||       Metaprogramming       ||
**  $$=============================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#ifdef __cplusplus
namespace cbn::meta {
  /**
   */
  template <typename T, T N>
  struct Constant { static constexpr T value {N}; };

  /**
   */
  struct True : Constant<bool, true> {};

  /**
   */
  struct False : Constant<bool, false> {};

  /**
   */
  template <typename... Ts>
  struct AlwaysFalse : False {};
  template <typename... Ts>
  constexpr auto AlwaysFalse_v = AlwaysFalse<Ts...>::value;

  /**
   */
  template <typename T, typename U>
  struct Same : False {};
  template <typename T>
  struct Same<T, T> : True {};
  template <typename T, typename U>
  constexpr auto Same_v = Same<T, U>::value;

  /**
   */
  template <typename T, typename U>
  concept SameAs = Same_v<T, U> and Same_v<U, T>;

  /**
   */
  template <typename T>
  struct IsInt : Constant<bool, requires (T t, T *p, void (*f)(T)) {
    reinterpret_cast<T>(t);
    f(0);
    p + t;
  }> {};
  template <typename T>
  constexpr auto IsInt_v = IsInt<T>::value;

  /**
   */
  template <typename T>
  struct IsFloat : Constant<bool, requires (T t) {
    {t + 0.5f} -> SameAs<T>;
    {t / 2} -> SameAs<T>;
  }> {};
  template <typename T>
  constexpr auto IsFloat_v = IsFloat<T>::value;

  /**
   */
  template <typename T>
  concept Int = IsInt_v<T>;

  /**
   */
  template <typename T>
  concept Float = IsFloat_v<T>;

  /**
   */
  template <typename T>
  concept Numeric = Int<T> or Float<T>;

  /**
   */
  template <typename T, typename... Ts>
  concept Allowed = (SameAs<T, Ts> or ...);

  /**
   */
  template <typename T>
  struct TID { using type = T; };

  /**
   */
  template <typename T>
  struct IsConst : False {};
  template <typename T>
  struct IsConst<const T> : True {};
  template <typename T>
  constexpr auto IsConst_v = IsConst<T>::value;

  /**
   */
  template <typename T>
  struct RemoveConst : TID<T> {};
  template <typename T>
  struct RemoveConst<const T> : TID<T> {};
  template <typename T>
  using RemoveConst_t = RemoveConst<T>::type;

  /**
   */
  template <typename T>
  struct IsVolatile : False {};
  template <typename T>
  struct IsVolatile<volatile T> : True {};
  template <typename T>
  constexpr auto IsVolatile_v = IsVolatile<T>::value;

  /**
   */
  template <typename T>
  struct RemoveVolatile : TID<T> {};
  template <typename T>
  struct RemoveVolatile<volatile T> : TID<T> {};
  template <typename T>
  using RemoveVolatile_t = RemoveVolatile<T>::type;

  /**
   */
  template <typename T>
  struct RemoveCV : RemoveConst<RemoveVolatile_t<T>> {};
  template <typename T>
  using RemoveCV_t = RemoveCV<T>::type;

  /**
   */
  template <typename T>
  struct IsRef : False {};
  template <typename T>
  struct IsRef<T &> : True {};
  template <typename T>
  struct IsRef<T &&> : True {};
  template <typename T>
  constexpr auto IsRef_v = IsRef<T>::value;

  /**
   */
  template <typename T>
  struct IsLVRef : False {};
  template <typename T>
  struct IsLVRef<T &> : True {};
  template <typename T>
  constexpr auto IsLVRef_v = IsLVRef<T>::value;

  /**
   */
  template <typename T>
  struct IsRVRef : False {};
  template <typename T>
  struct IsRVRef<T &&> : True {};
  template <typename T>
  constexpr auto IsRVRef_v = IsRVRef<T>::value;

  /**
   */
  template <typename T>
  auto AddLVRef__try(char) -> TID<T &>;
  template <typename T>
  auto AddLVRef__try(...) -> TID<T>;
  template <typename T>
  struct AddLVRef : typeof(AddLVRef__try<T>(0)) {};
  template <typename T>
  using AddLVRef_t = AddLVRef<T>::type;

  /**
   */
  template <typename T>
  auto AddRVRef__try(char) -> TID<T &&>;
  template <typename T>
  auto AddRVRef__try(...) -> TID<T>;
  template <typename T>
  struct AddRVRef : typeof(AddRVRef__try<T>(0)) {};
  template <typename T>
  using AddRVRef_t = AddRVRef<T>::type;

  /**
   */
  template <typename T>
  struct RemoveRef : TID<T> {};
  template <typename T>
  struct RemoveRef<T &> : TID<T> {};
  template <typename T>
  struct RemoveRef<T &&> : TID<T> {};
  template <typename T>
  using RemoveRef_t = RemoveRef<T>::type;

  /**
   */
  template <typename T>
  struct RemoveCVRef : RemoveCV<RemoveRef_t<T>> {};
  template <typename T>
  using RemoveCVRef_t = RemoveCVRef<T>::type;

  /**
   */
  template <typename T>
  auto AddPtr__try(char) -> TID<RemoveRef_t<T> *>;
  template <typename T>
  auto AddPtr__try(...) -> TID<T>;
  template <typename T>
  struct AddPtr : typeof(AddPtr__try<T>(0)) {};
  template <typename T>
  using AddPtr_t = AddPtr<T>::type;

  /**
   */
  template <typename T>
  struct IsArray : False {};
  template <typename T>
  struct IsArray<T[]> : True {};
  template <typename T, usz N>
  struct IsArray<T[N]> : True {};
  template <typename T>
  constexpr auto IsArray_v = IsArray<T>::value;

  // TODO: implement Rank (same as std::rank)
  // TODO: implement Extent (same as std::extent)

  /**
   * @brief Removes the first dimension of an array type.
   *
   * If T is an array of type X, the resultant type would be X, otherwise T is returned.
   * If T is a ɣ-dimensional array of type X (X[][N]..{ɣ-1}..[M]), the resultant type would
   * be of an array of ɣ-1 dimensions (X[N]..{ɣ-1}..[M]), the first of which has been discarded.
   *
   * @param T The type.
   * @return The resultant type.
   */
  template <typename T>
  struct RemoveExtent : TID<T> {};
  template <typename T>
  struct RemoveExtent<T[]> : TID<T> {};
  template <typename T, usz N>
  struct RemoveExtent<T[N]> : TID<T> {};
  template <typename T>
  using RemoveExtent_t = RemoveExtent<T>::type;

  /**
   * @brief Removes all dimensions of an array type.
   *
   * If T is an array of type X, the resultant type would be X, otherwise T is returned.
   * If T is a ɣ-dimensional array of type X (X[][N]..{ɣ-1}..[M]), the resultant type would
   * be X, where all of the ɣ array dimensions have been discarded.
   *
   * @param T The type.
   * @return The resultant type.
   */
  template <typename T>
  struct RemoveAllExtents : TID<T> {};
  template <typename T>
  using RemoveAllExtents_t = RemoveAllExtents<T>::type;
  template <typename T>
  struct RemoveAllExtents<T[]> : TID<RemoveAllExtents_t<T>> {};
  template <typename T, usz N>
  struct RemoveAllExtents<T[N]> : TID<RemoveAllExtents_t<T>> {};

  /**
   */
  template <typename T>
  struct IsFunc : Constant<bool, !IsConst_v<const T> and !IsRef_v<T>> {};
  template <typename T>
  constexpr auto IsFunc_v = IsFunc<T>::value;

  /**
   */
  template <typename T>
  constexpr T &&Forward(RemoveRef_t<T> &t) noexcept { return static_cast<T &&>(t); }
  template <typename T>
  constexpr T &&Forward(RemoveRef_t<T> &&t) noexcept = delete;

  /**
   */
  template <usz N, typename... Ts>
  requires (sizeof...(Ts) > 0)
  struct Pick;
  template <usz N, typename... Ts>
  using Pick_t = Pick<N, Ts...>::type;
  template <usz N, typename T, typename... Ts>
  struct Pick<N, T, Ts...> : TID<Pick_t<N-1, Ts...>> {};
  template <typename T, typename... Ts>
  struct Pick<0, T, Ts...> : TID<T> {};

  /**
   */
  template <typename T, typename... Ts>
  requires (sizeof...(Ts) > 0)
  struct Where;
  template <typename T, typename... Ts>
  constexpr auto Where_v = Where<T, Ts...>::value;
  template <typename T, typename U, typename... Ts>
  struct Where<T, U, Ts...> : Constant<usz, 1 + Where_v<T, Ts...>> {};
  template <typename T, typename... Ts>
  struct Where<T, T, Ts...> : Constant<usz, 0> {};

  /**
   */
  template <typename... Ts>
  struct List {
    /**
     */
    static consteval auto Count(void) noexcept { return sizeof...(Ts); }
    /**
     */
    template <typename T>
    static consteval auto Contains(void) noexcept { return (false or ... or Same_v<T, Ts>); }
    /**
     */
    template <typename T>
    requires (Contains<T>())
    static consteval auto Find(void) noexcept { return Where_v<T, Ts...>; }
  };

  /**
   */
  template <usz N>
  struct String {
    char value[N] {};
    /**
     */
    consteval String(const char (&s)[N]) noexcept {
      for (usz i = 0; i < N; ++i) value[i] = s[i];
    }
    /**
     */
    consteval auto Size(void) const noexcept { return N; }
    /**
     */
    template <usz M>
    consteval bool operator==(const String<M> &s) const noexcept {
      for (usz i = 0; i < N; ++i) {
        if (value[i] != s.value[i]) return false;
      }
      return true;
    }
    /**
     */
    template <usz M>
    consteval bool operator==(const char (&s)[M]) const noexcept { return *this == String<M>(s); }
  };

  /**
   */
  template <bool C, typename T, typename F>
  struct If : TID<F> {};
  template <typename T, typename F>
  struct If<true, T, F> : TID<T> {};
  template <bool C, typename T, typename F>
  using If_t = If<C, T, F>::type;

  /**
   */
  template <template <typename...> typename T, typename L>
  struct Replace;
  template <template <typename...> typename T, typename... Ts>
  struct Replace<T, List<Ts...>> : TID<T<Ts...>> {};
  template <template <typename...> typename T, typename L>
  using Replace_t = Replace<T, L>::type;

  /**
   */
  template <template <typename...> typename F, typename L>
  struct Transform;
  template <template <typename...> typename F, typename... Ts>
  struct Transform<F, List<Ts...>> : TID<List<F<Ts>...>> {};
  template <template <typename...> typename F, typename L>
  using Transform_t = Transform<F, L>::type;

  /**
   */
  template <template <typename, typename> typename F, typename I, typename... Ts>
  struct Fold;
  template <template <typename, typename> typename F, typename I, typename... Ts>
  struct Fold<F, I, List<Ts...>> : Fold<F, I, Ts...> {};
  template <template <typename, typename> typename F, typename I, typename T, typename... Ts>
  struct Fold<F, I, T, Ts...> : Fold<F, typename F<I, T>::type, Ts...> {};
  template <template <typename, typename> typename F, typename I>
  struct Fold<F, I> : TID<I> {};
  template <template <typename, typename> typename F, typename I, typename... Ts>
  using Fold_t = Fold<F, I, Ts...>::type;

  /**
   */
  template <typename T>
  struct Decay : If<IsArray_v<RemoveRef_t<T>>,
                    AddPtr_t<RemoveExtent_t<RemoveRef_t<T>>>,
                    If_t<IsFunc_v<RemoveRef_t<T>>,
                         AddPtr_t<RemoveRef_t<T>>,
                         RemoveCV_t<RemoveRef_t<T>>>> {};
  template <typename T>
  using Decay_t = Decay<T>::type;
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
