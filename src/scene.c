#include "scene.h"

/*
 * Initializes a scene that can hold up to 1000 hittable objects. The method 
 * accepts a pointer to the preallocated scene struct and allocates memory for 
 * each of the 1000 hittables that the scene can have. If any of these allocations 
 * fail, then the application exits with code 1.
 */
void scene_init(Hittable_List* scene)
{
	scene->length = 0;
	for (size_t i = 0; i < 1000; i++)
	{
		if ((scene->hittables[i] = malloc(sizeof(Hittable*))) == NULL)
		{
			fprintf(stderr, "malloc failed in scene");
			exit(1);
		}
	}	
}

/*
 * Adds an Obj_Object to the scene (created by importing a 3d model in the obj 
 * format). This object is similar to a scene, it has an array of internal hittables 
 * which are all triangles. This method loops over every triangle within the object 
 * and adds it to the scene. If the hittable object limit for the scene is reached 
 * (1000) then the application exits with code 1. See scene_add for extra info.
 */
void scene_add_obj(Hittable_List* scene, Obj_Object* object)
{
	for (size_t i = 0; i < object->length; i++)
	{
		size_t next_idx = scene->length++;
		if (next_idx >= sizeof(scene->hittables) / sizeof(scene->hittables[0]))
		{
			fprintf(stderr, "Attempting to write outside of array bounds in scene add obj\n");
			exit(1);
		}
		scene->hittables[next_idx] = object->tris[i];
	}
}

/**
 * Adds a single hittable object to the scene. If the hittable object limit for 
 * the scene is reached (1000) then the application exits with code 1. Hittables 
 * in the scene's array must be well packed for the intersection method to work 
 * well. This can be ensured as there is currently no way to dynamically remove 
 * a hittable from the scene, this limitation is minimal as this hittable could 
 * simply not be added in the first place.
 */
void scene_add(Hittable_List* scene, Hittable* object)
{
	size_t next_idx = scene->length++;
	if (next_idx >= sizeof(scene->hittables) / sizeof(scene->hittables[0]))
	{
		fprintf(stderr, "Attempting to write outside of array bounds: scene.hittables\n");
		exit(1);
	}
	scene->hittables[next_idx] = object;
}

/*
 * Returns the index of a hittable within the scene (scene) that the given ray (r)
 * intersected with within the interval (itvl). Information about the intersection 
 * is stored in (hit_rec) such as the intersection point, distance, colour, etc...
 * The closest intersection within the interval is always returned. If no objects 
 * from the scene intersect with the given ray, then the method returns SIZE_MAX 
 * (the maximum value of size_t) as a sentinel value.
 */
size_t scene_hit_idx(Hittable_List* scene, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	size_t hit_idx = SIZE_MAX;
	for (size_t i = 0; i < scene->length; i++)
	{
		if (scene->hittables[i] == NULL)
			break;

		Hit_Record* temp_rec;
		if ((temp_rec = malloc(sizeof(Hit_Record))) == NULL)
		{
			fprintf(stderr, "malloc failed in scene\n");
			exit(1);
		}
		if (hittable_hit(scene->hittables[i], r, itvl, temp_rec))
		{
			if (interval_surrounds(itvl, temp_rec->t))
			{
				hit_idx = i;
				itvl.max = temp_rec->t;
				memcpy(hit_rec, temp_rec, sizeof(*temp_rec));
			}
		}
		free(temp_rec);
	}
	return hit_idx;
}
