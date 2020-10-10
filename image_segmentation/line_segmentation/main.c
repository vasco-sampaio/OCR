#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "line_seg.h"
#include "pixel_functions.h"
#include <stdlib.h>

int main()
{
  //iniatializing sdl
  //if cannot, returns 1
  if(SDL_Init(SDL_INIT_VIDEO) == -1)
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  
  SDL_Surface *image_surface;
  image_surface = IMG_Load("binarize.bmp");
  int height = image_surface->h;

  long *histo;
  int *lines;

  histo = calloc(height, sizeof(long));

  vertHistogram(image_surface, histo);
  //moving_average(height, histo, 5);

  size_t nbLines = line_seg_count(histo, height);
  //printf("nbLines = %ld\n", nbLines);
  lines = calloc(nbLines, sizeof(size_t));
  
  line_seg(histo, height, lines);

  line_seg_drawing(image_surface, lines, nbLines);

  //freeing the image surface, histo and lines
  SDL_FreeSurface(image_surface);
  free(histo);
  free(lines);
}
