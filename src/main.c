#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <stdlib.h>

#include "renderer.h"
#include "vec.h"

int main(int argc, char *argv[]) 
{
	RND_Init();
	RND_SetPixel(20, 20, 0xFF00FF);
	RND_UpdateWindow();

	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&e);
		if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) 
		{
			quit = true;
			RND_Quit();
		}
	}
	return 0;
}

