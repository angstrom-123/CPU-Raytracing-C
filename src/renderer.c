#include "renderer.h"

#define BYTES_PER_PIXEL 4

static SDL_Window *window;
static SDL_Surface *surface;

/*
 * PUBLIC:
 */
void update_render_window(void)
{
	SDL_UpdateWindowSurface(window);
}

void close_render_window(void)
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void set_pixel(size_t x, size_t  y, Vector colour)
{
	colour.x = (colour.x < 0.0) ? 0.0 : sqrt(colour.x);
	colour.y = (colour.y < 0.0) ? 0.0 : sqrt(colour.y);
	colour.z = (colour.z < 0.0) ? 0.0 : sqrt(colour.z);

	uint32_t r = round(colour.x * 255.0);
	uint32_t g = round(colour.y * 255.0);
	uint32_t b = round(colour.z * 255.0);

	uint8_t* pixel = (uint8_t*) surface->pixels 
				   + y * surface->pitch 
				   + x * BYTES_PER_PIXEL;
	*(uint32_t*) pixel = (r << 16) | (g << 8) | (b);
}

void init_renderer(size_t screen_width, size_t screen_height)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "\nFailed to initialize sdl3 window %s\n", SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow("Ray Tracing", screen_width, screen_height, 0);
	if (window == NULL) exit(1);

#ifdef DEBUG
	SDL_SetWindowSize(window, screen_width * 4.0, screen_height * 4.0);
#endif

	surface  = SDL_GetWindowSurface(window);
	if (surface == NULL) exit(1);

	SDL_FillSurfaceRect(surface , NULL, 0xFF00FF);
}
