#include "render_utils.h"

Vector vec_rndm_in_hemi(Vector surf_norm) 
{
	Vector rnd_unit_vec = vec_rndm_unit();
	return (vec_dot(rnd_unit_vec, surf_norm) > 0.0) 
	? rnd_unit_vec
	: vec_mul(rnd_unit_vec, -1.0);
}

/*
 * Returns a new vector within a disk with a radius of 1 unit. This method uses 
 * a loop to repeatedly generate points within a 2x2 (-1 - 1) square and only 
 * proceeds if a point close enough to the centre (within the unit disk) is 
 * generated. On average, this is faster than performing the division to force
 * the generated vector to be within the disk.
 */
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

/*
 * Returns a new vector that is a reflection of the given vector (u) in the surface
 * that has the normal vector (surf_norm).
 */
Vector vec_reflect(Vector u, Vector surf_norm)
{
	double mult = 2.0 * vec_dot(u, surf_norm);
	return vec_sub(u, vec_mul(surf_norm, mult));
}

/*
 * Returns a new vector representing the new direction of the given vector (u)
 * after it hits a surface with normal (surf_norm) and is refracted with the 
 * refraction constant (constant). For reference, air has a constant of 1.0, and 
 * glass has a constant of around 1.5.
 */
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

/*
 * Returns a vector holding the coordinates of the point along the given ray (r)
 * at the given step (t).
 */
Vector ray_at(Ray ray, double t)
{
	return vec_add(ray.origin, vec_mul(ray.direction, t));
}
