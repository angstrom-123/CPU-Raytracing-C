#include "vec.h"

Vector vector_add(Vector u, Vector v)
{
	Vector out = {.x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z};
	return out;
}

Vector vector_sub(Vector u, Vector v)
{
	Vector out = {.x = u.x - v.x, .y = u.y - v.y, .z = u.z - v.z};
	return out;
}

Vector vector_mul(Vector u, double t)
{
	Vector out = {.x = u.x * t, .y = u.y * t, .z = u.z * t};
	return out;
}

Vector vector_div(Vector u, double t)
{
	Vector out = {.x = u.x / t, .y = u.y / t, .z = u.z / t};
	return out;
}

Vector cross_product(Vector u, Vector v)
{
	Vector out = {
		.x = u.y * v.z - u.z * v.y,
		.y = u.z * v.x - u.x * v.z,
		.z = u.x * v.y - u.y * v.x
	};
	return out;
}

double dot_product(Vector u, Vector v)
{
	return u.x * v.x 
		   + u.y * v.y 
		   + u.z * v.z;
}
