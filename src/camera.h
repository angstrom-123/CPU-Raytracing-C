#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"
#include "scene.h"
#include "scatter.h"
#include "hittable.h"

/*
 * Struct for storing the position and basis vectors of the camera.
 */
typedef struct Camera_Transform {
	Vector position;
	Vector facing;
	Vector u, v, w;
	Vector v_up;
} Camera_Transform;

/*
 * Struct for storing parameters, settings, and constants for the camera.
 */
typedef struct Camera {
	Camera_Transform* transform;		   // transform pointer
	double 			  aspect_ratio;		   // width / height
	uint16_t 		  samples_per_pixel;   // rays per pixel
	uint16_t 		  max_ray_bounces;	   // max bounces to track
	double 			  fov_radians;		   // field of view radians
	double 			  focus_distance;	   // focal length
	double 			  defocus_angle;	   // size of defocus disk
	Vector 			  defocus_disk_u;	   // x component of defocus disk
	Vector 			  defocus_disk_v;	   // y component of defocus disk
	Vector 			  pixel_0_pos;		   // top left pixel in the image
	Vector 			  pixel_delta_u;	   // x offset between pixels
	Vector 			  pixel_delta_v;	   // y offset between pixels 
	double 			  vp_height, vp_width; // dimensions of the viewport
	Vector 			  vp_u, vp_v;		   // vectors along viewport edges
} Camera;

/*
 * Initializes the default settigns for teh given camera struct pointed at by cam.
 */
extern void cam_init(Camera* cam, size_t screen_width, size_t screen_height);

/*
 * Forces a recalculation of the view matrices of the given camera struct.
 * This should be used after changing the position, rotation, focus distance, etc..
 * of the camera to see the effect.
 */
extern void cam_calculate_matrices(Camera* cam, size_t screen_width, 
								   size_t screen_height);

/*
 * Renders a portion of the given scene to the screen (between start / end, x / y).
 */
extern void cam_render_section(void (*set_pixel)(size_t, size_t, Vector), 
				   			   Camera* cam, Hittable_List* scene, size_t start_x, 
							   size_t start_y, size_t end_x, size_t end_y);

/*
 * Renders a the given scene to the screen.
 */
extern void cam_render(void (*set_pixel_func)(size_t, size_t, Vector), 
				   	   Camera* cam, Hittable_List* scene, 
				   	   size_t screen_width, size_t screen_height);

#endif
