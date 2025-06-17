#ifndef HITTABLE_H
#define HITTABLE_H

#include "math_utils.h"
#include "aabb.h"

/*
 * Specifies the supported material type for a hittable object.
 */
typedef enum E_Material {
	DIFFUSE,
	METALLIC,
	GLASS
} E_Material;

/*
 * Specifies the supported object type for a hittable, the number represents 
 * the amount of basis vectors that this hittable type stores.
 */
typedef enum E_Hittable {
	SPHERE = 1,
	TRI	   = 6
} E_Hittable;

/*
 * Overarching type for materials consisting of a type, colour, and refractive 
 * index (when applicable).
 */
typedef struct Material {
	E_Material type;
	Vector 	   albedo;
	double 	   constant;
} Material;

/*
 * Generic hittable type that supports variable number of vectors for representing 
 * different types of object.
 */
typedef struct Hittable {
	E_Hittable type;
	size_t 	   v_len;
	Vector*    vectors;
	Material   mat;
	double     scale;
	AABB 	   aabb;
} Hittable;

/*
 * Struct for an imported OBJ file that consists of many triangles.
 */
typedef struct Obj_Object {
	Hittable* tris[1000];
	size_t 	  length;
	Material  mat;
	Vector 	  pos;
} Obj_Object;

/*
 * Checks if the given ray collides with the given hittable in the given interval 
 * of distances and stores data about the collision in hit_rec.
 */
extern bool hittable_hit(Hittable* h, Ray r, Interval itvl, Hit_Record* hit_rec);

/*
 * Returns a pointer to a new sphere hittable with a given position, radius, and material
 */
extern Hittable* hittable_new_sphere(double x, double y, double z, double s, Material material);

/*
 * Returns a pointer to a new triangle hittable with given vertex coordinates, normals, 
 * and material.
 */
extern Hittable* hittable_new_tri(Vector a, Vector b, Vector c, 
								  Vector na, Vector nb, Vector nc, Material material);

#endif
