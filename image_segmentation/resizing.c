//resizing.c file

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ASPECT_RATIO 0.9       //aspect ratio applied to every matrix
#define MATRIX_SIZE 30

//Functions that resizes the letter matrix so that they can be processed
//by the neural network.

/*
	Calculates the dimension of the normalized matrix 
	in function on the aspect ratio wanted and the dimensions
	of the original matrix
*/

struct ratio* normalized_ratio(int width, int height, float n_ratio)
{
	struct ratio* r2 = malloc(sizeof(struct ratio));
	//float n_ratio = 0;
	//float r1 = 0;  										//we are not determining r2 in function of r1
	if(width > height)
	{
		r2->h = height; 
		//r1 = height/(float)width;
		//n_ratio = sqrt(r1); 
		r2->w = (int)height/n_ratio;
	}
	else
	{
		r2->w = width;
		//r1 = width/(float)height;
		//n_ratio = sqrt(r1); 
		r2->h = (int)width/n_ratio; 
	}
	r2->r = n_ratio;
	return r2;
}

/*
  Function that defines a new square matrix, of size = size.
*/
matrix interpolation(matrix* m, int n_width, int n_height)
{
  double* new_image = calloc(n_width * n_height, sizeof(double));
  double* image = m->mat;
  int width = m->width;
  int height = m->height;
  float scalex = width/(float)n_width;
  float scaley = height /(float)n_height;
  int px, py;
  for(int i = 0; i < n_height; ++i)
  {
   	for(int j = 0; j < n_width; ++j)
	{
	  px = (int)(j * scalex);
	  py = (int)(i * scaley);
	  new_image[i*n_width+j] = image[py * width + px];
	}
  }
  free(image);
  matrix new = {(int) n_width, (int)n_height, new_image};
  return new;
}


/*
  Filling the rows and columns of a matrix to obtain a 
  larger new_size square matrix
*/
void m_fill(matrix* m, int new_size)
{
  int height = m->height;
  int width = m->width;   

  if(height >= new_size && width >= new_size)
  	return;

  double* image = m->mat;
  double* filled = malloc(new_size * new_size * sizeof(double));
  size_t c = 0;
  
  int diff_w = (new_size-width)/2;
  int diff_h = (new_size-height)/2;

  for(int i = 0; i < new_size; ++i)
  {
    for(int j = 0; j < new_size; ++j)
	  {
	     if(i>=diff_h && i<new_size-diff_h && j>= diff_w && j<new_size-diff_w)
	     {
	       filled[i * new_size + j] = image[c++];
	       continue;
	     }
	     filled[i * new_size + j] = 0;
	  }
  }

  free(image);
  m->height = new_size;
  m->width = new_size;
  m->mat = filled;
}

matrix normalization(matrix *m)
{
	struct ratio* norm = normalized_ratio(m->width, m->height, ASPECT_RATIO);	 
	/*int ty, tx;
	for(int y = 0; y < height; ++y)
	{
		ty = norm->h * (y/height);
		for(int x = 0; x < width; ++x)
		{
			tx = norm->w * (x/width);
			new_image[ty * norm->w + tx] = image[y * width + x];
		}
	}*/
	matrix res = interpolation(m, norm->w, norm->h);
	matrix normalized = interpolation(&res, 22, 22);
	free(norm);
	m_fill(&normalized,MATRIX_SIZE);
	return normalized;

}

/*
  Function that prints a matrix.
*/
void print_m(matrix m)
{
  int height = m.height;
  int width = m.width;
  printf("Matrix of %d x %d :\n", height, width);
  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  printf("%c", (m.mat[i*m.width+j]!=1) ? '-' : '#');
	  if((j+1) % m.width == 0)
	    printf("\n");
	}
    }
}