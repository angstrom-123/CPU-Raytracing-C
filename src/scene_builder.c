#include "scene_builder.h"

Hittable_List* build_demo_scene()
{
	Vector col_red = {1.0, 0.2, 0.2};
	Material diff_red = {DIFFUSE, col_red, 0.0};
	Vector col_blue = {0.2, 0.2, 1.0};
	Material meta_blue = {METALLIC, col_blue, 0.0};
	Vector col_white = {1.0, 1.0, 1.0};
	Material glass_white = {GLASS, col_white, 1.5};

	Hittable* sphere_a = new_hittable_xyz(SPHERE, 0.0, -100.5, -1.0, 
										  100.0, diff_red);
	Hittable* sphere_b = new_hittable_xyz(SPHERE, 0.7, 0.0, -1.0, 
										  0.5, glass_white);
	Hittable* sphere_c = new_hittable_xyz(SPHERE, 0.1, 0.0, -1.5, 
										  0.5, meta_blue);
	Hittable* sphere_d = new_hittable_xyz(SPHERE, -0.5, 0.0, -2.0, 
										  0.5, diff_red);

	Hittable_List* scene = malloc(sizeof(Hittable_List));
	init_scene(scene);
	add_to_scene(scene, sphere_a);
	add_to_scene(scene, sphere_b);
	add_to_scene(scene, sphere_c);
	add_to_scene(scene, sphere_d);
	return scene;
}
