#ifndef AABB_H
#define AABB_H

#include "render_utils.h"
#include "math_utils.h"

typedef struct AABB {
	Interval x;
	Interval y;
	Interval z;
} AABB;

extern AABB AABB_from_AABB(AABB aabb_1, AABB aabb_2);
extern AABB AABB_from_corners(Vector u, Vector v);
extern bool AABB_hit(AABB aabb, Ray r, Interval* itvl, Hit_Record* hit_rec);

#endif
