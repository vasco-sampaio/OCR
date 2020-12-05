#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

matrix interpolation(double* image, int width, int height, int size)
{
  double* new_image = calloc(size*size, sizeof(double));
  float scalex = width /(float) size;
  float scaley = height /(float) size;
  int px, py;
  for(int i = 0; i < size; ++i)
    {
      for(int j = 0; j < size; ++j)
	{
	  px = (int)(j * scalex);
	  py = (int)(i * scaley);
	  new_image[i*size+j] = image[py * width + px];
	}
    }
  matrix m = {size, size, new_image};
  
  return m;
}

void print_m(matrix m)
{
  int height = m.height;
  int width = m.width;
  printf("Matrix of %d x %d :\n", height, width);
  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  printf("%c", (m.mat[i*m.width+j]==1) ? ' ' : '#');
	  if((j+1) % m.width == 0)
	    printf("\n");
	}
    }
}

void m_fill(matrix* m)
{
  double* image = m->mat;
  double* filled = malloc(900 * sizeof(double));
  size_t c = 0;
  for(int i = 0; i < 30; ++i)
    {
      for(int j = 0; j < 30; ++j)
	{
	  if(i>4 && i<25 && j>4 && j<25)
	    {
	      filled[i * 30 + j] = image[c++];
	      continue;
	    }
	  filled[i * 30 + j] = 1;
	}
    }

  free(image);
  m->height = 30;
  m->width = 30;
  m->mat = filled;
}
