#include "vec.h"

Vector VEC_Add(Vector u, Vector v)
{
	Vector out = {.x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z};
	return out;
}

Vector VEC_Sub(Vector u, Vector v)
{
	Vector out = {.x = u.x - v.x, .y = u.y - v.y, .z = u.z - v.z};
	return out;
}

Vector VEC_Mul(Vector u, double t)
{
	Vector out = {.x = u.x * t, .y = u.y * t, .z = u.z * t};
	return out;
}

Vector VEC_Div(Vector u, double t)
{
	Vector out = {.x = u.x / t, .y = u.y / t, .z = u.z / t};
	return out;
}

Vector VEC_Cross(Vector u, Vector v)
{
	Vector out = {
		.x = u.y * v.z - u.z * v.y,
		.y = u.z * v.x - u.x * v.z,
		.z = u.x * v.y - u.y * v.x
	};
	return out;
}

double VEC_Dot(Vector u, Vector v)
{
	return u.x * v.x 
		   + u.y * v.y 
		   + u.z * v.z;
}
