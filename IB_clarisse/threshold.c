#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "threshold.h"
#include "pixel_functions.h"

long *histo;

void histogram(SDL_Surface *image_surface)
{
  //allocating enough memory according to the levels of gray existing
  histo = calloc(256, sizeof(long));
  
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  //variables for the rgb values of each pixel
  Uint8 r,g,b;

  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  //getting the pixel value
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	  *(histo + r) +=1;
	}
    }
}


//function that binarize, using Otsu method
int threshold(SDL_Surface *image_surface)
{
  //updating the histogram
  histogram(image_surface);

  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  //initializing some variables
  long total_pixels = width * height;

  int threshold = 0;
  double maxt = 0;

  double w_b = 0; //weight background (white pixels)
  double w_f = 0; //weight foreground (black pixels)

  double m_b = 0; //mean background
  double m_f = 0; //mean foreground
  
  double sum_b = 0;
  double sum_f = 0;

  double bcv = 0; //between class variance
  long sum_pixel_gray = 0; //used to calculate the means
  
  for(int t = 0 ; t < 256 ; t++)
    {
      sum_pixel_gray += t * *(histo + t); //lvl_of_gray * nb_of_pixels_of_this_lvl
    }

  for(int t = 0 ; t < 256 ; t++)
    {
      //weights
      w_b += *(histo+t); 
      w_f = (total_pixels - w_b);

      sum_b += t * *(histo+t); //used to calculate the means
      sum_f = sum_pixel_gray - sum_b;

      //means
      m_b = sum_b / w_b;
      m_f = sum_f / w_f;

      //calculating the new between class variance
      bcv = w_b * w_f * (m_b - m_f) * (m_b - m_f);

      if(bcv > maxt) //getting the maximum 
	{
	  maxt = bcv;
	  threshold = t;
	}
    }
  return threshold;
}

int binarize(SDL_Surface *image_surface)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  int t = threshold(image_surface);
  
  ////////////////////////////
  printf("threshold = %d\n", t);
  ////////////////////////////
  
  Uint8 r,g,b;

  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

	  if (r < t)
	    {
	      r = 0;
	      g = 0;
	      b = 0;
	    }
	  else
	    {
	      r = 255;
	      g = 255;
	      b = 255;
	    }
	  Uint32 newPixel = SDL_MapRGB(image_surface->format, r, g, b);
	  put_pixel(image_surface, j, i, newPixel);
	}
    }

  //saving the image
  SDL_SaveBMP(image_surface, "binarize.bmp");
  
  //freeing the image surface
  SDL_FreeSurface(image_surface);
  
  return 0;
}
