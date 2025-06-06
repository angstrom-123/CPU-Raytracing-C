#include "camera.h"
#include "hittable.h"
#include "renderer.h"
#include "scene.h"

int main(int argc, char *argv[]) 
{
	const uint16_t screen_width = 854;
	const uint16_t screen_height = 480;

	init_renderer(screen_width, screen_height);

	Camera* cam = malloc(sizeof(Camera));
	init_camera(cam, screen_width, screen_height);

	Hittable_List scene = init_scene();

	Vector position = {0.0, 0.0, -1.0};
	double scale = 0.5;
	Vector albedo = {1.0, 1.0, 1.0};
	Hittable sphere = {SPHERE, position, scale, albedo};

	add_to_scene(&scene, &sphere);

	render(&set_pixel, cam, &scene, screen_width, screen_height);
	update_render_window();

	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&e);
		if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) 
			quit = true;
		SDL_Delay(10);
	}
	free(cam);
	close_render_window();
	return 0;
}

