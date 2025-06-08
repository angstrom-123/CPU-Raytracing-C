#include "camera.h"
#include "hittable.h"
#include "material.h"
#include "math_utils.h"

/*
 * PRIVATE:
 */

static void init_defaults(Camera* cam, double screen_width, double screen_height)
{
	Vector pos = {0.0, 0.0, 0.0};
	Vector facing = {0.0, 0.0, -1.0};
	Vector up = {0.0, 1.0, 0.0};
	Camera_Transform* trans = malloc(sizeof(Camera_Transform));

	cam->transform = trans;
	cam->transform->position = pos;
	cam->transform->facing = facing;
	cam->transform->v_up = up;

	cam->aspect_ratio = screen_width / screen_height;
	cam->samples_per_pixel = 10;
	cam->max_ray_bounces = 1;
	cam->fov_radians = PI / 3.0;
	cam->focus_distance = 1.0;
	cam->defocus_angle = 0.0; // TODO: find out why this fucks the projection
}

static Ray get_ray(Camera* cam, uint16_t col, uint16_t row)
{
	Vector ray_orig;
	Vector ray_dir;
	Vector square_sample = {generate_random() - 0.5, 
							generate_random() - 0.5, 
							0.0};
	Vector x_offset = vec_mul(cam->pixel_delta_u, 
							  (double) col + square_sample.x);
	Vector y_offset = vec_mul(cam->pixel_delta_v, 
							  (double) row + square_sample.y);
	Vector pixel_sample = vec_add(vec_add(x_offset, y_offset), 
								  cam->pixel_0_pos);
	if (cam->defocus_angle <= 0.0)
		ray_orig = cam->transform->position;
	else 
	{
		Vector disk_sample = vec_random_in_unit_disk();
		Vector x_d_offset = vec_mul(cam->defocus_disk_u, disk_sample.x);
		Vector y_d_offset = vec_mul(cam->defocus_disk_v, disk_sample.y);
		ray_orig = vec_add(vec_add(x_d_offset, y_d_offset), 
						   cam->transform->position);
	}
	ray_dir = vec_sub(pixel_sample, ray_orig);

	Ray out = {ray_orig, ray_dir};
	return out;
}

static Vector background_colour(Ray r)
{
	Vector bg_col_top = {0.5, 0.7, 1.0};
	Vector bg_col_bot = {1.0, 1.0, 1.0};
	Vector unit_dir = vec_unit(r.direction);
	double a = 0.5 * (unit_dir.y + 1.0);
	return vec_add(vec_mul(bg_col_bot, 1.0 - a), 
				   vec_mul(bg_col_top, a));
}


static Vector ray_colour(Ray r, Hittable_List* scene, uint8_t max_bounces)
{
	Interval itvl = {0.0, 1000.0};
	Hit_Record* hit_rec = malloc(sizeof(Hit_Record));
	if (hit_in_scene(scene, r, itvl, hit_rec))
	{
		// normals
		Vector white = {1.0, 1.0, 1.0};
		return vec_div(vec_add(hit_rec->nrml, white), 2.0);

		// diffuse
		// Vector dir = vec_random_on_hemisphere(hit_rec->nrml);
		// Ray r2 = {hit_rec->p, dir};
		// return vec_div(ray_colour(r2, scene, --max_bounces), 2.0);
	}
	return background_colour(r);
}

/*
 * PUBLIC:
 */

void cam_calc_matrices(Camera* cam, uint16_t screen_width, uint16_t screen_height) 
{
	double scrn_wid = (double) screen_width;
	double scrn_hei = (double) screen_height;

	// viewport dimensions
	cam->vp_height = 2.0 * tan(cam->fov_radians / 2.0) * cam->focus_distance;
	cam->vp_width = cam->vp_height * (scrn_wid / scrn_hei);

	// camera basis vectors
	cam->transform->w = vec_unit(vec_mul(cam->transform->facing, -1.0));
	cam->transform->u = vec_unit(vec_cross(cam->transform->v_up, 
										   cam->transform->w));
	cam->transform->v = vec_cross(cam->transform->w, cam->transform->u);

	// viewport basis vectors
	cam->vp_u = vec_mul(cam->transform->u, cam->vp_width);
	cam->vp_v = vec_mul(cam->transform->v, -cam->vp_height);

	// camera pixel offsets
	cam->pixel_delta_u = vec_div(cam->vp_u, scrn_wid);
	cam->pixel_delta_v = vec_div(cam->vp_v, scrn_hei);

	// camera top left pos
	Vector offset = vec_add(vec_mul(cam->transform->w, cam->focus_distance), 
							vec_add(vec_div(cam->vp_u, 2.0), 
								   	vec_div(cam->vp_v, 2.0)));
	Vector u_plus_v = vec_add(cam->pixel_delta_u, cam->pixel_delta_v);
	cam->pixel_0_pos = vec_add(vec_sub(cam->transform->position, offset),
							   vec_div(u_plus_v, 2.0));

	// defocus disk basis vectors
	cam->defocus_disk_u = vec_mul(cam->transform->u, cam->defocus_angle);
	cam->defocus_disk_v = vec_mul(cam->transform->v, cam->defocus_angle);
}

void cam_init(Camera* cam, uint16_t screen_width, uint16_t screen_height)
{
	seed_random(0);
	init_defaults(cam, (double) screen_height, (double) screen_width);
	cam_calc_matrices(cam, screen_width, screen_height);
}

void cam_render(void (*set_pixel_func)(uint16_t, uint16_t, Vector), 
			Camera* cam, Hittable_List* scene, 
			uint16_t screen_width, uint16_t screen_height)
{
	uint8_t samp_per_pix = cam->samples_per_pixel;
	for (uint16_t row = 0; row < screen_height; row++)
	{
		for (uint16_t col = 0; col < screen_width; col++)
		{
			Vector pix_col = {0.0, 0.0, 0.0};
			for (uint8_t samp_idx = 0; samp_idx < samp_per_pix; samp_idx++)
			{
				Ray r = get_ray(cam, col, row);
				Vector samp_col = ray_colour(r, scene, cam->max_ray_bounces);
				pix_col = vec_add(pix_col, samp_col);
			}
			set_pixel_func(col, row, vec_div(pix_col, (double) samp_per_pix));
		}
	}
}
