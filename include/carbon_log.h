#pragma once

#include <stdio.h>

#define CARBON_INFO(msg, ...) printf(msg, ##__VA_ARGS__)
#define CARBON_ERROR(msg, ...) fprintf(stderr, msg, ##__VA_ARGS__)
