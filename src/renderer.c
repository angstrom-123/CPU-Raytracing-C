#include "renderer.h"

#define BYTES_PER_PIXEL 4

static SDL_Window *window;
static SDL_Surface *surface;

/*
 * PUBLIC:
 */

/*
 * Refreshes the SDL window surface that holds the pixel buffer that is displayed.
 */
void update_render_window(void)
{
	SDL_UpdateWindowSurface(window);
}

/*
 * Closes the render window.
 */
void close_render_window(void)
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/*
 * Sets the colour of a pixel at the coordinates (x, y). The colour passed in 
 * is a vector where the components represent red, green, and blue. This vector 
 * should be normalized (each component in the range 0-1) however the method does
 * not break with unnormalized values. 
 *
 * Gamma correction is performed on the colour passed in before the pixel is 
 * written to the screen.
 */
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

/*
 * Initializes the SDL window used for displaying the render result. If any SDL 
 * initialization methods fail, the application exits with code 1.
 *
 * If the application is built in debug mode, then the window is scaled up by a 
 * factor of 4, as the image that is rendered is of reduced resolution to speed 
 * up debugging. In release mode, the window is at the same resolution as the 
 * rendered image.
 *
 * On initialization, the window is filled with a magenta colour.
 */
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

	surface = SDL_GetWindowSurface(window);
	if (surface == NULL) exit(1);

	SDL_FillSurfaceRect(surface , NULL, 0xFF00FF);
}
