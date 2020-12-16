#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
//#include "pixel_functions.h"
#include "../types.h"
#include <math.h>


/*
  Function that converts a degree to radian
*/
double degree_to_rad(double deg)
{
	double res = (M_PI*deg)/180;
	printf("rad = %f", res);
	return res;
}

/*
Function that calculates the average of a histogram
*/
double av(int *histo, size_t len)
{
	double sum = 0;
	for(size_t i = 0 ; i < len ; i++)
		{
			sum += histo[i];
		}
	return sum / len;
}

void build_histo(SDL_Surface *image_surface, int *histo, int w, int h)
{
  int ind = 0;
  for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  if (is_black(image_surface, j, i) == 1)
	      *(histo + ind) += 1;
	}
      ind++;
    }
}

/*
  Function that builds a vertical histogram and returns the average of black pixels
*/
double av_histo(SDL_Surface *image, int w, int h)
{
	int *histo = calloc(h, sizeof(int));
	build_histo(image, histo, w, h);
	for(int i = 0 ; i < h ; i++)
		{
			printf("%d ", histo[i]);
		}
	double a = av(histo, h);
	printf("av = %f\n", a);
	/*double a = 0;
	int max = histo[0];
	for(int i = 0 ; i < h ; i++)
		{
			if(max < histo[i])
				{
					a = i;
					max = histo[i];
				}
		}
		printf("a = %f\n", a);*/
	free(histo);
	return a;
}


/*
  Function that finds the angle of rotation
*/
double find_angle(SDL_Surface *image)
{
	double *averages = malloc(180*sizeof(double));
	SDL_Surface *tmp;
	for(size_t i = 0 ; i < 180 ; i++)
		{
			tmp = rotate2(image, image->w, image->h, degree_to_rad(i));
			averages[i] = av_histo(tmp, image->w, image->h);
			SDL_FreeSurface(tmp);
		}
	int deg = 0;
	double max = averages[0];
	for(int i = 1 ; i < 180 ; i++)
		{
			if (max < averages[i])
				{
					max = averages[i];
					deg = i;
				}
		}
	return degree_to_rad(deg);
}


/*
Function that makes an image totally white
*/
void make_img_white(SDL_Surface *image, int w, int h)
{
	Uint32 pixel;
	for(int i = 0 ; i < w ; i++ )
		{
			for(int j = 0 ; j < h ; j++)
				{
				        
					pixel = SDL_MapRGB(image->format, 255, 255, 255);
					put_pixel(image, i, j, pixel);
				}
		}
}


/*
   Function that rotates an image according to an angle.
   Pi = 180°
   2Pi = 360°
   */
SDL_Surface* rotate2(SDL_Surface *image, int w, int h, double angle)
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
	SDL_BlitSurface(image, NULL, is2, &is2->clip_rect);
	make_img_white(is2, w, h); 
	SDL_SaveBMP(is2, "rotation5.bmp");
	for(int i = 0 ; i < w ; i++)
	{
		distw = i - cw;
		for(int j = 0 ; j < h ; j++)
		{
		        disth = j - ch;
			resw = distw*a_cos - disth*a_sin + cw;
			resh = distw*a_sin + disth*a_cos + ch;

			if (resh > h)
				continue;
				//resh = h;
			if (resw > w)
				continue;
				//resw = w;
			if(resh < 0)
				continue;
				//resh = 0;
			if(resw < 0)
				continue;
				//resw = 0;

			pixel = get_pixel(image, resw, resh);
			put_pixel(is2, i, j, pixel); 
		}

	}
	return is2;
}

/*
   Function that rotates an image according to an angle.
   Pi = 180°
   2Pi = 360°
   */
void rotate(SDL_Surface *image, int w, int h, double angle)
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
	SDL_BlitSurface(image, NULL, is2, &is2->clip_rect);
	for(int i = 0 ; i < w ; i++)
	{
		distw = i - cw;
		for(int j = 0 ; j < h ; j++)
		{
		        disth = j - ch;
			resw = distw*a_cos - disth*a_sin + cw;
			resh = distw*a_sin + disth*a_cos + ch;

			if (resh > h)
				resh = h;
			if (resw > w)
				resw = w;
			if(resh < 0)
				resh = 0;
			if(resw < 0)
				resw = 0;

			pixel = get_pixel(is2, resw, resh);
			put_pixel(image, i, j, pixel); 
		}

	}
}
