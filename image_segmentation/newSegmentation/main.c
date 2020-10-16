#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "segmentation.h"
#include "pixel_functions.h"

#include <stdlib.h>
#include <err.h>

int main(int argc, char** argv)
{
  if(argc != 2)
    errx(1, "the number of arguments is not valid : you should have 2 arguments");

  SDL_Surface *image_surface;
  image_surface = IMG_Load(argv[1]);

  int height = image_surface->h;
  int width = image_surface->w;

  int *histo;
  histo = calloc(height, sizeof(int));
  verti_histo(image_surface, histo, 0, 0, width-1, height-1);
  hori_lines(image_surface, histo, 0, 0, width-1, height-1);

  SDL_SaveBMP(image_surface, "line_seg.bmp");

  int *histo2;
  histo2 = calloc(width, sizeof(int));
  hori_histo(image_surface, histo2, 0, 0, width-1, height-1);
  vert_lines(image_surface, histo2, 0, 0, width-1, height-1);

  SDL_SaveBMP(image_surface, "line_seg2.bmp");

  //freeing whatever needs to be freed
   SDL_FreeSurface(image_surface);
   free(histo);
   free(histo2);
}
