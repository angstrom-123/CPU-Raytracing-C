#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "random.h"

#define PI acos(-1.0)

/*
 * Struct for storing 3 axes of a vector either for coordinates or directions.
 */
typedef struct Vector {
	double x, y, z;
} Vector;

/*
 * Struct for representing a range of values between a min and max.
 */
typedef struct Interval {
	double min, max;
} Interval;

/*
 * Struct for representing a ray that can be cast with an origin and direction.
 */
typedef struct Ray {
	Vector origin;
	Vector direction;
} Ray;

/*
 * Returns the maximum value of the two numbers passed in.
 */
extern double max(double a, double b);

/*
 * Returns the minimum value of the two numbers passed in.
 */
extern double min(double a, double b);

/*
 * Returns a new vector with the result of an addition of the two vectors 
 * passed in (u + v)
 */
extern Vector vec_add(Vector u, Vector v);

/*
 * Returns a new vector with the result of an addition of the three vectors 
 * passed in (u + v + w)
 */
extern Vector vec_add_3(Vector u, Vector v, Vector w);

/*
 * Returns a new vector with the result of a subtraction of the second vector
 * from the first vector (u - v).
 */
extern Vector vec_sub(Vector u, Vector v);

/*
 * Returns a new vector with the result of the vector passed in multiplied by the 
 * scalar (u * t)
 */
extern Vector vec_mul(Vector u, double t);

/*
 * Returns a new vector with the result of the first vector multiplied by the 
 * second vector (u * v).
 */
extern Vector vec_mul_vec(Vector u, Vector v);

/*
 * Returns a new vector with the result of the vector passed in divided by the 
 * scalar (u / t)
 */
extern Vector vec_div(Vector u, double t);

/*
 * Returns the cross product of the two vectors passed in, in a new vector (u X v).
 */
extern Vector vec_cross(Vector u, Vector v);

/*
 * Returns the scalar (dot) product of the two vectors passed in (u . d).
 */
extern double vec_dot(Vector u, Vector v);

/*
 * Returns the square of the length of the given vector (|u|^2)
 */
extern double vec_length2(Vector u);

/*
 * Returns the length of the given vector (|u|).
 */
extern double vec_length(Vector u);

/*
 * Returns a new vector with the same direction as vector (u) but with a length 
 * of 1.0. (|u| -> 1.0)
 */
extern Vector vec_unit(Vector u);

/*
 * Returns a random vector with each component in the range min-max.
 */
extern Vector vec_rndm(double min, double max);

/*
 * Returns a random unit vector (|u| = 1.0)
 */
extern Vector vec_rndm_unit(void);

/*
 * Returns the axis corresponding to (axis_idx) of the vector: 0=x, 1=y, 2=z.
 * If an invalid axis index is supplied, the application exits with code 1.
 */
extern double vec_axis(Vector u, size_t axis_idx);

/*
 * Checks if the given vector (u) is near 0.0 on all axes.
 */
extern bool vec_near_zero(Vector u);

/*
 * Checks if val is within the bounds of the interval (i.min < v < i.max)
 */
extern bool interval_contains(Interval i, double val);

/*
 * Checks if val is within the bounds (inclusive) of the interval (i.min <= v <= i.max)
 */
extern bool interval_surrounds(Interval i, double val);

/*
 * Returns an interval with a very large range.
 */
extern Interval interval_universe(void);


#endif
