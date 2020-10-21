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

  marking_lines(image_surface, height, width);
  SDL_SaveBMP(image_surface, "line_seg.bmp");
  
  lineZones all = marking_letters(image_surface, width, height);
  SDL_SaveBMP(image_surface, "line_seg2.bmp");

  doc image = keep_letters(image_surface, all);
  resize_letter(image_surface, image);
  SDL_SaveBMP(image_surface, "line_seg3.bmp");
  
  //freeing whatever needs to be freed
   SDL_FreeSurface(image_surface);
   free(all.zones);
   free(image.allLines);
}
