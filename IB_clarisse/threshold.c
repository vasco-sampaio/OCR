#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "threshold.h"
#include "pixel_functions.h"

long *histo;
//long *wcv; //to keep all the within class variances calculated (depending on the threshold)

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

//function that finds the minimum of an array
int minimumArray(long *tab, int len)
{
  int min = *tab;
  int rank = 0;
  for(int i = 0 ; i < len ; i++)
    {
      if(min > *(tab + i))
	{
	  min = *(tab + i);
	  rank = i;
	}
    }
  return rank;
}

//function that binarize, using Otsu method
int threshold(SDL_Surface *image_surface)
{
  //allocating enough memory according to the possible nb of thresholds
  long *wcv = calloc(256, sizeof(long));

  histogram(image_surface);
  
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  //initializing variables

  int total_pixels = width * height;
  
  int sum_b = 0;
  double w_b = 0; //weight
  double m_b = 0; //mean
  double v_b = 0; //variance
  
  int sum_f = 0;
  double w_f = 0; //weight
  double m_f = 0; //mean
  double v_f = 0; //variance

  //calculate the best threshold value
  for(int t = 0 ; t < 256 ; t++)
    {
      //for the background pixels (white ones)
      for(int i = t ; i < 256 ; i++)
	{
	  sum_b += *(histo+i);
	}

      w_b = sum_b / total_pixels; //weight

      for(int i = t ; i < 256 ; i++)
	{
	  m_b += i * *(histo + i);
	}
      m_b /= sum_b; //mean

      for(int i = t ; i < 256 ; i++)
	{
	  v_b += (i - m_b)*(i - m_b) * *(histo + i);
	}
      v_b /= sum_b; //variance
      

      //for the foreground pixels
      for(int i = 0 ; i < t ; i++)
	{
	  sum_f += *(histo + i);
	}

      w_f = 1 - w_b; //weight

      for(int i = 0 ; i < t ; i++)
	{
	  m_f += i * *(histo + i);
	}
      m_f /= sum_f; //mean

      for(int i = 0 ; i < t ; i++)
	{
	  v_f += (i - m_f)*(i - m_f) * *(histo + i);
	}
      v_f /= sum_f; //variance
          
      //calculating the Within Class Variance
      *(wcv+t) = w_b * v_b + w_f * v_f;

      ///////////////
      printf("thresh %d = %ld\n", t, *(wcv+t));
      //////////////
    }

  return minimumArray(wcv, 256);
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
