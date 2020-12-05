#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "preprocessing.h"
#include "pixel_functions.h"

int main(int argc, char** argv)
{
  if(argc != 2)
    errx(1, "the number of arguments is not valid : you should have 2 arguments");

  preprocessing(argv[1]);

  SDL_Surface *image = IMG_Load(argv[1]);
  int height = image->h;
  int width = image->w;
  SDL_Surface * rot = rotate(image, width, height, 90);
  
  SDL_SaveBMP(rot, "rotate.bmp");

  SDL_FreeSurface(image);
  SDL_FreeSurface(rot);
  /*SDL_Surface *image_surface = IMG_Load(argv[1]);
  int height = image_surface->h;
  int width = image_surface->w;
  double mat[height*width]; //matrix that reprensents the image
  long *histo = calloc(256, sizeof(long));
  
  contrast(image_surface, 50, width, height);
  SDL_SaveBMP(image_surface, "contrast.bmp");

  toGrayscale(image_surface, width, height);
  SDL_SaveBMP(image_surface, "grayscale.bmp");

  reduce_noise(image_surface, width, height);
   SDL_SaveBMP(image_surface,"noise.bmp");

  binarize(image_surface, width, height, histo,mat);
  SDL_SaveBMP(image_surface, "binarize.bmp");
  Print the matrix of the image
  for(int i = 0; i < height;i++)
  {
	for(int j = 0; j < width;j++)
	  {
		  printf("%d ",(int)mat[i*width+j]);
	  }
	  printf("\n");
  }
  free(histo);
  SDL_FreeSurface(image_surface);*/
  
  return 0;
}
