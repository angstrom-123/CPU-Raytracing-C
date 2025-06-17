#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "math_utils.h"
#include "hittable.h"

/*
 * Struct representing a renderable scene that rays can be cast through. A list 
 * of hittables.
 */
typedef struct Hittable_List {
	Hittable* hittables[1000];
	size_t length;
} Hittable_List;

/*
 * Initializes a given scene by allocation heap memory for each hittable pointer.
 */
extern void scene_init(Hittable_List* scene);

/*
 * Adds a pointer to each triangle from inside an obj_object to the scene.
 */
extern void scene_add_obj(Hittable_List* scene, Obj_Object* object);

/*
 * Adds a normal hittable object pointer to the scene.
 */
extern void scene_add(Hittable_List* scene, Hittable* object);

/*
 * Casts the given ray through the given scene and returns the index of the 
 * hittable object that it collided with in the scene within the given interval.
 * Results of the collision are stored in the hit_rec.
 */
extern size_t scene_hit_idx(Hittable_List* scene, Ray r, Interval itvl, Hit_Record* hit_rec);

#endif
