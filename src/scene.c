#include "scene.h"

void init_scene(Hittable_List* scene)
{
	scene->length = 0;
	for (uint16_t i = 0; i < 1000; i++)
	{
		scene->hittables[i] = malloc(sizeof(Hittable*));
		if (scene->hittables[i] == NULL)
		{
			fprintf(stderr, "malloc failed in scene");
			exit(1);
		}
	}	
}

void add_to_scene(Hittable_List* scene, Hittable* object)
{
	uint16_t next_idx = scene->length++;
	if (next_idx >= sizeof(scene->hittables) / sizeof(scene->hittables[0]))
	{
		fprintf(stderr, "Attempting to write outside of array bounds: scene.hittables\n");
		exit(1);
	}
	scene->hittables[next_idx] = *&object;
}

uint16_t hit_in_scene(Hittable_List* scene, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	uint16_t hit_idx = UINT16_MAX;
	for (uint16_t i = 0; i < scene->length; i++)
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
