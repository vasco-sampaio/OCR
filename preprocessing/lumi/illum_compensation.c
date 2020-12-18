#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "pixel_operations.h"
#include "illum_compensation.h"
//#include "sdl_functions.h"
//#include "preprocessing.h"
#include <err.h>

/*
  Function that puts the image in grayscale.
*/
void toGrayscale(SDL_Surface *image_surface, int* grayscale, int w, int h)
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
	  		grayscale[i * w + j] = av;
		}
    }
}

int SOB_kernel(int* mat)
{

	int G1[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	int	G2[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
	int G3[9] = {-2, -1, 0, -1, 0, 1, 0, 1, 2};
	int G4[9] = {0, 1, 2, -1, 0, 1, -2, -1, 0};
	int c = 0;
	int gray = 0;

	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			c = y * 3 + x;
			gray += mat[c] *(G1[c] + G2[c] + G3[c] + G4[c]);
			
		}
	}
	gray /= 4;

	return gray;
}

/*
	Edge detection on greyscale image
	Sources: en.wikipedia.org/wiki/sobel_operator
			 cs.auckland.ac.nz/edge_detection_Sobel
			 projectrhea.org/rhea/index.php/An_implementation_of_Soble_Edge_Detection
*/

extrm sobelEdge(SDL_Surface* image, int* grayscale)
{
	int height = image->h;
	int width = image->w;
	int* mat = malloc(9*sizeof(int)); 
	int gray = 0;
	extrm mima;
	mima.min = 255;
	mima.max = 0;
	mima.grid = malloc(height*width * sizeof(int));

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			mat[0] = (i == 0 || j == 0) ? 0 : grayscale[(i-1) * width + (j - 1)];
			mat[1] = (i == 0) ? 0 : grayscale[(i-1) * width + j];
			mat[2] = (i == 0 || j == width - 1) ? 0 : grayscale[(i-1) * width + (j + 1)];
			mat[3] = (j == 0) ? 0 : grayscale[i * width + (j-1)];
			mat[4] = grayscale[i * width + j];
			mat[5] = (j == width-1) ? 0 : grayscale[i * width + (j-1)];
			mat[6] = (i == height - 1 || j == 0) ? 0 : grayscale[(i+1) * width + (j-1)];
			mat[7] = (i == height - 1) ? 0 : grayscale[(i+1) * width + j];
			mat[8] = (i == height - 1 || j == width-1) ? 0 : grayscale[(i+1) * width + (j+1)];;

			gray = SOB_kernel(mat);
			mima.grid[i * width + j] = gray;

			if(gray < mima.min)
				mima.min = gray;
			if(gray > mima.max)
				mima.max = gray;
		}
	}
	free(mat);
	//free(grayscale);

	return mima;
}

void binarize(SDL_Surface* image, int* grid, int min, int max)
{
	int th = (min + max)/2;
	int w = image->w;
	int h = image->h;
	Uint8 gr;

	for(int i = 0; i < h; ++i)
	{
		for(int j = 0; j < w; ++j)
		{
			/*gr = grid[i * w + j];
			put_pixel(image, j, i, SDL_MapRGB(image->format, gr, gr, gr));
			SDL_SaveBMP(image, "Sobel.bmp");*/
			gr = (grid[i * w + j] < th) ? 255 : 0;
			put_pixel(image, j, i, SDL_MapRGB(image->format, gr, gr, gr));
		}
	}
	free(grid);
}

/*
   Function that modifies the contrast of GRAYSCALE image
   */
void contrastBin(SDL_Surface *image, int lvlc, int w, int h)
{
	double factor = (259.0 * (lvlc + 255.0))/(255.0 * (259.0 - lvlc));
	Uint8 grid[h*w];
	Uint8 gr;
	int f;
	Uint8 max = 0;
	Uint8 min = 255;
	for(int i = 0 ; i < h ; i++)
	{
		for(int j = 0 ; j < w ; j++)
		{
			SDL_GetRGB(get_pixel(image, j, i), image->format, &gr, &gr, &gr);
			f = factor * (gr - 128) + 128;
			if (f < 0)
				f = 0;
			else if (f > 255)
				f = 255;

			gr = (Uint8)f;

			put_pixel(image, j, i, SDL_MapRGB(image->format, gr, gr, gr));
			grid[i*w+j] = gr;

			if(gr > max)
				max = gr;
			if(gr < min)
				min = gr;
		}
	}

	int th = (min+max)/2;
	for(int i = 0; i < h; ++i)
	{
		for(int j = 0; j < w; ++j)
		{
			gr = (grid[i * w + j] < th) ? 255 : 0;
			put_pixel(image, j, i, SDL_MapRGB(image->format, gr, gr, gr));
		}
	}
}

