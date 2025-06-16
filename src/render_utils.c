#include "render_utils.h"

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

Vector ray_at(Ray ray, double t)
{
	return vec_add(ray.origin, vec_mul(ray.direction, t));
}
