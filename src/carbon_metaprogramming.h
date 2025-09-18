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
  template <usz N, typename... Ts>
  requires (sizeof...(Ts) > 0)
  struct Pick;
  template <usz N, typename... Ts>
  using Pick_t = typename Pick<N, Ts...>::type;
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
  using If_t = typename If<C, T, F>::type;

  /**
   */
  template <template <typename...> typename T, typename L>
  struct Replace;
  template <template <typename...> typename T, typename... Ts>
  struct Replace<T, List<Ts...>> : TID<T<Ts...>> {};
  template <template <typename...> typename T, typename L>
  using Replace_t = typename Replace<T, L>::type;

  /**
   */
  template <template <typename...> typename F, typename L>
  struct Transform;
  template <template <typename...> typename F, typename... Ts>
  struct Transform<F, List<Ts...>> : TID<List<F<Ts>...>> {};
  template <template <typename...> typename F, typename L>
  using Transform_t = typename Transform<F, L>::type;

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
  using Fold_t = typename Fold<F, I, Ts...>::type;
}
#endif  // __cplusplus

// Local Variables:
// mode: c++
// End:
