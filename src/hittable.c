#include "hittable.h"

/*
 * PRIVATE:
 */

static bool _hit_sphere(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	Vector pos = hittable->vectors[0];
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
	Vector a = hittable->vectors[0];
	Vector ab = vec_sub(hittable->vectors[1], hittable->vectors[0]);
	Vector ac = vec_sub(hittable->vectors[2], hittable->vectors[0]);
	Vector na = hittable->vectors[3];
	Vector nb = hittable->vectors[4];
	Vector nc = hittable->vectors[5];

	Vector ao = vec_sub(r.origin, a);
	Vector dao = vec_cross(ao, r.direction);

	double determinant = -vec_dot(r.direction, vec_cross(ab, ac));
	double inv_det = 1.0 / determinant;

	double dst = vec_dot(ao, vec_cross(ab, ac)) * inv_det;
	double u = vec_dot(ac, dao) * inv_det;
	double v = -vec_dot(ab, dao) * inv_det;
	double w = 1.0 - u - v;

	if ((determinant < 0.0) || (dst < 0.0) || (u < 0.0) || (v < 0.0) || (w < 0.0))
		return false;

	hit_rec->t = dst;
	hit_rec->p = ray_at(r, hit_rec->t);
	Vector normal = vec_unit(vec_add_3(vec_mul(na, w), 
									   vec_mul(nb, u), 
							  		   vec_mul(nc, v)));
	if (vec_dot(r.direction, normal) < 0.0)
	{
		hit_rec->norm = vec_mul(normal, -1.0);
		hit_rec->front = false;
	} 
	else 
	{
		hit_rec->norm = normal;
		hit_rec->front = true;
	}
	hit_rec->atten = hittable->mat.albedo;

	return true;
}

/*
 * PUBLIC:
 */

Hittable* hittable_new_sphere(double x, double y, double z, double s, Material material)
{
	Vector pos = {x, y, z};

	Hittable* out;
	if ((out = malloc(sizeof(Hittable))) == NULL)
	{
		fprintf(stderr, "malloc failed in hittable\n");
		exit(1);
	} 

	out->type = SPHERE;
	out->v_len = (sizeof(Vector) * out->type);

	if ((out->vectors = malloc(out->v_len)) == NULL)
	{
		fprintf(stderr, "malloc failed in hittable\n");
		exit(1);
	}

	out->vectors[0] = pos;
	out->mat = material;
	out->scale = s;

	Interval x_i = {pos.x - s, pos.x + s};
	Interval y_i = {pos.y - s, pos.y + s};
	Interval z_i = {pos.z - s, pos.z + s};
	AABB aabb = {x_i, y_i, z_i};
	out->aabb = aabb;

	return out;
}

Hittable* hittable_new_tri(Vector a, Vector b, Vector c, 
						   Vector na, Vector nb, Vector nc, Material material)
{
	Hittable* out;
	if ((out = malloc(sizeof(Hittable))) == NULL)
	{
		fprintf(stderr, "malloc failed in hittable\n");
		exit(1);
	} 

	out->type = TRI;
	out->v_len = sizeof(Vector) * out->type;

	if ((out->vectors = malloc(out->v_len)) == NULL)
	{
		fprintf(stderr, "malloc failed in hittable\n");
		exit(1);
	}

	out->vectors[0] = a;
	out->vectors[1] = b;
	out->vectors[2] = c;
	out->vectors[3] = na;
	out->vectors[4] = nb;
	out->vectors[5] = nc;
	out->mat = material;

	double epsilon = 1.0E-8;
	Interval x_i = {min(min(a.x, b.x), c.x) - epsilon, max(max(a.x, b.x), c.x) + epsilon};
	Interval y_i = {min(min(a.y, b.y), c.y) - epsilon, max(max(a.y, b.y), c.y) + epsilon};
	Interval z_i = {min(min(a.z, b.z), c.z) - epsilon, max(max(a.z, b.z), c.z) + epsilon};
	AABB aabb = {x_i, y_i, z_i};
	out->aabb = aabb;

	return out;
}

bool hittable_hit(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	// if (!AABB_hit(hittable->aabb, r, &itvl, hit_rec))
	// 	return false;

	const size_t v_size = sizeof(Vector);
	switch (hittable->type) {
	case SPHERE: // sphere stores position in bv
		return _hit_sphere(hittable, r, itvl, hit_rec);
	case TRI: // tri stores position + 2 basis vectors in bv
		return _hit_tri(hittable, r, itvl, hit_rec);
	default: 
		return false;
	}
}
