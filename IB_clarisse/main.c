#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "grayscale.h"
#include "pixel_functions.h"

int main()
{
   //iniatializing sdl
  //if cannot, returns 1
  if(SDL_Init(SDL_INIT_VIDEO) == -1)
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  
  SDL_Surface *image_surface;
  image_surface = IMG_Load("test_image.jpg");

  toGrayscale(image_surface);
  
  return 0;
}
