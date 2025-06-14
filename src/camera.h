#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"
#include "scene.h"
#include "scatter.h"
#include "hittable.h"

typedef struct Camera_Transform {
	Vector position;
	Vector facing;
	Vector u, v, w;
	Vector v_up;
} Camera_Transform;

typedef struct Camera {
	Camera_Transform* transform;// transform pointer
	double aspect_ratio;		// width / height
	uint16_t samples_per_pixel;	// rays per pixel
	uint16_t max_ray_bounces;	// max bounces to track
	double fov_radians;			// field of view radians
	double focus_distance;		// focal length
	double defocus_angle;		// size of defocus disk
	Vector defocus_disk_u;		// x component of defocus disk
	Vector defocus_disk_v;		// y component of defocus disk
	Vector pixel_0_pos;			// top left pixel in the image
	Vector pixel_delta_u;		// x offset between pixels
	Vector pixel_delta_v;		// y offset between pixels 
	double vp_height, vp_width;	// dimensions of the viewport
	Vector vp_u, vp_v;			// vectors along viewport edges
} Camera;

extern void cam_init(Camera* cam, size_t screen_width, size_t screen_height);
extern void cam_calc_matrices(Camera* cam, size_t screen_width, size_t screen_height);
extern void cam_render_section(void (*set_pixel)(size_t, size_t, Vector), 
				   			   Camera* cam, Hittable_List* scene, size_t start_x, 
							   size_t start_y, size_t end_x, size_t end_y);
extern void cam_render(void (*set_pixel_func)(size_t, size_t, Vector), 
				   	   Camera* cam, Hittable_List* scene, 
				   	   size_t screen_width, size_t screen_height);

#endif
