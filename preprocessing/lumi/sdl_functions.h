#ifndef SDL_FUNCTIONS_H_
#define SDL_FUNCTIONS_H_

#include <stdlib.h>
#include <SDL.h>

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);

#endif