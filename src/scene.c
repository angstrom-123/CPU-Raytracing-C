#include "scene.h"

Hittable_List init_scene(void)
{
	Hittable_List scene = {{NULL}, 0, {0}};
	return scene;
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

bool hit_in_scene(Hittable_List* scene, Ray r, Interval itvl, Hit_Record* hit_rec)
{
	bool did_hit = false;
	for (uint16_t i = 0; i < scene->length; i++)
	{
		if (scene->hittables[i] != NULL)
		{
			Hit_Record* temp_rec = malloc(sizeof(Hit_Record));
			if (hittable_hit(scene->hittables[i], r, itvl, temp_rec))
			{
				did_hit = true;
				if (temp_rec->t <= itvl.max)
				{
					itvl.max = temp_rec->t;
					hit_rec = temp_rec;
				} 
			}
			free(temp_rec);
		}
	}
	return did_hit;
}
