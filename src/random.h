#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

extern double rng_01_fpcg(void);
extern void rng_set_seed(uint64_t seed);

#endif
