// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

using Types = cbn::meta::List<int, float, double, float, float, char>;

namespace List {
  static_assert(Types::Count() == 6);
  static_assert(Types::Contains<double>() and not Types::Contains<void>());
  static_assert(Types::Find<double>() == 2 and Types::Find<char>() == Types::Count() - 1);
}

namespace Replace {
  using Result = cbn::meta::Replace_t<cbn::Tuple, Types>;
  using Expected = cbn::Tuple<int, float, double, float, float, char>;
  static_assert(cbn::meta::Same_v<Result, Expected>);
}

namespace Transform {
  template <typename T>
  using AddPtr = T*;

  using Result = cbn::meta::Transform_t<AddPtr, Types>;
  using Expected = cbn::meta::List<int*, float*, double*, float*, float*, char*>;
  static_assert(cbn::meta::Same_v<Result, Expected>);
}

namespace Fold {
  template <typename Counter, typename T>
  struct CountFloats : cbn::meta::If<cbn::meta::Same_v<T, float>,
                                     cbn::meta::Constant<u8, Counter::value + 1>,
                                     Counter> {};

  constexpr auto result = cbn::meta::Fold_t<CountFloats, cbn::meta::Constant<u8, 0>, Types>::value;
  static_assert(result == 3);
}
