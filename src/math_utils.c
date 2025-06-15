#include "math_utils.h"

Vector vec_add(Vector u, Vector v)
{
	Vector out = {u.x + v.x, u.y + v.y, u.z + v.z};
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

Vector vec_rndm_in_hemi(Vector surf_norm) 
{
	Vector rnd_unit_vec = vec_rndm_unit();
	return (vec_dot(rnd_unit_vec, surf_norm) > 0.0) 
	? rnd_unit_vec
	: vec_mul(rnd_unit_vec, -1.0);
}

Vector vec_rndm_in_unit_disk(void)
{
	while (true)
	{
		Vector rnd_xy_vec = {2 * rng_01() - 1.0,
							 2 * rng_01() - 1.0,
							 0.0};
		if (vec_length2(rnd_xy_vec) < 1.0)
		{
			return rnd_xy_vec;
		}
	}
}

Vector vec_reflect(Vector u, Vector surf_norm)
{
	double mult = 2.0 * vec_dot(u, surf_norm);
	return vec_sub(u, vec_mul(surf_norm, mult));
}

Vector vec_refract(Vector u, Vector surf_norm, double constant) 
{
	double cos_theta = vec_dot(vec_mul(u, -1.0), surf_norm);
	if (cos_theta > 1.0) cos_theta = 1.0;
	Vector perp = vec_mul(vec_add(u, vec_mul(surf_norm, cos_theta)), constant);

	double abs_len_sq = 1.0 - vec_length2(perp);
	if (abs_len_sq < 0.0) abs_len_sq = -abs_len_sq;
	Vector para = vec_mul(surf_norm, sqrt(abs_len_sq));

	return vec_sub(perp, para);
}

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

Interval interval_universe(void)
{
	Interval out = {-1000.0, 1000.0};
	return out;
}

Vector ray_at(Ray ray, double t)
{
	return vec_add(ray.origin, vec_mul(ray.direction, t));
}
