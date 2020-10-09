#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"

//This file is in charge of doing the vertical histogram
//of a binarized image.
//
//The array is of the size of the number of rows in
//the image (height).
//
//This histogram will count the nb of black pixels in each row

long *v_histo;

int isBlack(SDL_surface *image_surface, Uint32 pixel)
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

void vertHistogram(SDL_Surface *image_surface)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  v_histo = calloc(height, sizeof(long));

  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  if (isBlack(image_surface, pixel) == 0)
	    *(v_histo + i) +=1;
	}
    }
    
}
