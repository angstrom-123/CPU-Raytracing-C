#include "renderer.h"

#define BYTES_PER_PIXEL 4
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

SDL_Window *window;
SDL_Surface *surface;

// sets a pixel at the coords to the colour
void RND_SetPixel(int x, int y, uint32_t colour)
{
	uint8_t *target_pixel = (uint8_t *) surface->pixels 
							+ y * surface->pitch 
							+ x * BYTES_PER_PIXEL;
	*(uint32_t *) target_pixel = colour;
}

// refreshes the window
void RND_UpdateWindow()
{
	SDL_UpdateWindowSurface(window);
}

// creates a window and screen surface to write pixels to 
int RND_Init()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Failed to initialize sdl3 window %s\n", SDL_GetError());
		return -1;
	}
	window = SDL_CreateWindow("Ray Tracing", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window == NULL) return -1;

	surface  = SDL_GetWindowSurface(window);
	if (surface == NULL) return -1;

	SDL_FillSurfaceRect(surface , NULL, 0xFFFFFF);
	return 0;
}

// destroys the sdl window
void RND_Quit()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}
