#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "preprocessing.h"
#include <math.h>

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
	  Uint32 pixel = get_pixel1(image_surface, j, i); 
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b); 
	  int  av = 0.3*r + 0.59*g + 0.11*b;
	  pixel = SDL_MapRGB(image_surface->format, av, av, av);
	  put_pixel1(image_surface, j, i, pixel);
	}
    }
}

SDL_Surface* rotate(SDL_Surface *image, int w, int h, double angle)
{
  int cw = w / 2;
  int ch = h / 2;

  double a_cos = cos(angle);
  double a_sin = sin(angle);

  int distw;
  int disth;

  int resw;
  int resh;

  Uint32 pixel;

  SDL_Surface *is2 = SDL_CreateRGBSurface(0, w, h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
  SDL_BlitSurface(image, NULL, is2, NULL);

  for(int i = 0 ; i < w ; i++)
    {
      distw = i - cw;
      for(int j = 0 ; j < h ; j++)
	{
	  disth = j - ch;
	  resw = distw*a_cos - disth*a_sin + cw;
	  resh = distw*a_sin + disth*a_cos + ch;
	  pixel = get_pixel1(image, resw, resh);
	  put_pixel1(is2, i, j, pixel); 
	}

    }
  return is2;
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
	  Uint32 pixel = get_pixel1(image_surface, j, i);
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
    { res += t * *(histo + t);
    }
  return res;
}


/*
  Function that initialises the double couples
*/
duo init_duo()
{
  duo res;
  res.b = 0; //for the foreground pixels (black pixels)
  res.f = 0; //for the background pixels (white pixels)
  return res;
}

/*
  Function that calculates the new weights
*/
duo cal_weight(duo w, int t, int total, long *histo)
{
  w.b += *(histo + t);
  w.f = total - w.b;
  return w;
}

/*
  Function that calculates the sums that are used to calculate 
  the means.
*/
duo cal_sum(duo s, int t, long *histo, long sum_gray)
{
  s.b += t * *(histo + t);
  s.f = sum_gray - s.b;
  return s;
}


/*
  Function that calculates the new means.
*/
duo cal_mean(duo m, duo s, duo w)
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
  duo weight = init_duo();
  duo mean = init_duo();
  duo sum = init_duo();
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
Function that checks if a function is black.
*/
int is_pixel_black(SDL_Surface *image, int i, int j)
{
  Uint32 pixel = get_pixel1(image, i, j);
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image->format, &r, &g, &b);
  if (r == 0)
    return 0;
  return 1;
}


/*
  Function that binarizes the image and fill the matrix if the information is in black.
*/
void binarize_b(SDL_Surface *image_surface, int w, int h, long *histo)
{
  int t = threshold(image_surface, w, h, histo)+1;
  printf("threshold = %d\n", t);
  Uint8 r, g, b;
  for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  Uint32 pixel = get_pixel1(image_surface, j, i);
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	  if (r < t)
	    {
	      pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
	    }
	  else
	    {
	      pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
	    }
	  put_pixel1(image_surface, j, i, pixel);
	}
    }
}

/*
  Function that binarizes the image and fill the matrix if the information is in white.
*/
void binarize_w(SDL_Surface *image_surface, int w, int h, long *histo)
{
  int t = threshold(image_surface, w, h, histo)+1;
  printf("threshold = %d\n", t);
  Uint8 r, g, b;
  for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  Uint32 pixel = get_pixel1(image_surface, j, i);
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	  if (r >= t)
	    {
	      pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
	    }
	  else
	    {
	      pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
	    }
	  put_pixel1(image_surface, j, i, pixel);
	}
    }
}


/*
Function that binarizes the images and choose the way to binarize,
depending on where the information is.
*/
void binarize(SDL_Surface *image_surface, int w, int h, long *histo)
{
  int black_corners = 0;
  black_corners += is_pixel_black(image_surface, 0, 0);
  black_corners += is_pixel_black(image_surface, 0, h-1);
  black_corners += is_pixel_black(image_surface, w-1, 0);
  black_corners += is_pixel_black(image_surface, w-1, h-1);

  if (black_corners >=2)
    binarize_b(image_surface, w, h, histo);
  else
    binarize_w(image_surface, w, h, histo);
}

/*
  Function that sorts an array
*/
void sort(Uint8 *arr, int len)
{
  for(int i = len-1 ; i > 0 ; i--)
    {
      for(int j = 0 ; j < i ; j++)
	{
	  if (*(arr+j) > *(arr+j+1))
	    {
	      Uint8 tmp = *(arr + j);
	      *(arr +j) = *(arr + j + 1);
	      *(arr + j +1) = tmp;
	    }
	}
    }
}


