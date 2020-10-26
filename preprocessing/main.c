#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "preprocessing.h"
#include "pixel_functions.h"

int main(int argc, char** argv)
{
  if(argc != 2)
    errx(1, "the number of arguments is not valid : you should have 2 arguments");

  SDL_Surface *image_surface;
  image_surface = IMG_Load(argv[1]);
  SDL_Surface *im = IMG_Load(argv[1]);
  int height = image_surface->h;
  int width = image_surface->w;
  long *histo = calloc(256, sizeof(long));
  toGrayscale(image_surface, width, height);
  //SDL_SaveBMP(image_surface, "grayscale.bmp");
  reduce_noise(image_surface, im, width, height);
  //  SDL_SaveBMP(image_surface,"noise.bmp");
  /*
  binarize(image_surface, width, height, histo);
  SDL_SaveBMP(image_surface, "binarize.bmp");*/
  SDL_FreeSurface(image_surface);
  free(histo);
  return 0;
}
