//resizing.c file

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

//Functions that resizes the letter matrix so that they can be processed
//by the neural network.

/*
  Function that defines a new square matrix, of size = size.
*/
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
  free(image);
  matrix m = {size, size, new_image};
  return m;
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
	  printf("%c", (m.mat[i*m.width+j]==1) ? ' ' : '#');
	  if((j+1) % m.width == 0)
	    printf("\n");
	}
    }
}


/*
  Filling the rows and columns of a square matrix to obtain a 
  larger new_size square matrix
*/
void m_fill(matrix* m, int new_size)
{
  int m_size = m->height;  //square matrix so height = width
  if(m_size >= new_size)
  	return;

  int diff = (new_size - m_size)/2;
  double* image = m->mat;
  double* filled = malloc(new_size * new_size * sizeof(double));
  size_t c = 0; 
  for(int i = 0; i < new_size; ++i)
  {
    for(int j = 0; j < new_size; ++j)
	  {
	     if(i>=diff && i<new_size-diff && j>= diff && j<new_size-diff)
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
