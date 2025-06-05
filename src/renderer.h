#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <stdio.h>

extern void set_pixel(int x, int y, uint32_t colour);
extern void update_render_window();
extern int init_renderer(int screen_width, int screen_height);
extern void close_render_window();

#endif
