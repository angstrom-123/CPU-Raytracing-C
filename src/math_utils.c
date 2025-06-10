#include "math_utils.h"
#include "random.h"

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

double vec_length_squared(Vector u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

double vec_length(Vector u)
{
	return sqrt(vec_length_squared(u));
}

Vector vec_unit(Vector u)
{
	return vec_div(u, vec_length(u));
}

Vector vec_random(double min, double max)
{
	Vector out = {(max - min) * rng_01_fpcg() + min,
				  (max - min) * rng_01_fpcg() + min,
				  (max - min) * rng_01_fpcg() + min};
	return out;
}

Vector vec_random_on_hemisphere(Vector surf_norm) 
{
	Vector rnd_unit_vec;
	while (true)
	{
		Vector p = vec_random(-1.0, 1.0);
		double len_squared = vec_length_squared(p);
		if ((len_squared < 1.0) && (len_squared > 1E-160))
		{
			rnd_unit_vec = vec_div(p, sqrt(len_squared));
			break;
		}
	}
	return (vec_dot(rnd_unit_vec, surf_norm) < 0.0) 
	? vec_mul(rnd_unit_vec, -1.0) 
	: rnd_unit_vec;
}

Vector vec_random_in_unit_disk(void)
{
	while (true)
	{
		Vector rnd_xy_vec = {2 * rng_01_fpcg() - 1.0,
							 2 * rng_01_fpcg() - 1.0,
							 0.0};
		if (vec_length_squared(rnd_xy_vec) < 1.0)
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

Vector vec_refract(Vector u, Vector surf_norm, double refraction_const) 
{
	Vector norm_theta, perp, para;
	double cos_theta;
	double inv_perp_len;

	cos_theta = vec_dot(vec_mul(u, -1.0), surf_norm);
	if (cos_theta > 1.0) cos_theta = 1.0;

	norm_theta = vec_mul(surf_norm, cos_theta);
	perp = vec_mul(vec_add(u, norm_theta), refraction_const);
	inv_perp_len = 1.0 - vec_length_squared(perp);
	if (inv_perp_len < 0.0) inv_perp_len = -inv_perp_len;

	para = vec_mul(surf_norm, (double) sqrt(inv_perp_len));
	return vec_sub(perp, para);
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
