#include <SDL3/SDL_timer.h>
#ifndef UNIT_TEST
#include "scene_builder.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"

#include <stdlib.h>
#include <SDL3/SDL_events.h>

void run(void)
{
	uint16_t screen_width = 100;
	uint16_t screen_height = 100;
#ifdef DEBUG
	screen_width = 400;
	screen_height = 226;
#elif RELEASE
	screen_width = 800;
	screen_height = 450;
#endif

	init_renderer(screen_width, screen_height);

	Camera* cam = malloc(sizeof(Camera));
	cam_init(cam, screen_width, screen_height);

	Hittable_List* scene = build_demo_scene();

	// cam_render(&set_pixel, cam, scene, screen_width, screen_height);
	// update_render_window();

	bool quit = false;
	bool render = true;
	SDL_Event e;
	uint16_t ctr = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			{
				printf("\n");
				quit = true;
			}
		}

		if (render)
		{
			uint16_t start_y = (uint16_t) (ctr * 4);
			uint16_t end_y = (uint16_t) (start_y + 5);
			if (end_y >= screen_height) 
			{
				end_y = screen_height;
				render = false;
			}

			double tmp = ((double) end_y / (double) screen_height) * 100.0;
			uint8_t percent_complete = (uint8_t) round(tmp);
			printf("\r%hhu%%", percent_complete);
			fflush(stdout);

			cam_render_range(&set_pixel, cam, scene, 0, start_y, screen_width, end_y);
			update_render_window();

			if (render == false) printf("\rRender complete\n");
			ctr++;
		}
	}
	free(cam);
	free(scene);
	close_render_window();
}
#endif

#ifdef UNIT_TEST
#include "random.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

void test(void)
{
	printf("Running Tests\n");

	printf("Testing rng distribution:\n");
	rng_set_seed(time(NULL));
	uint32_t test_count = 100000000;
	uint32_t bins[50] = {0};
	uint16_t bin_count = sizeof(bins) / sizeof(bins[0]);
	for (uint32_t i = 0; i < test_count; i++)
	{
		double val = rng_01();
		for (uint16_t j = 0; j < bin_count; j++)
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
	for (uint16_t i = 0; i < bin_count; i++)
	{
		if (bins[i] < expected) total_discrepancy += expected - bins[i];
		else total_discrepancy += bins[i] - expected;
	}
	double avg_discrepancy = (double) total_discrepancy / (double) bin_count;

	for (uint16_t i = 0; i < bin_count; i++)
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
	test();
#endif
#ifndef UNIT_TEST
	run();
#endif
	exit(0);
}
