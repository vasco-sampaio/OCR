//rotation.c file

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../types.h"
#include <math.h>

//------------------------------------------------------------------------------
//Brute force method to try to get the angle of rotation


/*
  Function that converts a degree to radian
*/
double degree_to_rad(double deg)
{
	double res = (M_PI*deg)/180;
	//printf("rad = %f\n", res);
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


/*
  Function that builds the vertical histogram of the image
*/
void build_histo(SDL_Surface *image_surface, int *histo, int w, int h)
{
  for(int i = 0 ; i < h ; i++)
    {
      for(int j = 0 ; j < w ; j++)
	{
	  if (is_black(image_surface, j, i) == 1)
	      histo[i] += 1;
	}
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
	for(size_t i = -90 ; i < 90 ; i++)
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
	free(averages);
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

//------------------------------------------------------------------------------
//Hough transform to try to get the angle of rotation

/*
  Function that returns a value between 0 and 255
*/
int clamp(int nb)
{
	if (nb > 255)
		return 255;
	if (nb < 0)
		return 0;
	return nb;
}


/*
Function that verifies if the coordinates are valid in the image
*/
int is_valid(int w, int h, SDL_Surface *image)
{
	if (w < image->w && w >= 0)
		{
			if(h < image->h && h >=0)
				return 1; //the coordinates are valid
		}
	return 0;
}

/*
  Function to convolute a given matrix.
  Useful for edge / line detection
*/
SDL_Surface* convolution(SDL_Surface *image, int *conv, size_t dim)
{
 	SDL_Surface *is2 = SDL_CreateRGBSurface(0, image->w, image->h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
	SDL_BlitSurface(image, NULL, is2, NULL);

	int offset = dim/2;
	Uint8 r, g, b;
	int red, green, blue;
	Uint32 pixel;
	double coef;
	for(int i = 0 ; i < image->w ; i++)
		{
			for(int j = 0 ; j < image->h ; j++)
				{
					red = 0;
					green = 0;
					blue = 0;
					for(int di = -offset ; di <= offset ; di++)
						{
							for(int dj = -offset ; dj <= offset ; dj++)
								{
									if(is_valid(i+di, j+dj,image)==1)
										{
											pixel = get_pixel(image, i+di, j+dj);
											SDL_GetRGB(pixel, image->format, &r, &g, &b);
											coef = conv[(dj+offset)*dim + (di+offset)];
											red += r*coef;
											green += g*coef;
											blue += b*coef;
										}
								}
						}
					pixel = SDL_MapRGB(is2->format, clamp(red), clamp(green), clamp(blue));
					put_pixel(is2, i, j, pixel);
				}
		}
	SDL_SaveBMP(is2, "conv.bmp");
	return is2;
}

/*
Function that is calculating the diagonal of an image
*/
int diagonal(SDL_Surface *image)
{
	int w = image->w;
	int h = image->h;
	return sqrt(w*w+h*h);
}


/*
Function that calculates the angle after a Hough Transform.
Looks for the angles where there is the highest count
in the Hough matrix.
Makes an average of these angles. Returns in degrees.
*/
int angle(int *H, int diag)
{
	int sum = 0;
	int nb = 0;
	int nb_sum = 0;
	for(int i = 0 ; i < diag ; i++)
		{
			for(int j = 0 ; j < 90 ; j++)
				{
					if (H[j*90+i] == nb)
						{
							sum += j;
							nb_sum += 1;
						}
					if(H[j*90+i] > nb)
						{
							nb = H[j*90+i];
							sum = j;
							nb_sum = 0;
						}
				}
		}
	if (nb_sum == 0)
		nb_sum = 1;
	return sum/nb_sum;
}

/*
Function that is looking for the angle with the Hough transformation
*/
double hough(SDL_Surface *image)
{
	int line_detec[] = {-1,-1,-1,2,2,2,-1,-1,-1};
	int edges[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
	int sobel[] = {-1,-2,-1,0,0,0,1,2,1};
	SDL_Surface *conv = convolution(image, line_detec, 3);
	int diag = diagonal(image);
	printf("diag = %d\n", diag);
	int *H = calloc(diag*90, sizeof(int));
	printf("diag*90 = %d\n", diag*90);
	int d;
	double theta;
	for(int i = 0 ; i < image->w -1 ; i++)
		{
			for(int j = 1 ; j < image->h -1 ; j++)
				{
					if(is_black(conv, i, j) == 0)
						{
							for(int t = 0 ; t < 90 ; t++)
								{
									theta = degree_to_rad(t);
									d = j*cos(theta)-i*sin(theta);
									if(d >= diag)
										d = diag-1;
									if(d < 0)
										d = 0;
									//printf("d*diag+t = %d and d = %d and theta = %f and t = %d\n", d*diag+t, d, theta, t);
									//printf("H = %d\n", H[t*90+d]);
									H[t*90+d] +=1;
								}
						}
				}
		}
	SDL_FreeSurface(conv);
	double res = degree_to_rad(angle(H, diag));
	free(H);
	return res;		
}


//------------------------------------------------------------------------------
//Rotation algorithms


/*
   Function that rotates an image according to an angle.
   Pi = 180°
   2Pi = 360°
   */
SDL_Surface* rotate2(SDL_Surface *image, int w, int h, double angle)
{
	int cw = w / 2;
	int ch = h / 2;
	printf("angle = %f\n", angle);
	double a_cos = cos(angle);
	double a_sin = sin(angle);

	int distw;
	int disth;

	int resw;
	int resh;

	Uint32 pixel;

	SDL_Surface *is2 = SDL_CreateRGBSurface(0, w, h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
	SDL_BlitSurface(image, NULL, is2, NULL);
	make_img_white(is2, w, h); 
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