/*
  Function that gets the median of a given list
*/
Uint8 get_median(Uint8 *arr, int len)
{
  if (len%2)
    return arr[len/2];
  Uint8 a1 = arr[len/2];
  Uint8 a2 = arr[len/2+1];
  return (a1 + a2)/2;

}


Uint8 get_average(Uint8 *arr, int len)
{
  unsigned int sum = 0;
  for(int i = 0; i < len; ++i)
    {
      sum += (unsigned int) arr[i];
    }
  return (Uint8) (sum / len);
}



/*
  Function that reduces noise in the image using medians
*/
void reduce_noise(SDL_Surface *is, int w, int h)
{
  SDL_Surface *is2 = SDL_CreateRGBSurface(0, w, h, is->format->BitsPerPixel, is->format->Rmask, is->format->Gmask, is->format->Bmask, is->format->Amask);
  SDL_BlitSurface(is, NULL, is2, NULL);
  for(int i = 0 ; i < w ; i++)
    {
      for(int j = 0 ; j < h ; j++)
	{
	  int k = 1;
	  Uint8 tmp = 0;
	  Uint8 tmp2 = 0;
	  Uint8 *pixels_val = calloc(5, sizeof(Uint8));
	  SDL_GetRGB(get_pixel1(is2, i, j), is2->format, pixels_val, &tmp, &tmp2);
	  if (i + 1 < w)
	    {
	      SDL_GetRGB(get_pixel1(is2, i+1, j), is2->format, pixels_val + k, &tmp, &tmp2);
	      k++;
	    }
	  if (i - 1 >= 0)
	    {
	      SDL_GetRGB(get_pixel1(is2, i-1, j), is2->format, pixels_val + k, &tmp, &tmp2);
	      k++;
	    }
	  if(j + 1 < h)
	    {
	      SDL_GetRGB(get_pixel1(is2, i, j+1), is2->format, pixels_val + k, &tmp, &tmp2);
	      k++;
	    }
	  if(j - 1 >= 0)
	    {
	      SDL_GetRGB(get_pixel1(is2, i, j-1), is2->format, pixels_val + k, &tmp, &tmp2);
	      k++;
	    }
	  sort(pixels_val, k);
	  Uint8 median = get_average(pixels_val, k);
	  Uint32 pixel = SDL_MapRGB(is->format, median, median, median);
	  put_pixel1(is, i, j, pixel);
	  free(pixels_val);
	}
    }
  SDL_FreeSurface(is2);
}


/*
  Function that truncates a value
*/
Uint8 truncate(int val)
{
  if (val < 0)
    return 0;
  if (val > 255)
    return 255;
  return val;
}

/*
  Function that modifies the contrast of the image
*/
void contrast(SDL_Surface *im, int lvlc, int w, int h)
{
  double factor = (259.0 * (lvlc + 255.0))/(255.0 * (259.0 - lvlc));
  Uint8 r, g, b;
  for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  SDL_GetRGB(get_pixel1(im, j, i), im->format, &r, &g, &b);
	  r = truncate(factor * (r - 128) + 128);
	  g = truncate(factor * (g - 128) + 128);
	  b = truncate(factor * (b - 128) + 128);
	  put_pixel1(im, j, i, SDL_MapRGB(im->format, r, g, b));
	}
    }
}

//------------------------------------------------------------------------------

//Function that does the whole preprocessing for an image

void preprocessing(char *path)
{
  SDL_Surface *image_surface = IMG_Load(path);
  int height = image_surface->h;
  int width = image_surface->w;
  
  long *histo = calloc(256, sizeof(long));
  
  contrast(image_surface, 50, width, height);
  SDL_SaveBMP(image_surface, "contrast.bmp");

  toGrayscale(image_surface, width, height);
  SDL_SaveBMP(image_surface, "grayscale.bmp");

  reduce_noise(image_surface, width, height);
  SDL_SaveBMP(image_surface,"noise.bmp");

  binarize(image_surface, width, height, histo);
  SDL_SaveBMP(image_surface, "binarize.bmp");
  
  free(histo);
  SDL_FreeSurface(image_surface);
}

//Other version that returns a SDL_Surface

SDL_Surface* preprocessing_SDL(char *path)
{
  SDL_Surface *image_surface = IMG_Load(path);
  int height = image_surface->h;
  int width = image_surface->w;
  
  long *histo = calloc(256, sizeof(long));
  
  contrast(image_surface, 50, width, height);
  SDL_SaveBMP(image_surface, "contrast.bmp");

  toGrayscale(image_surface, width, height);
  SDL_SaveBMP(image_surface, "grayscale.bmp");

  reduce_noise(image_surface, width, height);
  SDL_SaveBMP(image_surface,"noise.bmp");

  binarize(image_surface, width, height, histo);
  SDL_SaveBMP(image_surface, "binarize.bmp");
  
  free(histo);
  return image_surface;
}
