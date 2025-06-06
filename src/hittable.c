#include "hittable.h"
#include "my_assert.h"
#include <math.h>
#include <stdio.h>

bool hit_sphere(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	Vector oc = vec_sub(hittable->position, r.origin);
	double a = vec_length_squared(r.direction);
	double h = vec_dot(r.direction, oc);
	double c = vec_length_squared(oc) - hittable->scale * hittable->scale;

	double discriminant = h * h - a * c;
	if (discriminant < 0.0) 
		return false;

	double sqrt_d = sqrt(discriminant);
	double root = (h - sqrt_d) / a;
	if ((root <= itvl.min) || (itvl.max <= root))
	{
		root = (h + sqrt_d) / a;
		if ((root <= itvl.min) || (itvl.max <= root)) 
			return false;
	}

	hit_rec->t = root;
	hit_rec->p = ray_at(r, root);
	hit_rec->nrml = vec_div(vec_sub(hit_rec->p, hittable->position), hittable->scale);
	hit_rec->albedo = hittable->albedo;

	return true;
}

bool hit_tri(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	return false; // TODO
}

bool hittable_hit(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	switch (hittable->hittable_type) {
		case SPHERE : return hit_sphere(hittable, r, itvl, hit_rec);
		case TRI: return hit_tri(hittable, r, itvl, hit_rec);
	};
}
