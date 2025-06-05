#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include "renderer.h"

int main(int argc, char *argv[]) 
{
	init_renderer();
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

