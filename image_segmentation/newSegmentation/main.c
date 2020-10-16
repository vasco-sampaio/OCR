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

  int nbLines = count_get_lines(image_surface);
  printf("nbLines = %d\n", nbLines);
  lineZones all = init_lineZones(nbLines);
  //all.zones = calloc(nbLines, sizeof(coord));
  get_lines(image_surface, all);
  for(int i = 0 ; i < nbLines ; i++)
    {
      printf("i = %d\n", i);
      int topLw = all.zones[i].topLeft.w;
      int topLh = all.zones[i].topLeft.h;
      int botRw = all.zones[i].botRight.w;
      int botRh = all.zones[i].botRight.h;
      int *histo2 = calloc(botRw - topLw, sizeof(int));
      hori_histo(image_surface, histo2, topLw, topLh, botRw, botRh);
      vert_lines(image_surface, histo2, topLw, topLh, botRw, botRh);
      free(histo2);
    }
  
  SDL_SaveBMP(image_surface, "line_seg2.bmp");

  //freeing whatever needs to be freed
   SDL_FreeSurface(image_surface);
   free(histo);
}