/* 	Merges both surfaces, modifies in place ei(1st parameter)
	and frees cei(2nd parameter)*/

void mergeSurfaces(SDL_Surface* ei, SDL_Surface* cei)
{
	int h = ei->h; //same dimensions for both surfaces
	int w = ei->w;
	Uint8 pei, pcei;
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			SDL_GetRGB(get_pixel(ei, j, i), ei->format, &pei, &pei, &pei);
			SDL_GetRGB(get_pixel(cei, j, i), cei->format, &pcei, &pcei, &pcei);
			if(pei == 255 || pcei == 255)
				put_pixel(ei, j, i, SDL_MapRGB(ei->format, 0, 0, 0));
			else
				put_pixel(ei, j, i, SDL_MapRGB(ei->format, 255, 255, 255));

		}
	}
}

Uint8 EM_kernel(int* mat)
{
	//int G[9] = {1,1,1,1,1,1,1,1,1};
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if(mat[y * 3 + x] == 0)
				return 0;
		}
	}

	return 255;
}

/*
	Erosion of a binary image by morphology
*/

void erosionMask(SDL_Surface* image)
{
	int height = image->h;
	int width = image->w;
	Uint8* array = malloc(height*width*sizeof(Uint8));
	int* mat = malloc(9*sizeof(int)); 
	Uint8 tmp, px;

	for(int i = 0; i < height; ++i)
	{
		if(i == 0 || i == height - 1)
		{
			for(int j = 0; j < width; ++j)
				array[i * width + j] = 0;
			continue;
		}

		for(int j = 0; j < width; ++j)
		{
			if(j == 0 || j == width-1)
			{
				array[i * width + j] = 0;
				continue;
			}

			SDL_GetRGB(get_pixel(image, j-1, i-1), image->format, &tmp, &tmp, &tmp);
			mat[0] = tmp;

			SDL_GetRGB(get_pixel(image, j, i-1), image->format, &tmp, &tmp, &tmp);
			mat[1] = tmp;
			
			SDL_GetRGB(get_pixel(image, j+1, i-1), image->format, &tmp, &tmp, &tmp);
			mat[2] = tmp;
			
			SDL_GetRGB(get_pixel(image, j-1, i), image->format, &tmp, &tmp, &tmp);
			mat[3] = tmp;
			
			SDL_GetRGB(get_pixel(image, j, i), image->format, &tmp, &tmp, &tmp);
			mat[4] = tmp;
			
			SDL_GetRGB(get_pixel(image, j+1, i), image->format, &tmp, &tmp, &tmp);
			mat[5] = tmp;
			
			SDL_GetRGB(get_pixel(image, j-1, i+1), image->format, &tmp, &tmp, &tmp);
			mat[6] = tmp;
			
			SDL_GetRGB(get_pixel(image, j, i+1), image->format, &tmp, &tmp, &tmp);
			mat[7] = tmp;
			
			SDL_GetRGB(get_pixel(image, j+1, i+1), image->format, &tmp, &tmp, &tmp);
			mat[8] = tmp;

			px = EM_kernel(mat);	
			array[i * width + j] = px;
		}
	}


	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			px = array[i * width + j]; 
			put_pixel(image, j, i, SDL_MapRGB(image->format, px, px, px));
		}

	}

	free(mat);
	//return array;
}

/*void textInterpol(SDL_Surface* im)
{
	int h = im->h;
	int w = im->w;
	int sl = 0, sn;
	int pi, pc;
	Uint8 px;

	for(int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			SDL_GetRGB(get_pixel(im, j, i), im->format, &px, &px, &px);
			if(px == 0)
			{
				pi = i * w + j;
				while (px != 0)
				{
					SDL_GetRGB(get_pixel(im, j, i), im->format, &px, &px, &px);
					++j;
				}
				pc = i * w + j;

				for(int c = pi; c < pc; ++c)
				{
					SDL_GetRGB(get_pixel(im, j, i), im->format, &px, &px, &px);
					sl += px;
				}
				sl /= (pi - pc);
				sn = sl(pc - k)
			}
		}
	}

}*/
