#include "scene.h"

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
