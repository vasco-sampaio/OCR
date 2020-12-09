#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "segmentation.h"
#include "pixel_functions.h"
#include "utility.h"
#include "structures.h"
#include "matrix_letters.h"
#include "resizing.h"

#include "../neunet_HD/neural_main.h"
#include "../preprocessing/preprocessing.h"

#include <stdlib.h>
#include <err.h>

int main(int argc, char** argv)
{
  if(argc != 3)
    errx(1, "the number of arguments is not valid : you should have 3 arguments");
  
  neunet_t *nn = fileToNeuralNet(argv[2]);
  segmentation(argv[1], nn);

  /*SDL_Surface *image_surface;
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
  doc testtt = keep_letters(image_surface, all);

  docMat test = buildDocMat(image_surface, testtt);
  print_doc(&testtt);
  
  //freeing whatever needs to be freed
   SDL_FreeSurface(image_surface);
   free(all.zones);
   free_doc(image);
   free_doc(testtt);
   free_docMat(test);*/
  return 0;
}
