#include "scene.h"
#include <stdio.h>

Hittable_List init_scene()
{
	Hittable_List scene = {{NULL}, 0};
	return scene;
}

void add_to_scene(Hittable_List* scene, Hittable* object)
{
	scene->hittables[scene->length++] = *&object;
}

bool hit_in_scene(Hittable_List* scene, Ray r, Interval* itvl, Hit_Record* hit_rec)
{
	return false;
}
