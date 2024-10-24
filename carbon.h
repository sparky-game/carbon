/*
**  $$=================================================================================$$
**  ||                                                                                 ||
**  ||               ------                                                            ||
**  ||            ------==+                                                            ||
**  ||         ------=+++++                     _____            _                     ||
**  ||      ------=+++++                       /  __ \          | |                    ||
**  ||    =----=++++=           ------         | /  \/ __ _ _ __| |__   ___  _ __      ||
**  ||    ***++++            ------=++         | |    / _` | '__| '_ \ / _ \| '_ \     ||
**  ||    ***+++          ------=+++++         | \__/\ (_| | |  | |_) | (_) | | | |    ||
**  ||    ***+++      ------==++++**++          \____/\__,_|_|  |_.__/ \___/|_| |_|    ||
**  ||    ***+++     ++--==+++++****++                                                 ||
**  ||    ***+++     ***+++++   ****++                 The most lightweight            ||
**  ||    ***+++      #*++      ****++               C/C++ testing framework           ||
**  ||    ***+++                ****++               written in pure C (C99)           ||
**  ||    **+----             :---=+++                                                 ||
**  ||    *****=-----      ------=++++             Licensed under BSD-3-Clause         ||
**  ||      #*****=-----------=+++++                                                   ||
**  ||         ******=-----=+++++                        Wasym A. Alonso               ||
**  ||            ******=+++++                                                         ||
**  ||               ***+++                          wasymatieh01@gmail.com            ||
**  ||                                                                                 ||
**  ||                                        https://github.com/sparky-game/carbon    ||
**  ||                                                                                 ||
**  ||                                                                                 ||
**  $$=================================================================================$$
*/

// SPDX-License-Identifier: BSD-3-Clause
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

#define CARBON_NAME "SPARKY Carbon"
#define CARBON_VERSION "v0.6-alpha"

#define CARBON_MALLOC(sz)        malloc(sz)
#define CARBON_REALLOC(p, newsz) realloc(p, newsz)
#define CARBON_FREE(p)           free(p)

#ifdef __cplusplus
extern "C" {
#endif

#include "src/carbon_defines.h"
#include "src/carbon_log.h"
#include "src/carbon_fs.h"
#include "src/carbon_main.h"
#include "src/carbon_time.h"
#include "src/carbon_junit.h"
#include "src/carbon_clock.h"
#include "src/carbon_should.h"
#include "src/carbon_uniquelist.h"
#include "src/carbon_test_manager.h"

#ifdef __cplusplus
}
#endif
