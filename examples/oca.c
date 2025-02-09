// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#define CARBON_NO_TESTING
#define CARBON_IMPLEMENTATION
#include "../carbon.h"

static const char *instructions =
  "/*-------------------------------------------------------------------------------------------.\n"
  "| Al empezar, el q̃ saca tres y seis, se pone al num. 26. y si es quatro y cinco, al num. 53. |\n"
  "| El q̃ da en una oca, vuela hasta no dar en otra.                                            |\n"
  "| En el 6. paga, y se pasa al 12.                                                            |\n"
  "| En la venta paga, y no juega una vez.                                                      |\n"
  "| En el pozo paga, y no sale hasta q̃ otro le saque. En el laberinto paga, y vuelve al 39.    |\n"
  "| En la torre paga, y no juega tres veces.                                                   |\n"
  "| En la muerte, se vuelve à empezar.                                                         |\n"
  "| Desde el 60. con un dado, volando del 63. atrás, hasta venir justo, y se gana.             |\n"
  "`-------------------------------------------------------------------------------------------*/\n";

static const u8 oca_mappings[] = {
  [5]  = 9,  [9]  = 14,
  [14] = 18, [18] = 23,
  [23] = 27, [27] = 32,
  [32] = 36, [36] = 41,
  [41] = 45, [45] = 50,
  [50] = 54, [54] = 59,
  [59] = 63
};

static u8 roll_die(void) {
  return carbon_math_rand_between(1, 6);
}

static void print_board(void) {
  static u8 board[11][8] = {
    {25, 24, 23, 22, 21, 20, 19, 18},
    {26, 53, 52, 51, 50, 49, 48, 17},
    {27, 54,  0,  0,  0,  0, 47, 16},
    {28, 55,  0,  0,  0,  0, 46, 15},
    {29, 56,  0,  0,  0,  0, 45, 14},
    {30, 57,  0,  0,  0,  0, 44, 13},
    {31, 58,  0,  0,  0,  0, 43, 12},
    {32, 60,  0,  0,  0,  0, 42, 11},
    {33, 61, 62, 63,  0,  0, 41, 10},
    {34, 35, 36, 37, 38, 39, 40,  9},
    { 1,  2,  3,  4,  5,  6,  7,  8},
  };
  carbon_print("%$", $(instructions));
  for (u8 i = 0; i < 11; ++i) {
    for (u8 j = 0; j < 8; ++j) {
      if (!board[i][j]) carbon_print("      ");
      else carbon_print("+----+");
    }
    carbon_println("");
    for (u8 j = 0; j < 8; ++j) {
      if (!board[i][j]) carbon_print("      ");
      else carbon_print("| %02hhu |", board[i][j]);
    }
    carbon_println("");
    for (u8 j = 0; j < 8; ++j) {
      if (!board[i][j]) carbon_print("      ");
      else carbon_print("+----+");
    }
    carbon_println("");
  }
}

int main(void) {
  carbon_math_srand(carbon_time_get());
  print_board();
  usz t = 1;
  u8 pos = 1;

  for (;;) {
    carbon_print("[%$] Estoy en el num. %$...", $(t), $(pos));
    getchar();

    u8 d1 = roll_die();
    u8 d2 = pos >= 60 ? 0 : roll_die();
    u8 ds = d1 + d2;

    if (pos == 1) {
      if ((d1 == 3 && d2 == 6) || (d1 == 6 && d2 == 3)) pos = 26;
      else if ((d1 == 4 && d2 == 5) || (d1 == 5 && d2 == 4)) pos = 53;
      else pos += ds;
    }
    else pos += ds;

    if (pos > 63) pos = 2*63 - pos;

    if (!d2) carbon_println("    %$ en el dado, vuelo hasta el num. %$.", $(d1), $(pos));
    else carbon_println("    %$ y %$ en los dados (%$), vuelo hasta el num. %$.", $(d1), $(d2), $(ds), $(pos));

    // Oca
    if (pos < CARBON_ARRAY_LEN(oca_mappings) && oca_mappings[pos]) {
      pos = oca_mappings[pos];
      carbon_println("    De oca a oca y tiro porque me toca! Vuelo hasta el num. %$.", $(pos));
    }

    // Muerte
    if (pos == 58) {
      pos = 1;
      carbon_println("    Oh, no! Caíste a los pies de la muerte... A volver a empezar.");
    }

    // Jardín de la Oca
    if (pos == 63) {
      carbon_println("    Has llegado al Jardín de la Oca! Tu ganas!");
      break;
    }

    ++t;
  }
  return 0;
}
