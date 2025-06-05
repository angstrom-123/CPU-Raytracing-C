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

double vector_length_squared(Vector u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

double vector_length(Vector u)
{
	return sqrt(vector_length_squared(u));
}

Vector vector_random(double min, double max)
{
	Vector out = {
		.x = generate_random_in_range(min, max),
		.y = generate_random_in_range(min, max),
		.z = generate_random_in_range(min, max)
	};
	return out;
}

Vector vector_random_on_hemisphere(Vector surf_norm) 
{
	Vector rnd_unit_vec;
	while (true)
	{
		Vector p = vector_random(-1.0, 1.0);
		double len_squared = vector_length_squared(p);
		if ((len_squared < 1.0) && (len_squared > 0.0)) // TODO: check if this needs an offset from 0.0
		{
			rnd_unit_vec = vector_div(p, sqrt(len_squared));
			break;
		}
	}
	return (dot_product(rnd_unit_vec, surf_norm) < 0.0) 
		   ? vector_mul(rnd_unit_vec, -1.0) 
		   : rnd_unit_vec;
}

Vector vector_random_in_unit_disk()
{
	while (true)
	{
		Vector rnd_xy_vec = {
			.x = generate_random_in_range(-1.0, 1.0),
			.y = generate_random_in_range(-1.0, 1.0),
			0.0
		};
		if (vector_length_squared(rnd_xy_vec) < 1.0)
		{
			return rnd_xy_vec;
		}
	}
}

Vector reflect(Vector u, Vector surf_norm)
{
	double mult = 2.0 * dot_product(u, surf_norm);
	return vector_sub(u, vector_mul(surf_norm, mult));
}

Vector refract(Vector u, Vector surf_norm, double refraction_const) 
{
	double cos_theta = dot_product(vector_mul(u, -1.0), surf_norm);
	if (cos_theta > 1.0) cos_theta = 1.0;

	Vector norm_theta = vector_mul(surf_norm, cos_theta);
	Vector perp = vector_mul(vector_add(u, norm_theta), refraction_const);
	double inv_perp = 1.0 - vector_length_squared(perp);
	if (inv_perp < 0.0) inv_perp = -inv_perp;

	Vector para = vector_mul(surf_norm, sqrt(inv_perp));
	return vector_sub(perp, para);
}
