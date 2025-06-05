#ifndef CAMERA_H
#define CAMERA_H

#include "renderer.h"
#include "vec.h"

extern const double aspect_ratio;
extern const uint8_t samp_per_pix;
extern const uint8_t max_bounces;
extern const double focus_dist;
extern const double defocus_angle;

extern void init_camera(int screen_width, int screen_height);

#endif
