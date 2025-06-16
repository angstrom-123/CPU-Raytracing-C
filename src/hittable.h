#ifndef HITTABLE_H
#define HITTABLE_H

#include "math_utils.h"
#include "aabb.h"

typedef enum E_Material {
	DIFFUSE,
	METALLIC,
	GLASS
} E_Material;

typedef enum E_Hittable {
	SPHERE = 1,
	TRI	   = 6
} E_Hittable;

typedef struct Material {
	E_Material type;
	Vector 	   albedo;
	double 	   constant;
} Material;

typedef struct Hittable {
	E_Hittable type;
	size_t 	   v_len;
	Vector*    vectors;
	Material   mat;
	double     scale;
	AABB 	   aabb;
} Hittable;

typedef struct Obj_Object {
	Hittable* tris[1000];
	size_t 	  length;
	Material  mat;
	Vector 	  pos;
} Obj_Object;

extern bool hittable_hit(Hittable* h, Ray r, Interval itvl, Hit_Record* hit_rec);
extern Hittable* hittable_new_sphere(double x, double y, double z, double s, Material material);
extern Hittable* hittable_new_tri(Vector a, Vector b, Vector c, 
								  Vector na, Vector nb, Vector nc, Material material);

#endif
