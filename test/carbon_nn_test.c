// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

CARBON_TEST(carbon_nn, or_gate) {
  usz arch[] = {2, 1, 1};
  CBN_NeuralNet nn = carbon_nn_create(arch, CARBON_ARRAY_LEN(arch));
  carbon_math_srand(69);
  carbon_nn_rand(nn, -1, 1);
  CBN_Matrix train = carbon_math_mat_create(4, 3);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(train, row, 0) = i, CARBON_MAT_AT(train, row, 1) = j;
      CARBON_MAT_AT(train, row, 2) = i | j;
    }
  }
  carbon_nn_fit(nn, 1e6, train, 1);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 0) = i, CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 1) = j;
      carbon_nn_forward(nn);
      carbon_should_be_f(CARBON_MAT_AT(train, row, 2), carbon_math_round(CARBON_MAT_AT(CARBON_NN_OUT(nn), 0, 0)));
    }
  }
  carbon_math_mat_destroy(&train);
  carbon_nn_destroy(&nn);
  return CARBON_OK;
}

CARBON_TEST(carbon_nn, nor_gate) {
  usz arch[] = {2, 1, 1};
  CBN_NeuralNet nn = carbon_nn_create(arch, CARBON_ARRAY_LEN(arch));
  carbon_math_srand(69);
  carbon_nn_rand(nn, -1, 1);
  CBN_Matrix train = carbon_math_mat_create(4, 3);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(train, row, 0) = i, CARBON_MAT_AT(train, row, 1) = j;
      CARBON_MAT_AT(train, row, 2) = !(i | j);
    }
  }
  carbon_nn_fit(nn, 1e6, train, 1);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 0) = i, CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 1) = j;
      carbon_nn_forward(nn);
      carbon_should_be_f(CARBON_MAT_AT(train, row, 2), carbon_math_round(CARBON_MAT_AT(CARBON_NN_OUT(nn), 0, 0)));
    }
  }
  carbon_math_mat_destroy(&train);
  carbon_nn_destroy(&nn);
  return CARBON_OK;
}

CARBON_TEST(carbon_nn, and_gate) {
  usz arch[] = {2, 1, 1};
  CBN_NeuralNet nn = carbon_nn_create(arch, CARBON_ARRAY_LEN(arch));
  carbon_math_srand(69);
  carbon_nn_rand(nn, -1, 1);
  CBN_Matrix train = carbon_math_mat_create(4, 3);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(train, row, 0) = i, CARBON_MAT_AT(train, row, 1) = j;
      CARBON_MAT_AT(train, row, 2) = i & j;
    }
  }
  carbon_nn_fit(nn, 1e6, train, 1);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 0) = i, CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 1) = j;
      carbon_nn_forward(nn);
      carbon_should_be_f(CARBON_MAT_AT(train, row, 2), carbon_math_round(CARBON_MAT_AT(CARBON_NN_OUT(nn), 0, 0)));
    }
  }
  carbon_math_mat_destroy(&train);
  carbon_nn_destroy(&nn);
  return CARBON_OK;
}

CARBON_TEST(carbon_nn, nand_gate) {
  usz arch[] = {2, 1, 1};
  CBN_NeuralNet nn = carbon_nn_create(arch, CARBON_ARRAY_LEN(arch));
  carbon_math_srand(69);
  carbon_nn_rand(nn, -1, 1);
  CBN_Matrix train = carbon_math_mat_create(4, 3);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(train, row, 0) = i, CARBON_MAT_AT(train, row, 1) = j;
      CARBON_MAT_AT(train, row, 2) = !(i & j);
    }
  }
  carbon_nn_fit(nn, 1e6, train, 1);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 0) = i, CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 1) = j;
      carbon_nn_forward(nn);
      carbon_should_be_f(CARBON_MAT_AT(train, row, 2), carbon_math_round(CARBON_MAT_AT(CARBON_NN_OUT(nn), 0, 0)));
    }
  }
  carbon_math_mat_destroy(&train);
  carbon_nn_destroy(&nn);
  return CARBON_OK;
}

CARBON_TEST(carbon_nn, xor_gate) {
  usz arch[] = {2, 2, 1};
  CBN_NeuralNet nn = carbon_nn_create(arch, CARBON_ARRAY_LEN(arch));
  carbon_math_srand(69);
  carbon_nn_rand(nn, -1, 1);
  CBN_Matrix train = carbon_math_mat_create(4, 3);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(train, row, 0) = i, CARBON_MAT_AT(train, row, 1) = j;
      CARBON_MAT_AT(train, row, 2) = i ^ j;
    }
  }
  carbon_nn_fit(nn, 1e6, train, 1);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 0) = i, CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 1) = j;
      carbon_nn_forward(nn);
      carbon_should_be_f(CARBON_MAT_AT(train, row, 2), carbon_math_round(CARBON_MAT_AT(CARBON_NN_OUT(nn), 0, 0)));
    }
  }
  carbon_math_mat_destroy(&train);
  carbon_nn_destroy(&nn);
  return CARBON_OK;
}

CARBON_TEST(carbon_nn, xnor_gate) {
  usz arch[] = {2, 2, 1};
  CBN_NeuralNet nn = carbon_nn_create(arch, CARBON_ARRAY_LEN(arch));
  carbon_math_srand(69);
  carbon_nn_rand(nn, -1, 1);
  CBN_Matrix train = carbon_math_mat_create(4, 3);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(train, row, 0) = i, CARBON_MAT_AT(train, row, 1) = j;
      CARBON_MAT_AT(train, row, 2) = !(i ^ j);
    }
  }
  carbon_nn_fit(nn, 1e6, train, 1);
  for (usz i = 0; i < 2; ++i) {
    for (usz j = 0; j < 2; ++j) {
      usz row = 2*i + j;
      CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 0) = i, CARBON_MAT_AT(CARBON_NN_IN(nn), 0, 1) = j;
      carbon_nn_forward(nn);
      carbon_should_be_f(CARBON_MAT_AT(train, row, 2), carbon_math_round(CARBON_MAT_AT(CARBON_NN_OUT(nn), 0, 0)));
    }
  }
  carbon_math_mat_destroy(&train);
  carbon_nn_destroy(&nn);
  return CARBON_OK;
}
