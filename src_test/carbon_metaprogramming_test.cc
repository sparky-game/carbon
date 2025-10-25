// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

using Types = cbn::meta::List<i32, f32, f64, f32, f32, char>;

namespace IsInt {
  static_assert(!cbn::meta::IsInt_v<u0> and !cbn::meta::IsInt_v<u0 *>);
  static_assert(cbn::meta::IsInt_v<bool>);
  static_assert(cbn::meta::IsInt_v<char>);
  static_assert(cbn::meta::IsInt_v<u8> and cbn::meta::IsInt_v<i8>);
  static_assert(cbn::meta::IsInt_v<u16> and cbn::meta::IsInt_v<i16>);
  static_assert(cbn::meta::IsInt_v<u32> and cbn::meta::IsInt_v<i32>);
  static_assert(cbn::meta::IsInt_v<u64> and cbn::meta::IsInt_v<i64>);
  static_assert(!cbn::meta::IsInt_v<f32> and !cbn::meta::IsInt_v<f64>);
  static_assert(!cbn::meta::IsInt_v<flong>);
}

namespace IsFloat {
  static_assert(!cbn::meta::IsFloat_v<u0> and !cbn::meta::IsFloat_v<u0 *>);
  static_assert(!cbn::meta::IsFloat_v<bool>);
  static_assert(!cbn::meta::IsFloat_v<char>);
  static_assert(!cbn::meta::IsFloat_v<u8> and !cbn::meta::IsFloat_v<i8>);
  static_assert(!cbn::meta::IsFloat_v<u16> and !cbn::meta::IsFloat_v<i16>);
  static_assert(!cbn::meta::IsFloat_v<u32> and !cbn::meta::IsFloat_v<i32>);
  static_assert(!cbn::meta::IsFloat_v<u64> and !cbn::meta::IsFloat_v<i64>);
  static_assert(cbn::meta::IsFloat_v<f32> and cbn::meta::IsFloat_v<f64>);
  static_assert(cbn::meta::IsFloat_v<flong>);
}

namespace AddRef {
  using T = i32;
  static_assert(!cbn::meta::IsRef_v<T>   and
                !cbn::meta::IsLVRef_v<T> and
                !cbn::meta::IsRVRef_v<T>);
  using LT = cbn::meta::AddLVRef_t<T>;
  static_assert(cbn::meta::IsRef_v<LT>   and
                cbn::meta::IsLVRef_v<LT> and
                !cbn::meta::IsRVRef_v<LT>);
  using RT = cbn::meta::AddRVRef_t<T>;
  static_assert(cbn::meta::IsRef_v<RT>    and
                !cbn::meta::IsLVRef_v<RT> and
                cbn::meta::IsRVRef_v<RT>);
  using VR = cbn::meta::AddLVRef_t<u0>;
  static_assert(!cbn::meta::IsRef_v<VR>   and
                !cbn::meta::IsLVRef_v<VR> and
                !cbn::meta::IsRVRef_v<VR>);
}

namespace RemoveRef {
  static_assert(cbn::meta::Same_v<cbn::meta::RemoveRef_t<i32>, i32>);
  static_assert(cbn::meta::Same_v<cbn::meta::RemoveRef_t<i32 &>, i32>);
  static_assert(cbn::meta::Same_v<cbn::meta::RemoveRef_t<i32 &&>, i32>);
}

namespace List {
  static_assert(Types::Count() == 6);
  static_assert(Types::Contains<f64>() and not Types::Contains<u0>());
  static_assert(Types::Find<f64>() == 2 and Types::Find<char>() == Types::Count() - 1);
}

namespace String {
  constexpr cbn::meta::String str1 = "Hello, World!";
  static_assert(str1 == "Hello, World!");
  static_assert(str1.Size() == 14);
  static_assert(str1 != "Привет, Мир!");
  static_assert(str1 == str1);
  constexpr cbn::meta::String str2 = "Hello, Programmer!";
  static_assert(str2 == "Hello, Programmer!");
  static_assert(str2.Size() == 19);
  static_assert(str2 != "Привет, Программист!");
  static_assert(str2 != str1);
}

namespace Replace {
  using Result = cbn::meta::Replace_t<cbn::Tuple, Types>;
  using Expected = cbn::Tuple<i32, f32, f64, f32, f32, char>;
  static_assert(cbn::meta::Same_v<Result, Expected>);
}

namespace Transform {
  template <typename T>
  using AddPtr = T*;

  using Result = cbn::meta::Transform_t<AddPtr, Types>;
  using Expected = cbn::meta::List<i32*, f32*, f64*, f32*, f32*, char*>;
  static_assert(cbn::meta::Same_v<Result, Expected>);
}

namespace Fold {
  template <typename Counter, typename T>
  struct CountFloats : cbn::meta::If<cbn::meta::Same_v<T, f32>,
                                     cbn::meta::Constant<u8, Counter::value + 1>,
                                     Counter> {};

  constexpr auto result = cbn::meta::Fold_t<CountFloats, cbn::meta::Constant<u8, 0>, Types>::value;
  static_assert(result == 3);
}
