#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "preprocessing.h"

/*
This file is in charge of the image pre-processing.
We can :
-put the image in grayscale
-binarize the image
*/

/*
Function that puts the image in grayscale.
*/
void toGrayscale(SDL_Surface *image_surface, int w, int h)
{
  for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i); 
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b); 
	  int  av = 0.3*r + 0.59*g + 0.11*b;
	  pixel = SDL_MapRGB(image_surface->format, av, av, av);
	  put_pixel(image_surface, j, i, pixel);
	}
    }
}

/*
Function that builds the grey histogram of the image
*/
void histogram(SDL_Surface *image_surface, int w, int h, long *histo)
{
  Uint8 r,g,b;

  for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  //getting the pixel value
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	  *(histo + r) +=1;
	}
    }
}


/*
Function that calculates the sum of :
lvl_of_gray * nb_of_pixels_of_this_lvl
*/
long calcul_sum_gray(long *histo)
{
  long res = 0;
  for(int t = 0 ; t < 256 ; t++)
    {
      res += t * *(histo + t);
    }
  return res;
}


/*
Function that initialises the double couples
*/
couple init_couple()
{
  couple res;
  res.b = 0; //for the foreground pixels (black pixels)
  res.f = 0; //for the background pixels (white pixels)
  return res;
}

/*
Function that calculates the new weights
*/
couple cal_weight(couple w, int t, int total, long *histo)
{
  w.b += *(histo + t);
  w.f = total - w.b;
  return w;
}

/*
Function that calculates the sums that are used to calculate 
the means.
*/
couple cal_sum(couple s, int t, long *histo, long sum_gray)
{
  s.b += t * *(histo + t);
  s.f = sum_gray - s.b;
  return s;
}


/*
Function that calculates the new means.
*/
couple cal_mean(couple m, couple s, couple w)
{
  m.b = s.b / w.b;
  m.f = s.f / w.f;
  return m;
}

/*
Function that calculates the threshold for
the binarization
*/
int threshold(SDL_Surface *image_surface, int w, int h, long *histo)
{
  histogram(image_surface, w, h, histo);
  long total_pixels = w * h;
  int threshold = 0;
  double maxt = 0;
  couple weight = init_couple();
  couple mean = init_couple();
  couple sum = init_couple();
  double bcv = 0; //between class variance
  long sum_pixel_gray = calcul_sum_gray(histo); //used to calculate the means
  for(int t = 0 ; t < 256 ; t++)
    {
      weight = cal_weight(weight, t, total_pixels, histo);
      sum = cal_sum(sum, t, histo, sum_pixel_gray);
      mean = cal_mean(mean, sum, weight);
      bcv = weight.b * weight.f * (mean.b - mean.f) * (mean.b - mean.f);
      if(bcv > maxt) //getting the maximum 
	{
	  maxt = bcv;
	  threshold = t;
	}
    }
  return threshold;
}


/*
Function that binarizes the image.
*/
void binarize(SDL_Surface *image_surface, int w, int h, long *histo)
{
  int t = threshold(image_surface, w, h, histo);
  Uint8 r, g, b;
   for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	  if (r < t)
	    pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
	  else
	    pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
	  put_pixel(image_surface, j, i, pixel);
	}
    }
}
