// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include "carbon.inc"

CBN_NeuralNet carbon_nn_create(usz *arch, usz arch_count) {
  CBN_ASSERT(arch_count > 0);
  CBN_NeuralNet nn = {
    .arch = arch,
    .arch_count = arch_count,
    .ws = (CBN_Matrix *) CBN_MALLOC((arch_count - 1) * sizeof(CBN_Matrix)),
    .bs = (CBN_Row *) CBN_MALLOC((arch_count - 1) * sizeof(CBN_Row)),
    .as = (CBN_Row *) CBN_MALLOC(arch_count * sizeof(CBN_Row))
  };
  CBN_ASSERT(nn.ws && nn.bs && nn.as && "failed to allocate memory");
  nn.as[0] = carbon_math_row_create(nn.arch[0]);
  for (usz i = 1; i < nn.arch_count; ++i) {
    nn.ws[i - 1] = carbon_math_mat_create(nn.as[i - 1].cols, nn.arch[i]);
    nn.bs[i - 1] = carbon_math_row_create(nn.arch[i]);
    nn.as[i]     = carbon_math_row_create(nn.arch[i]);
  }
  return nn;
}

void carbon_nn_destroy(CBN_NeuralNet *nn) {
  if (!nn) {
    CBN_WARN("`nn` is not a valid pointer, skipping destruction");
    return;
  }
  carbon_math_row_destroy(&nn->as[0]);
  for (usz i = 1; i < nn->arch_count; ++i) {
    carbon_math_mat_destroy(&nn->ws[i - 1]);
    carbon_math_row_destroy(&nn->bs[i - 1]);
    carbon_math_row_destroy(&nn->as[i]);
  }
  CBN_FREE(nn->ws);
  CBN_FREE(nn->bs);
  CBN_FREE(nn->as);
  carbon_memory_set(nn, 0, sizeof(*nn));
}

void carbon_nn_fill(CBN_NeuralNet nn, f32 x) {
  for (usz i = 0; i < nn.arch_count - 1; ++i) {
    carbon_math_mat_fill(nn.ws[i], x);
    carbon_math_row_fill(nn.bs[i], x);
    carbon_math_row_fill(nn.as[i], x);
  }
  carbon_math_row_fill(nn.as[nn.arch_count - 1], x);
}

void carbon_nn_zero(CBN_NeuralNet nn) {
  carbon_nn_fill(nn, 0);
}

void carbon_nn_rand(CBN_NeuralNet nn, f32 min, f32 max) {
  for (usz i = 0; i < nn.arch_count - 1; ++i) {
    carbon_math_mat_rand(nn.ws[i], min, max);
    carbon_math_row_rand(nn.bs[i], min, max);
  }
}

void carbon_nn_forward(CBN_NeuralNet nn) {
  for (usz i = 0; i < nn.arch_count - 1; ++i) {
    carbon_math_mat_dot(carbon_math_row_to_mat(nn.as[i + 1]), carbon_math_row_to_mat(nn.as[i]), nn.ws[i]);
    carbon_math_mat_add(carbon_math_row_to_mat(nn.as[i + 1]), carbon_math_row_to_mat(nn.bs[i]));
    carbon_math_mat_map(carbon_math_row_to_mat(nn.as[i + 1]), carbon_math_sigmoid);
  }
}

f32 carbon_nn_cost(CBN_NeuralNet nn, CBN_Matrix m) {
  CBN_ASSERT(CARBON_NN_IN(nn).cols + CARBON_NN_OUT(nn).cols == m.cols);
  f32 c = 0;
  for (usz i = 0; i < m.rows; ++i) {
    CBN_Row row = carbon_math_mat_row(m, i);
    CBN_Row x = carbon_math_row_slice(row, 0, CARBON_NN_IN(nn).cols);
    CBN_Row y = carbon_math_row_slice(row, CARBON_NN_IN(nn).cols, CARBON_NN_OUT(nn).cols);
    carbon_math_row_copy(CARBON_NN_IN(nn), x);
    carbon_nn_forward(nn);
    for (usz j = 0; j < y.cols; ++j) {
      f32 d = CARBON_ROW_AT(CARBON_NN_OUT(nn), j) - CARBON_ROW_AT(y, j);
      c += d * d;
    }
  }
  return c / m.rows;
}

