#include "material.h"
#include "math_utils.h"

static bool near_zero(Vector u)
{
	double epsilon = 1E-6;

	if (u.x < 0.0) u.x *= -1.0;
	if (u.y < 0.0) u.y *= -1.0;
	if (u.z < 0.0) u.z *= -1.0;

	return (u.x < epsilon) && (u.y < epsilon) && (u.z < epsilon);
}

void scatter_diffuse(Ray* r, Hit_Record hit_rec)
{
	Vector ray_dir = vec_add(hit_rec.nrml, vec_random_in_unit_disk());
	Vector ray_orig = ray_at(*r, hit_rec.t);
	
	if (near_zero(ray_dir)) ray_dir = hit_rec.nrml;

	r->direction = ray_dir;
	r->origin = ray_orig;
}

void scatter_specular(Ray* r, Hit_Record hit_rec)
{
	// TODO
}
