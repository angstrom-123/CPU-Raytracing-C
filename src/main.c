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

int main(void) 
{
	const uint16_t screen_width = 854;
	const uint16_t screen_height = 480;

	bool debug_movement = false;

	init_renderer(screen_width, screen_height);

	Camera* cam = malloc(sizeof(Camera));
	cam_init(cam, screen_width, screen_height);

	Vector col_gray = {0.5, 0.5, 0.5};
	Material diff_gray = {DIFFUSE, col_gray};

	Hittable* sphere_a = new_hittable_xyz(SPHERE, 0.0, 0.0, -2.0, 
										  0.5, diff_gray);
	Hittable* sphere_b = new_hittable_xyz(SPHERE, 0.0, -40.5, -2.0, 
										  40.0, diff_gray);

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
	exit(0);
}

