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

//Functions that creates the histograms

/*
Function that creates a vertical histogram that counts the foreground 
pixels (black pixels) of each row, of a given rectangle.
*/
void verti_histo(SDL_Surface *image_surface, int *histo, coord rect)
{
  int ind = 0;
  for(int i = rect.topL.h ; i < rect.botR.h ; i++)
    {
      for(int j = rect.topL.w ; j < rect.botR.w ; j++)
	{
	  if (is_black(image_surface, j, i) == 1)
	      *(histo + ind) += 1;
	}
      ind++;
    }
}

/*
Function that creates a horizontal histogram that counts the foreground 
pixels of each column, of a given rectangle.
 */
void hori_histo(SDL_Surface *image_surface, int *histo, coord rect)
{
  int ind = 0;
  for(int j = rect.topL.w ; j < rect.botR.w ; j++)
    {
      for(int i = rect.topL.h ; i < rect.botR.h ; i++)
	{
	  if(is_black(image_surface, j, i) == 1)
	    *(histo + ind) += 1;
	}
      ind++;
    }
}
