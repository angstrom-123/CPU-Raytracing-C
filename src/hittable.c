#include "hittable.h"

/*
 * PRIVATE:
 */

static bool _hit_sphere(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	Vector pos = hittable->basis_vectors[0];
	Vector oc = vec_sub(pos, r.origin);
	double a = vec_length2(r.direction);
	double h = vec_dot(r.direction, oc);
	double c = vec_length2(oc) - (hittable->scale * hittable->scale);
	double discriminant = (h * h) - (a * c);

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
	hit_rec->atten = hittable->mat.albedo;
	Vector normal = vec_div(vec_sub(hit_rec->p, pos), hittable->scale);
	if (vec_dot(r.direction, normal) > 0.0)
	{
		hit_rec->norm = vec_mul(normal, -1.0);
		hit_rec->front = false;
	} 
	else 
	{
		hit_rec->norm = normal;
		hit_rec->front = true;
	}

	return true;
}

static bool _hit_tri(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	return false; // TODO
}

/*
 * PUBLIC:
 */

Hittable* sphere_new(double x, double y, double z, double s, Material material)
{
	Vector pos = {x, y, z};
	Hittable* out;
	if ((out = malloc(sizeof(Hittable))) == NULL)
	{
		fprintf(stderr, "malloc failed in hittable\n");
		exit(1);
	} 
	if ((out->basis_vectors = malloc(sizeof(Vector))) == NULL)
	{
		fprintf(stderr, "malloc failed in hittable\n");
		exit(1);
	}

	out->bv_len = sizeof(Vector);
	out->basis_vectors[0] = pos;
	out->mat = material;
	out->scale = s;

	return out;
}

bool hittable_hit(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	const size_t bv_size = sizeof(Vector);
	switch (hittable->bv_len) {
	case (bv_size * 1): // sphere stores position in bv
		return _hit_sphere(hittable, r, itvl, hit_rec);
	case (bv_size * 3): // tri stores position + 2 basis vectors in bv
		return _hit_tri(hittable, r, itvl, hit_rec);
	default: 
		return false;
	}
}
