#include "hittable.h"

/*
 * PRIVATE:
 */

static bool hit_sphere(Hittable* hittable, Ray r, Interval itvl, 
					   Hit_Record* hit_rec)
{
	Vector oc = vec_sub(hittable->transform.position, r.origin);
	double a = vec_length_squared(r.direction);
	double h = vec_dot(r.direction, oc);
	double c = vec_length_squared(oc) 
			 - hittable->transform.scale * hittable->transform.scale;

	double discriminant = h * h - a * c;
	if (discriminant < 0.0) 
		return false;

	double sqrt_d = sqrt(discriminant);
	double root = (h - sqrt_d) / a;
	if (!interval_surrounds(itvl, root))
	{
		root = (h + sqrt_d) / a;
		if (!interval_surrounds(itvl, root)) 
			return false;
	}

	hit_rec->t = root;
	hit_rec->p = ray_at(r, root);
	hit_rec->nrml = vec_div(vec_sub(hit_rec->p, hittable->transform.position), 
							hittable->transform.scale);
	hit_rec->material = hittable->material;

	return true;
}

static bool hit_tri(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	return false; // TODO
}

/*
 * PUBLIC:
 */

Hittable* new_hittable_trans(E_Hittable type, Hittable_Transform trans, Material material)
{
	Hittable* htbl = malloc(sizeof(Hittable));
	htbl->type = type;
	htbl->transform = trans;
	htbl->material = material;
	return htbl;
}

Hittable* new_hittable_pos(E_Hittable type, Vector pos, double s, Material material)
{
	Hittable_Transform trans = {pos, s, {0}};
	return new_hittable_trans(type, trans, material);
}

Hittable* new_hittable_xyz(E_Hittable type, double x, double y, double z, 
						   double s, Material material)
{
	Vector pos = {x, y, z};
	Hittable_Transform trans = {pos, s, {0}};
	return new_hittable_trans(type, trans, material);
}

bool hittable_hit(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	switch (hittable->type) {
		case SPHERE : return hit_sphere(hittable, r, itvl, hit_rec);
		case TRI: return hit_tri(hittable, r, itvl, hit_rec);
		default: return false;
	}
}
