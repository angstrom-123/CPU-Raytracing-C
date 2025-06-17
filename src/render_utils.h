#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include "math_utils.h"

/*
 * Struct for storing information about a ray's interaction with hittables in the 
 * scene.
 */
typedef struct Hit_Record {
	double t;	  // distance of an intersection with the scene
	Vector p;	  // position of the intersection with the scene
	Vector norm;  // normal vector of the surface at the intersection point
	Vector atten; // colour of the surface collided with
	bool   front; // true if the ray hit the front of the surface, else false
} Hit_Record;

/*
 * Returns a new random unit vector within the hemisphere centred around the 
 * given vector.
 */
extern Vector vec_rndm_in_hemi(Vector surf_norm);

/*
 * Returns a random vector within a unit (radius = 1.0) disk.
 */
extern Vector vec_rndm_in_unit_disk(void);

/*
 * Reflects the given vector in a plane with normal (surf_norm)
 */
extern Vector vec_reflect(Vector u, Vector surf_norm);

/*
 * Refracts the given vector through a surface with normal (surf_norm) and a 
 * refractive index of (refraction_const)
 */
extern Vector vec_refract(Vector u, Vector surf_norm, double refraction_const);

/*
 * Returns the coordinates of a point (t) units along the given ray.
 */
extern Vector ray_at(Ray ray, double t);

#endif
