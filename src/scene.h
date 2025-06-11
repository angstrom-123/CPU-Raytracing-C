#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "math_utils.h"
#include "hittable.h"

typedef struct Hittable_List {
	Hittable* hittables[1000]; // array of pointers
	uint16_t length;
} Hittable_List;

extern void init_scene(Hittable_List* scene);
extern void add_to_scene(Hittable_List* scene, Hittable* object);
extern uint16_t hit_in_scene(Hittable_List* scene, Ray r, Interval itvl, Hit_Record* hit_rec);

#endif
