#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>

#include "math_utils.h"
#include "hittable.h"

typedef struct Hittable_List {
	Hittable* hittables[1000]; // array of pointers
	uint16_t length;
	uint8_t padding[6];
} Hittable_List;

extern Hittable_List init_scene(void);
extern void add_to_scene(Hittable_List* scene, Hittable* object);
extern bool hit_in_scene(Hittable_List* scene, Ray r, Interval itvl, Hit_Record* hit_rec);

#endif
