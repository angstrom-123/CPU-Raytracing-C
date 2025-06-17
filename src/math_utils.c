#include "math_utils.h"

double max(double a, double b)
{
	if (a > b) return a;
	return b;
}

double min(double a, double b)
{
	if (a < b) return a;
	return b;
}

Vector vec_add(Vector u, Vector v)
{
	Vector out = {u.x + v.x, u.y + v.y, u.z + v.z};
	return out;
}

Vector vec_add_3(Vector u, Vector v, Vector w)
{
	Vector out = {u.x + v.x + w.x, u.y + v.y + w.y, u.z + v.z + w.z};
	return out;
}

Vector vec_sub(Vector u, Vector v)
{
	Vector out = {u.x - v.x, u.y - v.y, u.z - v.z};
	return out;
}

Vector vec_mul(Vector u, double t)
{
	Vector out = {u.x * t, u.y * t, u.z * t};
	return out;
}

Vector vec_mul_vec(Vector u, Vector v)
{
	Vector out = {u.x * v.x, u.y * v.y, u.z * v.z};
	return out;
}

Vector vec_div(Vector u, double t)
{
	Vector out = {u.x / t, u.y / t, u.z / t};
	return out;
}

Vector vec_cross(Vector u, Vector v)
{
	Vector out = {u.y * v.z - u.z * v.y,
				  u.z * v.x - u.x * v.z,
				  u.x * v.y - u.y * v.x
	};
	return out;
}

double vec_dot(Vector u, Vector v)
{
	return u.x * v.x 
		 + u.y * v.y 
		 + u.z * v.z;
}

double vec_length2(Vector u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

double vec_length(Vector u)
{
	return sqrt(vec_length2(u));
}

Vector vec_unit(Vector u)
{
	return vec_div(u, vec_length(u));
}

Vector vec_rndm(double min, double max)
{
	Vector out = {(max - min) * rng_01() + min,
				  (max - min) * rng_01() + min,
				  (max - min) * rng_01() + min};
	return out;
}

Vector vec_rndm_unit(void)
{
	while (true)
	{
		Vector p = vec_rndm(-1.0, 1.0);
		double len_squared = vec_length2(p);
		if ((len_squared <= 1.0) && (len_squared > 1.0E-160))
		{
			return vec_div(p, sqrt(len_squared));
		}
	}
}

double vec_axis(Vector u, size_t axis_idx)
{
	switch (axis_idx) {
	case 0:
		return u.x;
	case 1:
		return u.y;
	case 2:
		return u.z;
	default:
		fprintf(stderr, "attempting to access oob axis in math utils\n");
		exit(1);
	}
}

/*
 * Returns true if the absolute value of each axis is less than e.
 */
bool vec_near_zero(Vector u)
{
	double e = 1.0E-8;
	bool x_low, y_low, z_low;

	if (u.x < 0.0)
		x_low = u.x > -e;
	else 
		x_low = u.x < e;

	if (u.y < 0.0)
		y_low = u.y > -e;
	else 
		y_low = u.y < e;

	if (u.z < 0.0)
		z_low = u.z > -e;
	else 
		z_low = u.z < e;

	return x_low && y_low && z_low;
}

bool interval_contains(Interval i, double val)
{
	return (val < i.max) && (val > i.min);
}

bool interval_surrounds(Interval i, double val)
{
	return (val <= i.max) && (val >= i. min);
}

/*
 * Overall scale of scenes is assumed to be fairly small, so the interval is 
 * between -1000.0 and 1000.0
 */
Interval interval_universe(void)
{
	Interval out = {-1000.0, 1000.0};
	return out;
}
