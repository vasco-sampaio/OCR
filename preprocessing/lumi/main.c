#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
#include <SDL.h>
#include "pixel_operations.h"
#include "illum_compensation.h"
//#include "sdl_functions.h"
//#include "preprocessing.h"
#include <err.h>

SDL_Surface* load_image(char *path)
{
  SDL_Surface *img = IMG_Load(path);
  if (!img)
     errx(3, "Can't load %s: %s", path, IMG_GetError());

  return img;
}

int main(int argc, char** argv)
{
	if(argc != 2)
		errx(1, "2 arguments needed!");

	SDL_Surface* image_surface1;
  SDL_Surface* image_surface2;

  image_surface1 = load_image(argv[1]);
  image_surface2 = load_image(argv[1]); //maybe a better way to make them equal?

  int w = image_surface1->w;
  int h = image_surface1->h;

  int* grayscale = malloc(w * h * sizeof(int));

  toGrayscale(image_surface1, grayscale, w, h);
  extrm mmg = sobelEdge(image_surface1, grayscale);

  binarize(image_surface1, mmg.grid, mmg.min, mmg.max);

  toGrayscale(image_surface2, grayscale, w, h);
  contrastBin(image_surface2, 50, w, h);

  SDL_SaveBMP(image_surface1, "EIbin.bmp");
  SDL_SaveBMP(image_surface2, "CEIbin.bmp");

  mergeSurfaces(image_surface1, image_surface2);

  SDL_SaveBMP(image_surface1, "Merged.bmp");

  erosionMask(image_surface1);

  SDL_SaveBMP(image_surface1, "Erosion.bmp");

  //free surface 2 is done in merge surfaces
  free(grayscale);
  SDL_FreeSurface(image_surface1);
  SDL_FreeSurface(image_surface2);

  return 0;
}