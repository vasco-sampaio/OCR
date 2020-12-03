//structures.c file

#include <err.h>
#include <stdlib.h>

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