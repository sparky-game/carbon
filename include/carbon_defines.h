#pragma once

#ifdef CARBON_IMPLEMENTATION
#define CARBON_API inline
#else
#define CARBON_API
#endif  // CARBON_IMPLEMENTATION

#define false 0
#define true 1

typedef _Bool bool_t;
