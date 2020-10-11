#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"

/*
This file is in charge of the character segmentation
*/


//function that checks if a pixel is black in a binarized image
int isBlack(SDL_Surface *image_surface, Uint32 pixel)
{
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
  
  //checks if the pixel is black
  //if it is, returns 0
  if (r == 0) 
    return 0;
  return 1;
}


/*
Function that builds the horizontal histogram of a binarized image.

The array is of the size of the number of columns in
the image (width).

This histogram will count the nb of black pixels in each column.
*/
void horiHistogram(SDL_Surface *image_surface, long *histo)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  if (isBlack(image_surface, pixel) == 0)
	    *(histo + j) +=1;
	}
    }  
}


//This function calculates the average number of black pixels
//in the histogram.
int average_black_pixels(int len, long *histo)
{
  int a = 0;
  for(int i = 0 ; i < len ; i++)
    {
      a += *(histo + i);
    }
  return a / len;
}
