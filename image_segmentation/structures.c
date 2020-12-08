//structures.c file

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include <string.h>
#include "resizing.h"
#include "matrix_letters.h"

#include "string.h"

#include "../neunet_HD/neural_main.h"

/*
Regroups functions to initialize or free the structures.
Regroups some test functions too.
*/

//------------------------------------------------------------------------------

//FUNCTIONS TO INITIALIZE

/*
Function that initialises a lineZone variable
*/
lineZones init_lineZones(int nbLines)
{
  lineZones res;
  res.nbZones = nbLines;
  res.zones = calloc(nbLines, sizeof(coord));
  for(int i = 0 ; i < nbLines ; i++)
    {
      res.zones[i].topL.w = 0;
      res.zones[i].topL.h = 0;
      res.zones[i].botR.w = 0;
      res.zones[i].botR.h = 0;
    }
  return res;
}


/*
Function that initialises a structure line
*/
line init_line(int nbLetters)
{
  line res;
  res.nbLetters = nbLetters;
  res.nbSpaces = 0;
  res.letters = calloc(nbLetters, sizeof(coord));
  return res;
}


/*
Function that initialises a structure doc
*/
doc init_doc(int nbLines)
{
  doc res;
  res.nbLines = nbLines;
  res.allLines = calloc(nbLines, sizeof(line));
  return res;
}


//------------------------------------------------------------------------------

//FUNCTIONS TO FREE

/*
Function that frees a doc
*/
void free_doc(doc image)
{
  for(int i = 0 ; i < image.nbLines ; i++)
    {
      free(image.allLines[i].letters);
    }
  free(image.allLines);
}

/*
Function that frees docMat
*/
void free_docMat(docMat m)
{
  for(int i = 0 ; i < m.nbLines ; i++)
    {
      for(int j = 0 ; j < m.lines[i].nbLetters ; j++)
	{
	  free(m.lines[i].letterMat[j].mat);
	}
      free(m.lines[i].letterMat);
    }
  free(m.lines);
}

//------------------------------------------------------------------------------

//FUNCTIONS TO TEST

/*
Function to test if the detection of spaces works.
Prints random text with the spaces of the text from the image.
*/
void print_line(line *l)
{
  int sum = 0;
  for(int i = 0 ; i < l->nbLetters ; i++)
    {
      printf("A");
      sum += 1;
      if (l->letters[i].folBySpace == 1)
	printf(" ");
    }
  printf("\n %d \n", sum);
}

/*
  Function that gives back a string for UI.
  Transforms the letter in a matrix, resizes it.
  Should be giving it to the Neural Network, and will give back a letter.
*/
char* line_string(SDL_Surface *surface, line *l, neunet_t *nn)
{
  char *res = calloc((l->nbLetters + l->nbSpaces + 2), sizeof(char)); //+2 for \n
  strcpy(res, "");
  int w;
  int h;
  char letter;
  matrix m;
  int space = 0;
  for(int i = 0 ; i < l->nbLetters ; ++i)
    {
      m = buildMatrix(surface, l->letters[i]);
      w = l->letters[i].botR.w - l->letters[i].topL.w;
      h = l->letters[i].botR.h - l->letters[i].topL.h;
      m = interpolation(m.mat, w, h, 20);
      m_fill(&m);
      
      letter = neural_net_ask(nn, m.mat);
      
      res[i+space] = letter;
      if (l->letters[i].folBySpace == 1)
	{
	  space++;
	  res[i+space] = ' ';
	}
    }
  //strcat(res, "\n");
  free(m.mat);
  sprintf((res+l->nbLetters+l->nbSpaces), "\n");
  return res;
}

/*
  Function that calculates the number of char in an image
*/
int nb_char(doc *image)
{
  int res = 0;
  for(int i = 0 ; i < image->nbLines ; i++)
    {
      res += image->allLines[i].nbLetters + image->allLines[i].nbSpaces + 2; //+2 for \n
    }
  return res;
}

/*
Function that gives back a string with all the letters of the doc for the UI
*/
char* doc_string(SDL_Surface *surface, doc *image, neunet_t *nn)
{
  char *res = calloc(nb_char(image),sizeof(char));
  strcpy(res, "");
  char *tmp;
  for(int i = 0 ; i < image->nbLines ; i++)
    {
      tmp = line_string(surface, &image->allLines[i], nn);
      strcat(res, tmp);
      free(tmp);
    }
  return res;
}

/*
Function to test if the detection of spaces works.
Prints the text of the whole doc
*/
void print_doc(doc *image)
{
  for(int i = 0 ; i < image->nbLines ; i++)
    {
      print_line(&image->allLines[i]);
    }
}

/*
Function that prints the content of a matrix
*/
void print_matrix(matrix m)
{
  for(int i = 0 ; i < m.height ; i++)
    {
      for(int j = 0 ; j < m.width ; j++)
	{
	  printf("%.0lf", m.mat[i*m.width+j]);
	  if((j+1) % m.width == 0)
	    printf("\n");
	}
    }
}
