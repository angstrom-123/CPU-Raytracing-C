#include <stdlib.h>

#ifndef UNIT_TEST
#include "camera.h"
#include "hittable.h"
#include "renderer.h"
#include "scene.h"

#include <SDL3/SDL_events.h>

static bool keys[322];

static bool handle_events(void)
{
	SDL_Event* event = malloc(sizeof(SDL_Event));
	SDL_PollEvent(event);
	if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) 
	{
		free(event);
		return true;
	}
	if (event->type == SDL_EVENT_KEY_DOWN)
		keys[event->key.scancode] = true;
	if (event->type == SDL_EVENT_KEY_UP)
		keys[event->key.scancode] = false;

	free(event);
	return false;
}

static bool apply_events(Camera_Transform* trans)
{
	double move_step = 0.05;

	Vector pos_delta = {0.0, 0.0, 0.0};

	if (keys[SDL_SCANCODE_W])
		pos_delta.z -= move_step;
	else if (keys[SDL_SCANCODE_A])
		pos_delta.x -= move_step;
	else if (keys[SDL_SCANCODE_S])
		pos_delta.z += move_step;
	else if (keys[SDL_SCANCODE_D])
		pos_delta.x += move_step;
	else 
		return false;

	trans->position = vec_add(trans->position, pos_delta);
	return true;
}

void run(void)
{
	const uint16_t screen_width = 320;
	const uint16_t screen_height = 180;

	bool debug_movement = false;

	init_renderer(screen_width, screen_height);

	Camera* cam = malloc(sizeof(Camera));
	cam_init(cam, screen_width, screen_height);

	Vector col_white = {1.0, 1.0, 1.0};
	Material diff_white = {DIFFUSE, col_white};

	Hittable* sphere_a = new_hittable_xyz(SPHERE, 0.0, 0.0, -1.0, 
										  0.5, diff_white);
	Hittable* sphere_b = new_hittable_xyz(SPHERE, 0.0, -100.5, -1.0, 
										  100.0, diff_white);

	Hittable_List scene = init_scene();
	add_to_scene(&scene, sphere_a);
	add_to_scene(&scene, sphere_b);

	cam_render(&set_pixel, cam, &scene, screen_width, screen_height);
	update_render_window();

	bool quit = false;
	uint8_t ctr = 0;
	while (!quit)
	{
		quit = handle_events();
		if (debug_movement) 
		{
			clock_t start, end;
			start = clock();
			apply_events(cam->transform);

			if (ctr == 0)
			{
				cam_calc_matrices(cam, screen_width, screen_height);
				cam_render(&set_pixel, cam, &scene, screen_width, screen_height);
				update_render_window();
				ctr = 2;
			}

			end = clock();
			uint32_t delta = end - start;
			if (delta > 10)
				delta = 10;

			SDL_Delay(10 - delta);
			ctr--;
		}
		else 
		{
			SDL_Delay(10);
		}

	}
	free(cam);
	close_render_window();
}
#endif

#ifdef UNIT_TEST
#include "random.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

void test(void)
{
	printf("Running Tests\n");
	printf("Testing rng distribution:\n");
	rng_set_seed(time(NULL));
	uint32_t bin_0, bin_1, bin_2, bin_3, bin_4, bin_5, bin_6, bin_7, bin_8, bin_9;
	for (uint32_t i = 0; i < 1000000; i++)
	{
		double val = rng_01_fpcg();
		if (val < 0.1) bin_0++;
		else if (val < 0.2) bin_1++;
		else if (val < 0.3) bin_2++;
		else if (val < 0.4) bin_3++;
		else if (val < 0.5) bin_4++;
		else if (val < 0.6) bin_5++;
		else if (val < 0.7) bin_6++;
		else if (val < 0.8) bin_7++;
		else if (val < 0.9) bin_8++;
		else bin_9++;
	}
	printf("0.0-0.1: %hu\n0.1-0.2: %hu\n0.2-0.3: %hu\n0.3-0.4: %hu\n0.4-0.5: %hu\n"
			"0.5-0.6: %hu\n0.6-0.7: %hu\n0.7-0.8: %hu\n0.8-0.9: %hu\n0.9=1.0: %hu\n",
		   bin_0, bin_1, bin_2, bin_3, bin_4, bin_5, bin_6, bin_7, bin_8, bin_9);
}
#endif

int main(void) 
{
#ifdef UNIT_TEST 
	test();
#endif
#ifndef UNIT_TEST
	run();
#endif
	exit(0);
}
