#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <stdlib.h>
#include <stdio.h>

#include "math_utils.h"

extern void set_pixel(uint16_t x, uint16_t y, Vector colour);
extern void update_render_window(void);
extern void init_renderer(uint16_t screen_width, uint16_t screen_height);
extern void close_render_window(void);

#endif