CBN_NeuralNet carbon_nn_backprop(CBN_NeuralNet nn, CBN_Matrix m) {
  CBN_ASSERT(CARBON_NN_IN(nn).cols + CARBON_NN_OUT(nn).cols == m.cols);
  CBN_NeuralNet g = carbon_nn_create(nn.arch, nn.arch_count);
  carbon_nn_zero(g);
  for (usz i = 0; i < m.rows; ++i) {
    CBN_Row row = carbon_math_mat_row(m, i);
    CBN_Row x = carbon_math_row_slice(row, 0, CARBON_NN_IN(nn).cols);
    CBN_Row y = carbon_math_row_slice(row, CARBON_NN_IN(nn).cols, CARBON_NN_OUT(nn).cols);
    carbon_math_row_copy(CARBON_NN_IN(nn), x);
    carbon_nn_forward(nn);
    for (usz j = 0; j < nn.arch_count; ++j) {
      carbon_math_row_fill(g.as[j], 0);
    }
    for (usz j = 0; j < y.cols; ++j) {
      CARBON_ROW_AT(CARBON_NN_OUT(g), j) = CARBON_ROW_AT(CARBON_NN_OUT(nn), j) - CARBON_ROW_AT(y, j);
    }
    for (usz l = nn.arch_count - 1; l > 0; --l) {
      for (usz j = 0; j < nn.as[l].cols; ++j) {
        f32 a = CARBON_ROW_AT(nn.as[l], j);
        f32 da = CARBON_ROW_AT(g.as[l], j);
        f32 qa = CARBON_NN_SIGMOID_DX(a);
        CARBON_ROW_AT(g.bs[l - 1], j) += 2 * da * qa;
        for (usz k = 0; k < nn.as[l - 1].cols; ++k) {
          f32 pa = CARBON_ROW_AT(nn.as[l - 1], k);
          f32 w = CARBON_MAT_AT(nn.ws[l - 1], k, j);
          CARBON_MAT_AT(g.ws[l - 1], k, j) += 2 * da * qa * pa;
          CARBON_ROW_AT(g.as[l - 1], k) += 2 * da * qa * w;
        }
      }
    }
  }
  for (usz i = 0; i < g.arch_count - 1; ++i) {
    for (usz j = 0; j < g.ws[i].rows; ++j) {
      for (usz k = 0; k < g.ws[i].cols; ++k) {
        CARBON_MAT_AT(g.ws[i], j, k) /= m.rows;
      }
    }
    for (usz j = 0; j < g.bs[i].cols; ++j) {
      CARBON_ROW_AT(g.bs[i], j) /= m.rows;
    }
  }
  return g;
}

void carbon_nn_learn(CBN_NeuralNet nn, CBN_NeuralNet g, f32 lr) {
  for (usz i = 0; i < nn.arch_count - 1; ++i) {
    for (usz j = 0; j < nn.ws[i].rows; ++j) {
      for (usz k = 0; k < nn.ws[i].cols; ++k) {
        CARBON_MAT_AT(nn.ws[i], j, k) -= lr * CARBON_MAT_AT(g.ws[i], j, k);
      }
    }
    for (usz j = 0; j < nn.bs[i].cols; ++j) {
      CARBON_ROW_AT(nn.bs[i], j) -= lr * CARBON_ROW_AT(g.bs[i], j);
    }
  }
}

void carbon_nn_fit(CBN_NeuralNet nn, usz iters, CBN_Matrix train, f32 lr) {
  for (usz i = 0; i < iters; ++i) {
    CBN_NeuralNet g = carbon_nn_backprop(nn, train);
    carbon_nn_learn(nn, g, lr);
    carbon_nn_destroy(&g);
  }
}

void carbon_nn_print(CBN_NeuralNet nn, const char *name) {
  carbon_println("%s = [", name);
  for (usz i = 0; i < nn.arch_count - 1; ++i) {
    carbon_math_mat_print(nn.ws[i], carbon_string_fmt("ws%zu", i));
    carbon_math_row_print(nn.bs[i], carbon_string_fmt("bs%zu", i));
  }
  carbon_println("];");
}
