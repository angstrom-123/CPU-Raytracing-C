#ifndef AABB_H
#define AABB_H

#include "render_utils.h"
#include "math_utils.h"

/*
 * AABB is axis-aligned so each axis component is represented as two scalar 
 * values representing its range.
 */
typedef struct AABB {
	Interval x;
	Interval y;
	Interval z;
} AABB;

/*
 * Returns a new AABB that contains the two given AABB's.
 */
extern AABB AABB_from_AABB(AABB aabb_1, AABB aabb_2);

/*
 * Returns a new AABB with its opposing corners being u and v.
 */
extern AABB AABB_from_corners(Vector u, Vector v);

/*
 * Checks if the ray given intersects with the aabb and updates the bounds 
 * of the interval if it does.
 */
extern bool AABB_hit(AABB aabb, Ray r, Interval* itvl);

#endif
