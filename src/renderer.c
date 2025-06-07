#include "renderer.h"

#define BYTES_PER_PIXEL 4

static SDL_Window *window;
static SDL_Surface *surface;

/*
 * PRIVATE:
 */

static double to_gamma_cmp(double component)
{
	if (component > 0)
		return sqrt(component);
	return 0.0;
}

static void to_gamma(Vector* v)
{
	v->x = to_gamma_cmp(v->x);
	v->y = to_gamma_cmp(v->y);
	v->z = to_gamma_cmp(v->z);
}

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

void set_pixel(uint16_t x, uint16_t  y, Vector colour)
{
	double r, g, b;
	uint8_t* target_pixel;

	colour = vec_unit(colour);
	to_gamma(&colour);

	r = round(colour.x * 255.0);
	g = round(colour.y * 255.0);
	b = round(colour.z * 255.0);

	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;

	target_pixel = (uint8_t*) surface->pixels 
						  + y * surface->pitch 
						  + x * BYTES_PER_PIXEL;
	*(uint32_t*) target_pixel = (uint32_t) r << 16 | (uint32_t) g << 8 | (uint32_t) b;
}

int init_renderer(uint16_t screen_width, uint16_t screen_height)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Failed to initialize sdl3 window %s\n", SDL_GetError());
		return -1;
	}
	window = SDL_CreateWindow("Ray Tracing", screen_width, screen_height, 0);

	// SDL_SetWindowSize(window, screen_width * 2, screen_height * 2);

	if (window == NULL) return -1;

	surface  = SDL_GetWindowSurface(window);
	if (surface == NULL) return -1;

	SDL_FillSurfaceRect(surface , NULL, 0xFF00FF);
	return 0;
}
