#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include "math_utils.h"

typedef struct Hit_Record {
	double t;
	Vector p;
	Vector norm;
	Vector atten;
	bool   front;
} Hit_Record;

extern Vector vec_rndm_in_hemi(Vector surf_norm);
extern Vector vec_rndm_in_unit_disk(void);
extern Vector vec_reflect(Vector u, Vector surf_norm);
extern Vector vec_refract(Vector u, Vector surf_norm, double refraction_const);
extern Vector ray_at(Ray ray, double t);

#endif
