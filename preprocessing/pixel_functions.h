#ifndef PIXEL_FUNCTIONS_H_
#define PIXEL_FUNCTIONS_H_

# include <SDL/SDL.h>
# include <SDL/SDL_image.h>


Uint32 get_pixel1(SDL_Surface *surface, int x, int y);
void put_pixel1(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif
