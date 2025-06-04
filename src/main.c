#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include "camera.h"

int main(int argc, char *argv[]) 
{
	CAM_Init();
	CAM_SetPixel(20, 20, 0xFF00FF);
	CAM_UpdateWindow();

	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		SDL_PollEvent(&e);
		if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) 
		{
			quit = true;
			CAM_Quit();
		}
	}
	return 0;
}

