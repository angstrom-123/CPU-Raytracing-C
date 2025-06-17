#ifndef RANDOM_H
#define RANDOM_H

// 0 = fast pcg 
// 1 = xoshiro256+
#define PRNG_IDX 1

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * Returns a random floating point number between 0 and 1.
 */
extern double rng_01(void);

/*
 * Seeds the rng with the given number.
 */
extern void rng_set_seed(uint64_t seed);

#endif
