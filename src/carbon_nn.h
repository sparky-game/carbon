/*
**  $$=======================$$
**  ||       NeuralNet       ||
**  $$=======================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_NN_IN(nn) (CBN_ASSERT((nn).arch_count > 0), (nn).as[0])
#define CARBON_NN_OUT(nn) (CBN_ASSERT((nn).arch_count > 0), (nn).as[(nn).arch_count - 1])
#define CARBON_NN_SIGMOID_DX(x) ((x) * (1 - (x)))
#define CARBON_NN_PRINT(nn) carbon_nn_print(nn, #nn)

typedef struct {
  usz *arch;
  usz arch_count;
  CBN_Matrix *ws;
  CBN_Row *bs;
  CBN_Row *as;
} CBN_NeuralNet;

CARBON_API CBN_NeuralNet carbon_nn_create(usz *arch, usz arch_count);
CARBON_API void carbon_nn_destroy(CBN_NeuralNet *nn);
CARBON_API void carbon_nn_fill(CBN_NeuralNet nn, f32 x);
CARBON_API void carbon_nn_zero(CBN_NeuralNet nn);
CARBON_API void carbon_nn_rand(CBN_NeuralNet nn, f32 min, f32 max);
CARBON_API void carbon_nn_rand(CBN_NeuralNet nn, f32 min, f32 max);
CARBON_API void carbon_nn_forward(CBN_NeuralNet nn);
CARBON_API f32 carbon_nn_cost(CBN_NeuralNet nn, CBN_Matrix m);
CARBON_API CBN_NeuralNet carbon_nn_backprop(CBN_NeuralNet nn, CBN_Matrix m);
CARBON_API void carbon_nn_learn(CBN_NeuralNet nn, CBN_NeuralNet g, f32 lr);
CARBON_API void carbon_nn_fit(CBN_NeuralNet nn, usz iters, CBN_Matrix train, f32 lr);
CARBON_API void carbon_nn_print(CBN_NeuralNet nn, const char *name);
