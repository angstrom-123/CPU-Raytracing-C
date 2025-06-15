#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "random.h"

#define PI acos(-1.0)

typedef struct Vector {
	double x, y, z;
} Vector;

typedef struct Interval {
	double min, max;
} Interval;

typedef struct Ray {
	Vector origin;
	Vector direction;
} Ray;

extern Vector vec_add(Vector u, Vector v);
extern Vector vec_sub(Vector u, Vector v);
extern Vector vec_mul(Vector u, double t);
extern Vector vec_mul_vec(Vector u, Vector v);
extern Vector vec_div(Vector u, double t);
extern Vector vec_cross(Vector u, Vector v);
extern double vec_dot(Vector u, Vector v);
extern double vec_length2(Vector u);
extern double vec_length(Vector u);
extern Vector vec_unit(Vector u);
extern Vector vec_rndm(double min, double max);
extern Vector vec_rndm_unit(void);
extern Vector vec_rndm_in_hemi(Vector surf_norm);
extern Vector vec_rndm_in_unit_disk(void);
extern Vector vec_reflect(Vector u, Vector surf_norm);
extern Vector vec_refract(Vector u, Vector surf_norm, double refraction_const);
extern bool vec_near_zero(Vector u);

extern bool interval_contains(Interval i, double val);
extern bool interval_surrounds(Interval i, double val);
extern Interval interval_universe(void);

extern Vector ray_at(Ray ray, double t);

#endif
