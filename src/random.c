#include "random.h"

#if PRNG_IDX == 0 // fast pcg

/*
 * PRIVATE:
 */

static uint64_t state = 0xCAFE00DD15EA5E5l;
static uint64_t const mult = 6364136223846793005;

static double u32_to_double_01(uint32_t val)
{
	uint64_t u64 = 0x3FF0000000000000 | (((((uint64_t) val) << 1) | 1) << 19);
	return *(double*) &u64 - 1.0; // force bits of u64 to be interpreted as double
}

/*
 * PUBLIC:
 */

void rng_set_seed(uint64_t seed)
{
	state = 2 * seed + 1; // seed must be odd
}

double rng_01(void)
{
	uint64_t x = state;
	uint64_t count = x >> 61;

	state = x * mult;
	x ^= x >> 22;

	return u32_to_double_01((uint32_t) (x >> (22 + count)));
}

#elif PRNG_IDX == 1 // xoshiro256+

/*
 * PRIVATE:
 */

static uint64_t state[4];

static double u64_to_double_01(uint64_t val)
{
	uint64_t u64 = 0x3FF0000000000000 | ((val >> 12) | 1);
	return *(double*) &u64 - 1.0; // force bits of u64 to be interpreted as double
}

/*
 * PUBLIC:
 */

void rng_set_seed(uint64_t seed)
{
	state[0] = seed * 0xAB7F0912A3B1C813; 
	state[1] = seed * 0x9E3779B97F4A7C15;
	state[2] = seed * 0xBF58476D1CE4E5B9;
	state[3] = seed * 0x94D049BB133111EB;
}

double rng_01(void)
{
	uint64_t res = state[0] + state[3];
	uint64_t t = state[1] << 17;

	state[2] ^= state[0];
	state[3] ^= state[1];
	state[1] ^= state[2];
	state[0] ^= state[3];
	state[2] ^= t;
	state[3] = (state[3] << 45) | (state[3] >> (64 - 45));

	return u64_to_double_01(res);
}

#else // did not select a valid prng

double rng_01(void)
{
	fprintf(stderr, "Invalid PRNG selected in random.h\n");
	exit(1);
}

void rng_set_seed(uint64_t seed)
{
	fprintf(stderr, "Invalid PRNG selected in random.h\n");
	exit(1);
}

#endif
