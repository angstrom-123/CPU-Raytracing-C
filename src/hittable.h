#ifndef HITTABLE_H
#define HITTABLE_H

#include "math_utils.h"

typedef enum E_Material {
	DIFFUSE,
	METALLIC,
	GLASS
} E_Material;

typedef struct Material {
	E_Material type;
	Vector albedo;
	double constant;
} Material;

typedef struct Hit_Record {
	double t;
	Vector p;
	Vector norm;
	Vector atten;
	bool   front;
} Hit_Record;

typedef struct Hittable {
	size_t 	 bv_len;
	Vector*  basis_vectors;
	Material mat;
	double   scale;
} Hittable;

extern bool hittable_hit(Hittable* h, Ray r, Interval itvl, Hit_Record* hit_rec);
extern Hittable* sphere_new(double x, double y, double z, double s, Material material);

#endif
