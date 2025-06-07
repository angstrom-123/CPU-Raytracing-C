#ifndef HITTABLE_H
#define HITTABLE_H

#include "math_utils.h"

typedef enum E_Hittable {
	SPHERE,
	TRI
} E_Hittable;

typedef struct Hit_Record {
	double t;
	Vector p;
	Vector nrml;
	Vector albedo;
} Hit_Record;

typedef struct Hittable_Transform {
	Vector position;
	double scale;
} Hittable_Transform;

typedef struct Hittable {
	E_Hittable type;
	Hittable_Transform transform;
	Vector albedo;
} Hittable;


extern bool hittable_hit(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec);
extern Hittable* new_hittable_trans(E_Hittable type, Hittable_Transform trans, Vector albedo);
extern Hittable* new_hittable_pos(E_Hittable type, Vector pos, double s, Vector albedo);
extern Hittable* new_hittable_xyz(E_Hittable type, double x, double y, double z,
								  double s, Vector albedo);

#endif
