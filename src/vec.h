#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct Vector {
	double x, y, z;
} Vector;

extern Vector vector_add(Vector u, Vector v);
extern Vector vector_sub(Vector u, Vector v);
extern Vector vector_mul(Vector u, double t);
extern Vector vector_div(Vector u, double t);
extern Vector cross_product(Vector u, Vector v);
extern double dot_product(Vector u, Vector v);

#endif
