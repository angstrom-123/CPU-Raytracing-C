#include "random.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * PRIVATE:
 */

static uint64_t state = 0xcafe00dd15ea5e5l;
static uint64_t const mult = 6364136223846793005;
static bool seeded = false;

static uint32_t rng_fast_pcg(void)
{
	uint64_t x = state;
	uint64_t count = x >> 61;
	state = x * mult;
	x ^= x >> 22;
	return (uint32_t) (x >> (22 + count));
}

/*
 * PUBLIC:
 */

double rng_01_fpcg(void)
{
	if (!seeded) 
	{
		fprintf(stderr, "\nRNG must be seeded before use\n");
		exit(1);
	}
	uint64_t as_u64 = 0x3FF0000000000000 | ((((uint64_t) rng_fast_pcg()) << 1) | 1) << 19;
	return *(double*) &as_u64 - 1.0; // force bits of u64 to be interpreted as double
	
}

void rng_set_seed(uint64_t seed)
{
	state = 2 * seed + 1; // seed must be odd
	seeded = true;
}
