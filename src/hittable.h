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


typedef struct Hittable {
	E_Hittable hittable_type;
	Vector position;
	double scale;
	Vector albedo;
} Hittable;

extern bool hittable_hit(Hittable* hittable, Ray r, Interval itvl, Hit_Record* hit_rec);

#endif
