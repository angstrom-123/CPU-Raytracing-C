#include "scatter.h"

/*
 * Returns the direction of a ray that has scattered diffusely (lambertian 
 * diffuse) in a completely rough surface with the normal (surf_norm). In this 
 * simple implementation, the direction of the scattered ray is completely random.
 */
Vector scatter_diffuse(Vector surf_norm)
{
	Vector out = vec_add(surf_norm, vec_rndm_unit());
	if (vec_near_zero(out)) return surf_norm;
	return out;
}

/*
 * Returns the direction of a ray that has been reflected in a surface with 
 * normal (surf_norm). The incoming direction of the ray to be reflected is required.
 * This implementation always assumes perfect reflectance.
 */
Vector scatter_metallic(Vector incoming, Vector surf_norm)
{
	return vec_reflect(incoming, surf_norm);
}

/*
 * Returns the direction of a ray that has been scattered through a transparent 
 * material such as glass. This requires knowledge of the incoming direction (incoming),
 * normal of the surface (surf_norm), if the incoming ray is entering or exiting the 
 * material (front_face: true if entering, false if exiting) and the refraction 
 * constant of the material (constant).
 *
 * For a realistic effect, glancing rays are reflected whereas direct hits are 
 * refracted, these two are blended between by using a random number to weight 
 * each case more heavily towards one or the other. This method uses an approximation 
 * of the reflectance of the material based on its constant, this approximation is 
 * quite accurate and widely accepted as "good enough".
 */
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
