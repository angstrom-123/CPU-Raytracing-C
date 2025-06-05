#include <stdlib.h>
#include <time.h>

#include "random.h"

double generate_random()
{
	return generate_random_in_range(0.0, 1.0);
}

double generate_random_in_range(double min, double max)
{
	srand(time(NULL));
	return min + ((double) rand() / RAND_MAX) * (max - min);
}
