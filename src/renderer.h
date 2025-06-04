#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <stdio.h>

extern void RND_SetPixel(int x, int y, uint32_t colour);
extern void RND_UpdateWindow();
extern int RND_Init();
extern void RND_Quit();

#endif
