#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "math_utils.h"
#include "hittable.h"

typedef struct Hittable_List {
	Hittable* hittables[1000]; // array of pointers
	size_t length;
} Hittable_List;

extern void scene_init(Hittable_List* scene);
extern void scene_add(Hittable_List* scene, Hittable* object);
extern size_t scene_hit_idx(Hittable_List* scene, Ray r, Interval itvl, Hit_Record* hit_rec);

#endif
