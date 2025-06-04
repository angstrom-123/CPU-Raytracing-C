#ifndef CAMERA_H
#define CAMERA_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <stdio.h>

extern void CAM_SetPixel(int x, int y, uint32_t colour);
extern void CAM_UpdateWindow();
extern int CAM_Init();
extern void CAM_Quit();

#endif
