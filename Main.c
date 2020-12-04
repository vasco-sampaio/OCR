#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "image_segmentation/histo_segmentation/segmentation.h"
#include "preprocessing/preprocessing.h"

#include <stdlib.h>
#include <err.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  if(argc != 2)
    errx(1, "the number of arguments is not valid : you should have 2 arguments");

  SDL_Surface *image = preprocessing_SDL(argv[1]);
  char *text = segmentation_SDL(image);
  printf("text = \n%s\n", text);

  free(text);
  
  return 0;
}
