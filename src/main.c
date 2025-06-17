#ifndef UNIT_TEST
#include "scene_builder.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"

#include <stdlib.h>
#include <SDL3/SDL_events.h>

/*
 * Initializes all required components, constructs a scene, and opens a render 
 * window to start rendering. Periodically polls window events, however this can 
 * be a little laggy especially when rendering with high settings. It may be 
 * necessary to force quit the application if you want to  close prematurely 
 * if the render is very demanding.
 */
void _run(void)
{
	size_t screen_width = 100;
	size_t screen_height = 100;
#ifdef DEBUG
	screen_width = 200;
	screen_height = 113;
#elif RELEASE
	screen_width = 800;
	screen_height = 450;
#endif

	init_renderer(screen_width, screen_height);

	Camera* cam;
	if ((cam = malloc(sizeof(Camera))) != NULL)
		cam_init(cam, screen_width, screen_height);
	else 
	{
		fprintf(stderr, "malloc failed in main\n");
		exit(1);
	}

	// Uncomment the one you want to render, or make your own!
	// Hittable_List* scene = build_demo_scene(cam);
	Hittable_List* scene = build_model_scene(cam);

	cam_calculate_matrices(cam, screen_width, screen_height);

	SDL_Event e;
	uint16_t start_row = 0;

	bool quit = false;
	bool render = true;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type){
			case (SDL_EVENT_WINDOW_CLOSE_REQUESTED):
				quit = true;
				break;
			default:
				break;
			}
		}

		if (render)
		{
			size_t end_row = (size_t) (start_row + 1);
			if (end_row >= screen_height) 
			{
				end_row = screen_height;
				render = false;
			}

			double tmp = ((double) end_row / (double) screen_height) * 100.0;
			size_t percent_complete = round(tmp);
			printf("\r%zu%%", percent_complete);
			fflush(stdout);

			cam_render_section(&set_pixel, cam, scene, 0, 
							   start_row, screen_width, end_row);
			update_render_window();

			if (render == false) printf("\rRender complete\n");
			start_row = end_row;
		} 
		else 
		{
			SDL_Delay(10);
		}
	}
	free(cam);
	free(scene);
	close_render_window();
	printf("\n");
}
#endif

#ifdef UNIT_TEST
#include "random.h"
#include "obj_importer.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

void _test_obj_import(char* file_name)
{
	printf("Testing OBJ file importing:\n");
	printf("Importing: %s\n", file_name);
	Vector white = {1.0, 1.0, 1.0};
	Material diff_white = {DIFFUSE, white, 0.0};
	Obj_Object* obj = parse_obj_file(file_name, 0.0, 0.0, 0.0, diff_white);
}

void _test_rng(void)
{
	printf("Testing rng distribution:\n");
	rng_set_seed(time(NULL));
	uint32_t test_count = 100000000;
	uint32_t bins[50] = {0};
	uint16_t bin_count = sizeof(bins) / sizeof(bins[0]);
	for (size_t i = 0; i < test_count; i++)
	{
		double val = rng_01();
		for (size_t j = 0; j < bin_count; j++)
		{
			double max = (j + 1) * (1.0 / (double) bin_count);
			if (val < max) 
			{
				bins[j]++;
				break;
			}
		}
	}
	uint32_t total_discrepancy = 0.0;
	uint32_t expected = round((double) test_count / (double) bin_count);
	for (size_t i = 0; i < bin_count; i++)
	{
		if (bins[i] < expected) total_discrepancy += expected - bins[i];
		else total_discrepancy += bins[i] - expected;
	}
	double avg_discrepancy = (double) total_discrepancy / (double) bin_count;

	for (size_t i = 0; i < bin_count; i++)
	{
		printf("%f-%f: %hu\n", (i * (1.0 / (double) bin_count)), 
			   ((i + 1) * (1.0 / (double) bin_count)), bins[i]);
	}

	printf("\nAverage discrepancy: %f\n", avg_discrepancy);
}
#endif

int main(void) 
{
#ifdef UNIT_TEST 
	_test_obj_import("res/porsche.obj");
	// _test_rng();
#endif
#ifndef UNIT_TEST
	_run();
#endif
	exit(0);
}
