#include "camera.h"
#include <stdio.h>

void init_cam_vals(Camera* cam, double screen_width, double screen_height)
{
	Vector pos = {0.0, 0.0, 0.0};
	Vector facing = {0.0, 0.0, -1.0};
	Vector up = {0.0, 1.0, 0.0};

	cam->aspect_ratio = screen_width / screen_height;
	cam->samples_per_pixel = 25;
	cam->max_ray_bounces = 1;
	cam->fov_radians = PI / 2.0;
	cam->look_from = pos;
	cam->look_at = facing;
	cam->view_up = up;
	cam->focus_distance = 1.0;
	cam->defocus_radians = 0.0;
}

void init_camera(Camera* cam, uint16_t screen_width, uint16_t screen_height)
{
	double scrn_wid = (double) screen_width;
	double scrn_hei = (double) screen_height;

	// initialize default values
	init_cam_vals(cam, scrn_wid, scrn_hei);

	// viewport dimensions
	cam->vp_height = 2.0 * tan(cam->fov_radians / 2.0) * cam->focus_distance;
	cam->vp_width = cam->vp_height * (scrn_wid / scrn_hei);

	// camera basis vectors
	cam->w = vec_unit(vec_sub(cam->look_from, cam->look_at));
	cam->u = vec_unit(vec_cross(cam->view_up, cam->w));
	cam->v = vec_cross(cam->w, cam->u);

	// viewport basis vectors
	cam->vp_u = vec_mul(cam->u, cam->vp_width);
	cam->vp_v = vec_mul(cam->v, -cam->vp_height);

	// camera pixel offsets
	cam->pixel_delta_u = vec_div(cam->vp_u, scrn_wid);
	cam->pixel_delta_v = vec_div(cam->vp_v, scrn_hei);

	// viewport top left pos
	Vector offset = vec_add(vec_mul(cam->w, cam->focus_distance), 
							vec_add(vec_div(cam->vp_u, 2.0), 
								   	vec_div(cam->vp_v, 2.0)));

	// camera top left pos
	Vector u_plus_v = vec_add(cam->pixel_delta_u, cam->pixel_delta_v);
	cam->pixel_0_pos = vec_add(vec_sub(cam->look_from, offset),
							   vec_div(u_plus_v, 2.0));

	// defocus disk basis vectors
	cam->defocus_disk_u = vec_mul(cam->u, cam->defocus_radians);
	cam->defocus_disk_v = vec_mul(cam->v, cam->defocus_radians);
}

Ray get_ray(Camera* cam, uint16_t col, uint16_t row)
{
	Vector square_sample = {generate_random() - 0.5, 
							generate_random() - 0.5, 
							0.0};
	Vector x_offset = vec_mul(cam->pixel_delta_u, (double) col + square_sample.x);
	Vector y_offset = vec_mul(cam->pixel_delta_v, (double) row + square_sample.y);
	Vector pixel_sample = vec_add(vec_add(x_offset, y_offset), 
										  cam->pixel_0_pos);
	Vector ray_orig;
	if (cam->defocus_radians <= 0.0)
		ray_orig = cam->look_from;
	else 
	{
		Vector disk_sample = vec_random_in_unit_disk();
		Vector x_d_offset = vec_mul(cam->defocus_disk_u, disk_sample.x);
		Vector y_d_offset = vec_mul(cam->defocus_disk_v, disk_sample.y);
		ray_orig = vec_add(vec_add(x_d_offset, y_d_offset), cam->look_from);
	}
	Vector ray_dir = vec_sub(pixel_sample, ray_orig);
	Ray out = {ray_orig, ray_dir};
	return out;
}

Vector ray_colour(Ray r, Hittable_List* scene, uint8_t max_bounces)
{
	Vector bg_col = {0.7, 0.8, 1.0};
	Vector colour;
	if (scene->hittables[0] != NULL)
	{
		Hit_Record* hit_rec = malloc(sizeof(Hit_Record));
		Interval itvl = {0.0, 100.0};
		if (hittable_hit(scene->hittables[0], r, itvl, hit_rec))
			colour = hit_rec->nrml;
		else 
			colour = bg_col;
		free(hit_rec);
	}
	return colour;
}

void render(void (*set_pixel_func)(uint16_t, uint16_t, Vector), 
			Camera* cam, Hittable_List* scene, 
			uint16_t screen_width, uint16_t screen_height)
{
	uint8_t samples_per_pixel = cam->samples_per_pixel;
	for (uint16_t row = 0; row < screen_height; row++)
	{
		for (uint16_t col = 0; col < screen_width; col++)
		{
			Ray r = get_ray(cam, col, row);
			set_pixel_func(col, row, ray_colour(r, scene, cam->max_ray_bounces));
		}
	}
}
