#ifndef RANDOM_H
#define RANDOM_H

// fast pcg
// #define PRNG_IDX 0
// xoshiro256+
#define PRNG_IDX 1

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern double rng_01(void);
extern void rng_set_seed(uint64_t seed);

#endif
