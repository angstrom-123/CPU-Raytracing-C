#include "camera.h"
#include "renderer.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(int argc, char *argv[]) 
{
	init_renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
	init_camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	set_pixel(20, 20, 0xFF00FF);
	update_render_window();

	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&e);
		if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) 
		{
			quit = true;
			close_render_window();
		}
	}
	return 0;
}

