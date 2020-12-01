#ifndef PIXEL_FUNCTIONS_H_
#define PIXEL_FUNCTIONS_H_

#include <stdlib.h>
#include <SDL.h>

Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void swap(int *a, int *b);
void trace_hori_red_line(SDL_Surface *surface, int startH, int startW, int endH, int endW);
void trace_vert_red_line(SDL_Surface *surface, int startH, int startW, int endH, int endW);

#endif
