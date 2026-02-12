// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

#include "../hdrs/constants.h"

int main(void) {
  const auto skap = TUTORIAL_DIR "/assets.skap";
  const auto decl = TUTORIAL_DIR "/assets.d/assets.txt";
  CBN_ASSERT(cbn::SKAP::Create(decl, skap));
}
