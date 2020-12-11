//matrix_letters.c file

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
/*#include "pixel_functions.h"
#include "segmentation.h"
#include "matrix_letters.h"
#include "structures.h"*/
#include "../types.h"

/*
This file regroups the functions that uses matrices
*/

//------------------------------------------------------------------------------

/*
Function that builds a matrix for a letter
*/
matrix buildMatrix(SDL_Surface *image, coord let)
{
  matrix m;
  m.width = let.botR.w - let.topL.w;
  m.height = let.botR.h - let.topL.h;
  m.mat = calloc(m.width*m.height, sizeof(double));
  for(int i = 0 ; i < m.height; i++)
    {
      for(int j = 0 ; j < m.width  ; j++)
	{
	  if(is_black(image, j+let.topL.w, i+let.topL.h) == 1)
	    m.mat[i*m.width + j] = 1;
	  else
	    m.mat[i*m.width + j] = 0;
	}
    }
  //print_matrix(m);
  //printf("\n\n");
  return m;
}

/*
Function that builds a lineMat
*/
lineMat buildLineMat(SDL_Surface *image, line l)
{
  lineMat lm;
  lm.letterMat = malloc(l.nbLetters*sizeof(matrix));
  lm.nbLetters = l.nbLetters;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      lm.letterMat[i] = buildMatrix(image, l.letters[i]);
    }
  return lm;
}

/*
Function that build docMat
*/
docMat buildDocMat(SDL_Surface *image, doc i)
{
  docMat r;
  r.nbLines = i.nbLines;
  r.lines = malloc(i.nbLines*sizeof(lineMat));
  for(int j = 0 ; j < i.nbLines ; j++)
    {
      r.lines[j] = buildLineMat(image, i.allLines[j]);
    }
  return r;
}
