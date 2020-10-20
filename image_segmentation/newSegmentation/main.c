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

  int nbLines = count_get_lines(image_surface);
  printf("nbLines = %d\n", nbLines);
  lineZones all = init_lineZones(nbLines);

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

  doc image = init_doc(nbLines);
  for(int i = 0 ; i < nbLines ; i++)
    {
      int nbLetters = count_get_letters(image_surface, all.zones[i]);
      printf("nbLetters = %d", nbLetters);
      image.allLines[i] = init_line(nbLetters);
      get_letters(image_surface, all.zones[i], image.allLines[i]);
	
    }

  //testing if my doc functions work
  for(int i = 0 ; i < nbLines ; i++)
    {
      for(int j = 0 ; j < image.allLines[i].nbLetters ; j++)
	{
	  int botRh = image.allLines[i].letters[j].botRight.h;
	  int botRw = image.allLines[i].letters[j].botRight.w;
	  int topLh = image.allLines[i].letters[j].topLeft.h;
	  int topLw = image.allLines[i].letters[j].topLeft.w;
	  printf("botRh = %d\n", botRh);
	  printf("topLh = %d\n", topLh);
	  printf("botRw = %d\n", botRw);
	  printf("topLw = %d\n", topLw);
	  int *histo3 = calloc(botRh - topLh, sizeof(int));
	  verti_histo(image_surface, histo3, topLw, topLh, botRw, botRh);
	  hori_lines(image_surface, histo3, topLw, topLh, botRw, botRh);
	  free(histo3);
	}
    }

  //freeing whatever needs to be freed
   SDL_FreeSurface(image_surface);
   free(all.zones);
}
