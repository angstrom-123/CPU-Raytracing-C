#import "aabb.h"

/*
 * Returns the interval corresponding to the axis index passed in for the given 
 * AABB. Axis indices map: 0 = x, 1 = y, 2 = z, 3 = error (exit code 1)
 */
static Interval _axis_interval(AABB aabb, size_t axis_idx)
{
	switch (axis_idx){
	case 0:
		return aabb.x;
	case 1:
		return aabb.y;
	case 2:
		return aabb.z;
	default:
		fprintf(stderr, "attempting to access oob axis in AABB\n");
		exit(1);
	}
}

/*
 * Returns the index of the largest axis (interval with the largest range) for 
 * the given AABB. Axis index mapping can be seen above in _axis_interval method.
 */
static size_t _largest_axis(AABB aabb)
{
	double max = -1000.0;
	size_t idx;
	for (size_t i = 0; i < 3; i++)
	{
		Interval tmp = _axis_interval(aabb, i); 
		double range = tmp.max - tmp.min;
		if (range > max)
		{
			max = range;
			idx = i;
		}
	}
	return idx;
}

/*
 * Returns a new AABB that encloses the two AABB's passed in.
 */
AABB AABB_from_AABB(AABB aabb_1, AABB aabb_2)
{
	Interval x = {min(aabb_1.x.min, aabb_2.x.min), max(aabb_1.x.max, aabb_2.x.max)};
	Interval y = {min(aabb_1.y.min, aabb_2.y.min), max(aabb_1.y.max, aabb_2.y.max)};
	Interval z = {min(aabb_1.z.min, aabb_2.z.min), max(aabb_1.z.max, aabb_2.z.max)};
	AABB out = {x, y, z};
	return out;
}

/*
 * Returns a new AABB between 2 opposing corners passed in. If any axis of the 
 * resulting AABB is too small, then it is automatically increased to a minimum
 * size of 2E-8 to avoid floating point rounding errors.
 */
AABB AABB_from_corners(Vector u, Vector v)
{
	Interval x = {min(u.x, v.x), max(u.x, v.x)};
	if (x.max - x.min < 1.0E-8)
	{
		x.min -= 1.0E-8;
		x.max += 1.0E-8;
	}

	Interval y = {min(u.y, v.y), max(u.y, v.y)};
	if (y.max - y.min < 1.0E-8)
	{
		y.min -= 1.0E-8;
		y.max += 1.0E-8;
	}

	Interval z = {min(u.z, v.z), max(u.z, v.z)};
	if (z.max - z.min < 1.0E-8)
	{
		z.min -= 1.0E-8;
		z.max += 1.0E-8;
	}

	AABB out = {x, y, z};
	return out;
}

/*
 * Checks if a given ray intersects with the given AABB within a given distance 
 * range (itvl). If no intersection occurs, then the function returns false. If 
 * there is an intersection within the interval, then the bounds of the interval 
 * are reduced to reflect the new colission. This interval can then be reused for 
 * further colision tests to reduce the amount of objects that need to be colision
 * tested (as some will be outside of the range).
 *
 * This method uses ray/slab intersection testing as AABB's are by definition 
 * axis-aligned. This check is very cheap.
 */
bool AABB_hit(AABB aabb, Ray r, Interval* itvl)
{
	for (size_t i = 0; i < 3; i++)
	{
		Interval ax_itvl = _axis_interval(aabb, i);
		double axis_dir_inv = 1.0 / vec_axis(r.direction, i);
		
		double t0 = (ax_itvl.min - vec_axis(r.origin, i)) * axis_dir_inv;
		double t1 = (ax_itvl.max - vec_axis(r.origin, i)) * axis_dir_inv;

		if (t0 < t1)
		{
			if (t0 > itvl->min)
				itvl->min = t0;
			if (t1 < itvl->max)
				itvl->max= t1;
		}
		else 
		{
			if (t1 > itvl->min)
				itvl->min = t1;
			if (t0 < itvl->max)
				itvl->max= t0;
		}

		if (itvl->max <= itvl->min)
			return false;
	}

	return true;
}
