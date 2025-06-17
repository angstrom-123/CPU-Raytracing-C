#ifndef SCATTER_H
#define SCATTER_H

#include "math_utils.h"
#include "render_utils.h"

/*
 * Returns the direction of a ray after a lambertian diffuse in a surface with 
 * the given normal.
 */
extern Vector scatter_diffuse(Vector surf_norm);

/*
 * Returns the direction of a ray after a perfect reflection in a surface with 
 * the given normal.
 */
extern Vector scatter_metallic(Vector incoming, Vector surf_norm);

/*
 * Returns the direction of a ray after a reflection / refraction with a 
 * transparent material with a given normal and refractive index.
 */
extern Vector scatter_glass(Vector incoming, Vector surf_norm, 
							bool front_face, double constant);

#endif
