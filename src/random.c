#include "random.h"

/*
 * PRIVATE:
 */

static uint64_t fpcg_state = 0xcafe00dd15ea5e5l;
static uint64_t const fpcg_mult = 6364136223846793005;

static uint64_t xoshiro_state[4];

static uint32_t fast_pcg(void)
{
	uint64_t x = fpcg_state;
	uint64_t count = x >> 61;
	fpcg_state = x * fpcg_mult;
	x ^= x >> 22;
	return (uint32_t) (x >> (22 + count));
}

static uint64_t xoshiro_256p(void)
{
	uint64_t res = xoshiro_state[0] + xoshiro_state[3];
	uint64_t t = xoshiro_state[1] << 17;

	xoshiro_state[2] ^= xoshiro_state[0];
	xoshiro_state[3] ^= xoshiro_state[1];
	xoshiro_state[1] ^= xoshiro_state[2];
	xoshiro_state[0] ^= xoshiro_state[3];
	xoshiro_state[2] ^= t;
	xoshiro_state[3] = (xoshiro_state[3] << 45) | (xoshiro_state[3] >> (64 - 45));

	return res;
}

static double u64_to_double_01(uint64_t val)
{
	uint64_t u64 = 0x3FF0000000000000 | ((val >> 12) | 1);
	return *(double*) &u64 - 1.0; // force bits of u64 to be interpreted as double
}

static double u32_to_double_01(uint32_t val)
{
	uint64_t u64 = 0x3FF0000000000000 | (((((uint64_t) val) << 1) | 1) << 19);
	return *(double*) &u64 - 1.0; // force bits of u64 to be interpreted as double
}

/*
 * PUBLIC:
 */

double rng_01(void)
{
#if PRNG_IDX == 0
	return u32_to_double_01(fast_pcg());
#elif PRNG_IDX == 1
	return u64_to_double_01(xoshiro_256p());
#else
	return 0.0;
#endif
}

void rng_set_seed(uint64_t seed)
{
#if PRNG_IDX == 0
	fpcg_state = 2 * seed + 1; // seed must be odd
#elif PRNG_IDX == 1
	xoshiro_state[0] = seed * 0xAB7F0912A3B1C813; 
	xoshiro_state[1] = seed * 0x9E3779B97F4A7C15;
	xoshiro_state[2] = seed * 0xBF58476D1CE4E5B9;
	xoshiro_state[3] = seed * 0x94D049BB133111EB;
#else 
	fprintf(stderr, "Invalid PRNG selected in random.h\n");
	exit(1);
#endif
}
