#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <stdlib.h>
#include <stdio.h>

#include "math_utils.h"

/*
 * Assigns the given colour to the pixel at the given coordinates.
 */
extern void set_pixel(size_t x, size_t y, Vector colour);

/*
 * Refreshes the render window to see any changes.
 */
extern void update_render_window(void);

/*
 * Initializes the render window with given dimensions.
 */
extern void init_renderer(size_t screen_width, size_t screen_height);

/*
 * Closes the render window.
 */
extern void close_render_window(void);

#endif
