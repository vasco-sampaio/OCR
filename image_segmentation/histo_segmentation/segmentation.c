//segmentation.c file

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "segmentation.h"

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
