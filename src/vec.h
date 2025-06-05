#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "random.h"

typedef struct Vector {
	double x, y, z;
} Vector;

extern Vector vector_add(Vector u, Vector v);
extern Vector vector_sub(Vector u, Vector v);
extern Vector vector_mul(Vector u, double t);
extern Vector vector_div(Vector u, double t);
extern Vector cross_product(Vector u, Vector v);
extern double dot_product(Vector u, Vector v);
extern Vector vector_random(double min, double max);
extern Vector vector_random_on_hemisphere(Vector surf_norm);
extern Vector vector_random_in_unit_disk();
extern Vector reflect(Vector u, Vector surf_norm);
extern Vector refract(Vector u, Vector surf_norm, double refraction_const);

#endif
