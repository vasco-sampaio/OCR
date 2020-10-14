#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "char_seg.h"
#include "pixel_functions.h"
#include <stdlib.h>

int main()
{
  //iniatializing sdl
  //if cannot, returns 1
  if(SDL_Init(SDL_INIT_VIDEO) == -1)
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  
  SDL_Surface *image_surface;
  image_surface = IMG_Load("test1.jpg");
  int width = image_surface->w;

  long *histo;
  int *chars;

  histo = calloc(width, sizeof(long));

  horiHistogram(image_surface, histo);
  //moving_average(width, histo, 15);
  //int a = average_black_pixels(width, histo);

  size_t nbChars = char_seg_count(histo, width);
  chars = calloc(nbChars, sizeof(size_t));

  char_seg(histo, width, chars);

  char_seg_drawing(image_surface, chars, nbChars);

  //freeing the image surface, histo and chars
  SDL_FreeSurface(image_surface);
  free(histo);
  free(chars);
}
