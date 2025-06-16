#include "scene_builder.h"

Hittable_List* build_demo_scene()
{
	Vector col_red = {1.0, 0.2, 0.2};
	Vector col_blue = {0.2, 0.2, 1.0};
	Vector col_white = {1.0, 1.0, 1.0};
	Vector col_gray = {0.7, 0.7, 0.7};

	Material diff_blue = {DIFFUSE, col_blue, 0.0};
	Material diff_red = {DIFFUSE, col_red, 0.0};
	Material metal_blue = {METALLIC, col_blue, 0.0};
	Material metal_gray = {METALLIC, col_gray, 0.0};
	Material glass_white = {GLASS, col_white, 1.5};

	Hittable* sphere_a = hittable_new_sphere(0.0, -100.5, -1.0, 100.0, diff_red);
	// Hittable* sphere_b = hittable_new_sphere(0.55, 0.0, -1.0, 0.5, glass_white);
	// Hittable* sphere_c = hittable_new_sphere(0.1, 0.0, -1.5, 0.5, metal_blue);
	// Hittable* sphere_d = hittable_new_sphere(-0.5, 0.0, -2.0, 0.5, diff_red);
	Obj_Object* obj = parse_obj_file("res/porsche.obj", 0.0, 0.0, 0.0, diff_red);

	Hittable_List* scene;
	if ((scene = malloc(sizeof(Hittable_List))) == NULL)
	{
		fprintf(stderr, "malloc failed in scene builder\n");
		exit(1);
	} 
	scene_init(scene);

	scene_add(scene, sphere_a);
	// scene_add(scene, sphere_b);
	// scene_add(scene, sphere_c);
	// scene_add(scene, sphere_d);
	scene_add_obj(scene, obj);

	return scene;
}
