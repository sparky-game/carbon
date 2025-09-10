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

namespace List {
  static_assert(Types::Count() == 6);
  static_assert(Types::Contains<f64>() and not Types::Contains<void>());
  static_assert(Types::Find<f64>() == 2 and Types::Find<char>() == Types::Count() - 1);
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
