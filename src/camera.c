#include "camera.h"
#include "scene.h"

/*
 * PRIVATE:
 */

static void _set_defaults(Camera* cam, double screen_width, double screen_height)
{
	Vector pos = {0.0, 0.5, 2.0};
	Vector facing = {0.0, 0.0, -1.0};
	Vector up = {0.0, 1.0, 0.0};
	Camera_Transform* trans;
	if ((trans = malloc(sizeof(Camera_Transform))) == NULL)
	{
		fprintf(stderr, "malloc failed in camera\n");
		exit(1);
	}

	cam->transform = trans;
	cam->transform->position = pos;
	cam->transform->facing = facing;
	cam->transform->v_up = up;

	cam->aspect_ratio = screen_width / screen_height;
	cam->samples_per_pixel = 50;
	cam->max_ray_bounces = 5;
	// cam->samples_per_pixel = 200;
	// cam->max_ray_bounces = 75;
	cam->fov_radians = PI / 2.0;
	cam->focus_distance = 1.5;
	cam->defocus_angle = 0.0;
	// cam->defocus_angle = 0.1;
// #ifdef DEBUG
// 	cam->samples_per_pixel = 20;
// 	cam->max_ray_bounces = 5;
// #elif RELEASE
// 	cam->samples_per_pixel = 200;
// 	cam->max_ray_bounces = 75;
// #endif
}

static Ray _get_ray(Camera* cam, uint16_t col, uint16_t row)
{
	Vector ray_orig;
	Vector ray_dir;
	Vector square_sample = {rng_01() - 0.5, 
							rng_01() - 0.5, 
							0.0};
	Vector x_offset = vec_mul(cam->pixel_delta_u, 
							  (double) (col + square_sample.x));
	Vector y_offset = vec_mul(cam->pixel_delta_v, 
							  (double) (row + square_sample.y));
	Vector pixel_sample = vec_add(vec_add(x_offset, y_offset), 
								  cam->pixel_0_pos);
	if (cam->defocus_angle <= 0.0)
		ray_orig = cam->transform->position;
	else 
	{
		Vector disk_sample = vec_rndm_in_unit_disk();
		Vector x_d_offset = vec_mul(cam->defocus_disk_u, disk_sample.x);
		Vector y_d_offset = vec_mul(cam->defocus_disk_v, disk_sample.y);
		ray_orig = vec_add(vec_add(x_d_offset, y_d_offset), 
						   cam->transform->position);
	}
	ray_dir = vec_sub(pixel_sample, ray_orig);
	Ray out = {ray_orig, ray_dir};
	return out;
}

static Vector _bg_ray_col(Ray r)
{
	Vector bg_col_top = {0.5, 0.7, 1.0};
	Vector bg_col_bot = {1.0, 1.0, 1.0};
	Vector unit_dir = vec_unit(r.direction);
	double a = 0.5 * (unit_dir.y + 1.0);
	return vec_add(vec_mul(bg_col_bot, 1.0 - a), 
				   vec_mul(bg_col_top, a));
}

static Vector _ray_col(Ray r, Hittable_List* scene, uint16_t max_bounces)
{
	if (max_bounces <= 0) 
	{
		Vector black = {0.0, 0.0, 0.0};
		return black;
	}
	Interval itvl = {0.001, 1000.0};
	Hit_Record* hit_rec;
	if ((hit_rec = malloc(sizeof(Hit_Record))) == NULL)
	{
		fprintf(stderr, "malloc failed in camera\n");
		exit(1);
	}
	size_t hit_idx;
	if ((hit_idx = scene_hit_idx(scene, r, itvl, hit_rec)) != SIZE_MAX) 
	{
		Ray bounce;
		bounce.origin = hit_rec->p;
		Material mat = scene->hittables[hit_idx]->mat;
		switch (mat.type) {
		case DIFFUSE: 
			bounce.direction = scatter_diffuse(hit_rec->norm);
			break;
		case METALLIC: 
			bounce.direction = scatter_metallic(r.direction, hit_rec->norm);
			break;
		case GLASS: 
			bounce.direction = scatter_glass(r.direction, hit_rec->norm, 
					hit_rec->front, mat.constant);
			break;
		default:
			free(hit_rec);
			return _bg_ray_col(r);
		}
		Vector col =  vec_mul_vec(hit_rec->atten, 
						   		  _ray_col(bounce, scene, --max_bounces));
		free(hit_rec);
		return col;
	}
	free(hit_rec);
	return _bg_ray_col(r);
}

/*
 * PUBLIC:
 */

void cam_calculate_matrices(Camera* cam, size_t screen_width, size_t screen_height) 
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

void cam_init(Camera* cam, size_t screen_width, size_t screen_height)
{
	rng_set_seed(time(NULL));
	_set_defaults(cam, (double) screen_height, (double) screen_width);
	cam_calculate_matrices(cam, screen_width, screen_height);
}

void cam_render_section(void (*set_pixel)(size_t, size_t, Vector), Camera* cam, 
						Hittable_List* scene, size_t start_x, size_t start_y,
					    size_t end_x, size_t end_y)
{
	size_t samp_per_pix = cam->samples_per_pixel;
	for (size_t row = start_y; row < end_y; row++)
	{
		fflush(stdout);
		for (size_t col = start_x; col < end_x; col++)
		{
			Vector pix_col = {0.0, 0.0, 0.0};
			for (size_t samp_idx = 0; samp_idx < samp_per_pix; samp_idx++)
			{
				Ray r = _get_ray(cam, col, row);
				Vector samp_col = _ray_col(r, scene, cam->max_ray_bounces);
				pix_col = vec_add(pix_col, samp_col);
			}
			set_pixel(col, row, vec_div(pix_col, (double) samp_per_pix));
		}
	}
}

void cam_render(void (*set_pixel)(size_t, size_t, Vector), Camera* cam, 
				Hittable_List* scene, size_t screen_width, size_t screen_height)
{
	uint8_t samp_per_pix = cam->samples_per_pixel;
	for (size_t row = 0; row < screen_height; row++)
	{
		printf("\rscanlines remaining: %u  ", (uint) (screen_height - row - 1));
		fflush(stdout);
		for (size_t col = 0; col < screen_width; col++)
		{
			Vector pix_col = {0.0, 0.0, 0.0};
			for (size_t i = 0; i < samp_per_pix; i++)
			{
				Ray r = _get_ray(cam, col, row);
				Vector samp_col = _ray_col(r, scene, cam->max_ray_bounces);
				pix_col = vec_add(pix_col, samp_col);
			}
			set_pixel(col, row, vec_div(pix_col, (double) samp_per_pix));
		}
	}
	printf("\rrender complete         \n");
}
