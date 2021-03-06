//segmentation.c file

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
/*
#include "structures.h"
#include "utility.h"
#include "resizing.h"
*/
#include "../types.h"

/*
Regroups the functions that do the segmentation.
This segmentation uses histograms
*/

//------------------------------------------------------------------------------

/*
Function that does the vertical histogram and
draws the lines to show the lines of the text.
*/
void marking_lines(SDL_Surface *image_surface, int height, int width)
{
  int *histo;
  histo = calloc(height, sizeof(int));
  coord rect;
  rect.topL.h = 0;
  rect.topL.w = 0;
  rect.botR.w = width-1;
  rect.botR.h = height;
  verti_histo(image_surface, histo, rect);
  hori_lines(image_surface, histo, rect);
  free(histo);
}

/*
Function that gets the nb of lines and for each line, 
makes the horizontal histogram to separate the
letters, and draws the lines.
*/
lineZones marking_letters(SDL_Surface *image_surface, int w, int h)
{
  coord rect;
  rect.topL.h = 0;
  rect.topL.w = 0;
  rect.botR.w = w;
  rect.botR.h = h;
  int nbLines = count_get_lines(image_surface, rect);
  lineZones all = init_lineZones(nbLines);
  get_lines(image_surface, rect, all);
  for(int i = 0 ; i < nbLines ; i++)
    {
      int topLw = all.zones[i].topL.w;
      int botRw = all.zones[i].botR.w;
      int *histo2 = calloc(botRw - topLw, sizeof(int));
      hori_histo(image_surface, histo2, all.zones[i]);
      vert_lines(image_surface, histo2, all.zones[i]);
      free(histo2);
    }
  return all;
}

/*
Function that takes all the coordinates of the rectangles
that contains each letter.
*/
doc keep_letters(SDL_Surface *image_surface, lineZones all)
{
  doc image = init_doc(all.nbZones);
  for(int i = 0 ; i < all.nbZones ; i++)
    {
      int nbLetters = count_get_letters(image_surface, all.zones[i]);
      image.allLines[i] = init_line(nbLetters);
      get_letters(image_surface, all.zones[i], image.allLines[i]);
    }
  return image;
}

/*
Function that resizes the rectangle arround a letter but 
takes into account the accents and the points on the "i"
*/
void resize(SDL_Surface *image, coord letter)
{
  int len =letter.botR.h - letter.topL.h;
  int *histo = calloc(len, sizeof(int));
  verti_histo(image, histo, letter);
  tuples t = generate_groups(histo, len);
  hori_lines_define(image, histo, letter, t);
  free(t.list);
  free(histo);
}


/*
Function that resizes the rectangles arround the letters.
*/
void resize_letter(SDL_Surface *image_surface, doc image)
{
  for(int i = 0 ; i < image.nbLines ; i++)
    {
      sepLetters(image_surface, image.allLines[i]);
      
      for(int j = 0 ; j < image.allLines[i].nbLetters ; j++)
	{
	  resize(image_surface, image.allLines[i].letters[j]);
	}
    }
  detectSpaceDoc(&image);
}

//------------------------------------------------------------------------------

//Function that makes all the segmentation (as a main)

void segmentation(char *path, neunet_t *nn)
{
  SDL_Surface *image_surface;
  image_surface = IMG_Load(path);
  
  int height = image_surface->h;
  int width = image_surface->w;

  marking_lines(image_surface, height, width);
  SDL_SaveBMP(image_surface, "result_pictures/line_seg.bmp");
  
  lineZones all = marking_letters(image_surface, width, height);
  SDL_SaveBMP(image_surface, "result_pictures/line_seg2.bmp");

  doc image = keep_letters(image_surface, all);
  resize_letter(image_surface, image);

  SDL_SaveBMP(image_surface, "result_pictures/line_seg3.bmp");
  doc testtt = keep_letters(image_surface, all);

  detectSpaceDoc(&testtt);
  print_doc(&testtt);

  char *res = doc_string(image_surface, &testtt, nn);
  printf("\n%s", res);
  free(res);
  
  //freeing whatever needs to be freed
   SDL_FreeSurface(image_surface);
   free(all.zones);
   free_doc(image);
   free_doc(testtt);
   free(nn);
}

char* segmentation_SDL(SDL_Surface * image_surface, neunet_t *nn)
{ 
  int height = image_surface->h;
  int width = image_surface->w;

  marking_lines(image_surface, height, width);
  SDL_SaveBMP(image_surface, "result_pictures/line_seg.bmp");
  
  lineZones all = marking_letters(image_surface, width, height);
  SDL_SaveBMP(image_surface, "result_pictures/line_seg2.bmp");

  doc image = keep_letters(image_surface, all);
  resize_letter(image_surface, image);

  SDL_SaveBMP(image_surface, "result_pictures/line_seg3.bmp");
  doc testtt = keep_letters(image_surface, all);

  detectSpaceDoc(&testtt);
  print_doc(&testtt);
  char *res = doc_string(image_surface, &testtt, nn);
  
  //freeing whatever needs to be freed
   SDL_FreeSurface(image_surface);
   free(all.zones);
   free_doc(image);
   free_doc(testtt);
   free(nn);
   return res;
}


/*
Function that links the preprocessing, the segmentation and the
neural network together.
*/
char* ocr(char * image_path, char* neunet_path)
{
  SDL_Surface *image_surface = preprocessing_SDL(image_path);

  neunet_t *nn = fileToNeuralNet(neunet_path);

  char *res = segmentation_SDL(image_surface, nn);
  return res;
}

/*
Function useful to do the segmentation on the datasets
and to train the neural network.
*/
double* dataset(char *path)
{
  SDL_Surface *image_surface = preprocessing_dataset(path);
  int height = image_surface->h;
  int width = image_surface->w;
  
  //getting the letter through segmentation
  lineZones l = marking_letters(image_surface, width, height);
  SDL_SaveBMP(image_surface, "result_pictures/marking.bmp");
  resize(image_surface, l.zones[0]);
  SDL_SaveBMP(image_surface, "result_pictures/resize.bmp");
  doc data = keep_letters(image_surface, l);
  matrix letter = buildMatrix(image_surface, data.allLines[0].letters[0]);
  letter = normalization(&letter);
  //print_m(letter); //to debug
  SDL_SaveBMP(image_surface, "result_pictures/dataset.bmp");

  //freeing whatever needs to be freed
  SDL_FreeSurface(image_surface);
  free_doc(data);
  free(l.zones);
  
  return letter.mat;
}
