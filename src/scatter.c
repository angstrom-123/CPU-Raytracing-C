#include "scatter.h"

Vector scatter_diffuse(Vector surf_norm)
{
	Vector out = vec_add(surf_norm, vec_random_unit());
	if (vec_near_zero(out)) return surf_norm;
	return out;
}

Vector scatter_metallic(Vector incoming, Vector surf_norm)
{
	return vec_reflect(incoming, surf_norm);
}

Vector scatter_glass(Vector incoming, Vector surf_norm, bool front_face, double constant)
{
	if (front_face) constant = 1.0 / constant;
	double cos_theta = vec_dot(vec_mul(vec_unit(incoming), -1.0), surf_norm);
	if (cos_theta > 1.0) cos_theta = 1.0;
	double sin_theta = sqrt(1.0 - (cos_theta * cos_theta));

	double reflectance;
	{
		double tmp = (1.0 - constant) / (1.0 + constant);
		tmp = tmp * tmp;
		reflectance = tmp + (1.0 - tmp) * pow(1.0 - cos_theta, 5.0);
	}

	return ((constant * sin_theta > 1.0) || (reflectance > rng_01()))
	? vec_reflect(vec_unit(incoming), surf_norm)
	: vec_refract(vec_unit(incoming), surf_norm, constant);
}
