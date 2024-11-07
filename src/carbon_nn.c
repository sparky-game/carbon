// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#ifndef CARBON_IMPLEMENTATION
#include <carbon.h>
#endif  // CARBON_IMPLEMENTATION

CBN_NeuralNet carbon_nn_create(usz *arch, usz arch_count) {
  CARBON_ASSERT(arch_count > 0);
  CBN_NeuralNet nn = {
    .arch = arch,
    .arch_count = arch_count,
    .ws = (CBN_Matrix *) CARBON_MALLOC((arch_count - 1) * sizeof(CBN_Matrix)),
    .bs = (CBN_Row *) CARBON_MALLOC((arch_count - 1) * sizeof(CBN_Row)),
    .as = (CBN_Row *) CARBON_MALLOC(arch_count * sizeof(CBN_Row))
  };
  if (!nn.ws || !nn.bs || !nn.as) {
    CARBON_ERROR("failed to allocate memory");
    memset(&nn, 0, sizeof(CBN_NeuralNet));
  }
  return nn;
}

CARBON_API void carbon_nn_fill(CBN_NeuralNet nn, f32 x) {
  for (usz i = 0; i < nn.arch_count - 1; ++i) {
    carbon_math_mat_fill(nn.ws[i], x);
    carbon_math_row_fill(nn.bs[i], x);
    carbon_math_row_fill(nn.as[i], x);
  }
  carbon_math_row_fill(nn.as[nn.arch_count - 1], x);
}

CARBON_API void carbon_nn_zero(CBN_NeuralNet nn) {
  carbon_nn_fill(nn, 0);
}

CARBON_API void carbon_nn_rand(CBN_NeuralNet nn, f32 min, f32 max) {
  for (usz i = 0; i < nn.arch_count - 1; ++i) {
    carbon_math_mat_rand(nn.ws[i], min, max);
    carbon_math_row_rand(nn.bs[i], min, max);
  }
}
