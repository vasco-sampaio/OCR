//utility.c file

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
/*#include "pixel_functions.h"
  #include "structures.h"*/
#include "../types.h"

/*
Regroups all the functions that contribute to detect the letters
 */

//------------------------------------------------------------------------------

//Functions that builds the histograms for given rectangles on the image

/*
Function that creates a vertical histogram that counts the foreground 
pixels (black pixels) of each row, of a given rectangle.
*/
void verti_histo(SDL_Surface *image_surface, int *histo, coord rect)
{
  int ind = 0;
  for(int i = rect.topL.h ; i < rect.botR.h ; i++)
    {
      for(int j = rect.topL.w ; j < rect.botR.w ; j++)
	{
	  if (is_black(image_surface, j, i) == 1)
	      *(histo + ind) += 1;
	}
      ind++;
    }
}

/*
Function that creates a horizontal histogram that counts the foreground 
pixels of each column, of a given rectangle.
 */
void hori_histo(SDL_Surface *image_surface, int *histo, coord rect)
{
  int ind = 0;
  for(int j = rect.topL.w ; j < rect.botR.w ; j++)
    {
      for(int i = rect.topL.h ; i < rect.botR.h ; i++)
	{
	  if(is_black(image_surface, j, i) == 1)
	    *(histo + ind) += 1;
	}
      ind++;
    }
}



//------------------------------------------------------------------------------

//Functions that separate letters that the histogram method doesn't separate

/*
Function that calculates the average width of a letter on a line
*/
int avLenLetter(line l)
{
  int sum = 0;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      sum += (l.letters[i].botR.w - l.letters[i].topL.w);
    }
  if (l.nbLetters != 0)
    return sum / l.nbLetters;
  return sum;
}

/*
Function that calculates the average height of a letter on a line
*/
double avHeiLetter(line l)
{
  double sum = 0;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      sum += (l.letters[i].botR.h - l.letters[i].topL.h);
    }
  if (l.nbLetters != 0)
    return sum / l.nbLetters;
  return sum;
}

/*
Function that separates letters of a same line if they are still in the
same rectangle of letter, even after segmentation
*/
void sepLetters(SDL_Surface *image, line l)
{
  int av = avLenLetter(l);
  double av2 = avHeiLetter(l);
  int len;
  double hei;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      len = l.letters[i].botR.w - l.letters[i].topL.w;
      hei = l.letters[i].botR.h - l.letters[i].topL.h;
      if (len > 2*av && hei < 1.2*av2)
	{
	  int w = l.letters[i].topL.w + (len/2.4);
	  trace_vert_red_line(image, l.letters[i].topL.h, w, l.letters[i].botR.h, w);
	}
    }
}

//------------------------------------------------------------------------------

//Functions that detect the spaces between words

/*
Function that calculates the average space between two letters
*/
int avSpaceLetter(line *l)
{
  int sum = 0;
  for(int i = 0 ; i < l->nbLetters - 1 ; i++)
    {
      sum += (l->letters[i+1].topL.w - l->letters[i].botR.w);
    }
  if (l->nbLetters != 0)
    return sum / l->nbLetters;
  return sum;
}

/*
Function that detects the spaces between two letters on a line
*/
void detectSpace(line *l)
{
  int av = avSpaceLetter(l);
  int len;
  for(int i = 0 ; i < l->nbLetters - 1 ; i++)
    {
      len = l->letters[i+1].topL.w - l->letters[i].botR.w;
      if (len > av*1.3)
	{
	  l->nbSpaces +=1;
	  l->letters[i].folBySpace = 1; //letter is followed by a space
	}
    }
}

/*
Function that detects all the spaces in a doc
*/
void detectSpaceDoc(doc *image)
{
  for(int i = 0 ; i < image->nbLines ; i++)
    {
      detectSpace(&image->allLines[i]);
    }
}


//------------------------------------------------------------------------------

//Functions to detect the lines after a line separation of the doc 

/*
Function that goes to the summit of a rectangle,
at the right of a given point.
Returns the width of this given point
*/
int r_sum_line(SDL_Surface *image_surface, int wMin, int wMax, int h)
{
  int red = 0;
  int res = wMin;
  while (red == 0 && res < wMax)
    {
      red = is_red(image_surface, res, h);
      res++;
    }
  return res-1;
}


/*
Function that goes to the summit of a rectangle,
at the bottom of a given point.
Returns the height of this given point
*/
int bot_sum_line(SDL_Surface *image_surface, int hMin, int hMax, int w)
{
  int red = 0;
  int res = hMin;
  while (red == 0 && res < hMax)
    {
      red = is_red(image_surface, w, res);
      res++;
    }
  return res-1;
}

/*
Function that goes through the image to count the zones 
not touched by the lines drawn
*/
int count_get_lines(SDL_Surface *image_surface, coord rect)
{
  int res = 0;
  int i = rect.topL.h;
  while (i < rect.botR.h)
    {
      int j = rect.topL.w;
      while(j < rect.botR.w)
	{
	  int red = is_red(image_surface, j, i);
	  if(red == 0) //first encounter with a pixel not red
	    {
	      int tmp =  bot_sum_line(image_surface, i, rect.botR.h, j);
	      res++;
	      j = rect.botR.w;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
  return res;
}


/*
Function that goes through the image to get the zones 
not touched by the lines drawn.
Gets 2 coordinates to define a rectangle
*/
void get_lines(SDL_Surface *image_surface, coord rect, lineZones all)
{
  int ind = 0;
  int i = rect.topL.h;
  while (i < rect.botR.h)
    {
      int j = rect.topL.w;
      while(j < rect.botR.w)
	{
	  int red = is_red(image_surface, j, i);
	  if(red == 0) //first encounter with a pixel not red
	    {
	      all.zones[ind].topL.w = j;
	      all.zones[ind].topL.h = i;
	      all.zones[ind].botR.w = r_sum_line(image_surface, j, rect.botR.w, i);
	      int tmp =  bot_sum_line(image_surface, i, rect.botR.h, j);
	      all.zones[ind].botR.h = tmp;
	      ind++;
	      j = rect.botR.w;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
}


//------------------------------------------------------------------------------

//Functions to detect the letters after the letter separation

/*
Function that goes to the summit of a rectangle,
at the right of a given point.
Returns the width of this given point
*/
int r_sum_letter(SDL_Surface *image_surface, int wMin, int wMax, int h)
{
  int red = 0;
  int res = wMin;
  while (red == 0 && res < wMax)
    {
      red = is_red(image_surface, res, h);
      res++;
    }
  return res;
}


/*
Function that goes to the summit of a rectangle,
at the bottom of a given point.
Returns the height of this given point
*/
int bot_sum_letter(SDL_Surface *image_surface, int hMin, int hMax, int w)
{
  int red = 0;
  int res = hMin;
  while (red == 0 && res < hMax)
    {
      red = is_red(image_surface, w, res);
      res++;
    }
  return res;
}

/*
Function that goes through a given rectangle to count the zones 
were there are letters
*/
int count_get_letters(SDL_Surface *image_surface, coord rect)
{
  int res = 0;
  int i = rect.topL.w;
  while(i < rect.botR.w)
    {
      int j = rect.topL.h;
      while(j < rect.botR.h)
	{
	  int red = is_red(image_surface, i, j);
	  if (red == 0)//first encounter of a pixel not red
	    {
	      int tmp = r_sum_letter(image_surface, i, rect.botR.w, j);
	      res++;
	      j = rect.botR.h;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
  return res;
}


/*
Function that goes through a given rectangle to get the zones not touched by the red lines and where there are the letters.
*/
void get_letters(SDL_Surface *image_surface, coord rect, line l)
{
  int ind = 0;
  int i = rect.topL.w;
  while(i < rect.botR.w)
    {
      int j = rect.topL.h;
      while(j < rect.botR.h)
	{
	  int red = is_red(image_surface, i, j);
	  if (red == 0)//first encounter of a pixel not red
	    {
	      l.letters[ind].folBySpace = 0;
	      l.letters[ind].topL.w = i;
	      l.letters[ind].topL.h = j;
	      l.letters[ind].botR.h = bot_sum_letter(image_surface, j, rect.botR.h, i);
	      int tmp = r_sum_letter(image_surface, i, rect.botR.w, j);
	      l.letters[ind].botR.w = tmp;
	      ind++;
	      j = rect.botR.h;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
}

//------------------------------------------------------------------------------

//Functions to take into account the i and accents on letters

/*
Calculates the ratio w/h of a rectangle
*/
double rect_ratio(coord r)
{
  double w = r.botR.w - r.topL.w;
  double h = r.botR.h - r.topL.h;
  if (w == 0)
    return 0;
  return h/w;
}

/*
Function that counts the number of groups of black pixels in a given 
histogram.
*/
int count_groups(int *histo, int lenH)
{
  int inZone = 0;
  int res = 0;
  for(int i = 0 ; i < lenH ; i++)
    {
      if (inZone == 0 && *(histo + i) != 0)
	{
	  inZone = 1;
	  ++res;
	}
      if(inZone == 1 && *(histo + i) == 0)
	{
	  inZone = 0;
	}
    }
  return res;
}

/*
Function that returns the rank of the black pixels groups
*/
tuples generate_groups(int *histo, int lenH)
{
  tuples t;
  int nbgp = count_groups(histo, lenH);
  t.nb = nbgp;
  t.list = calloc(nbgp, sizeof(couple));
  int inZone = 0;
  int j = 0;
  for(int i = 0 ; i < lenH ; i++)
    {
      if (inZone == 0 && *(histo + i) != 0)
	{
	  inZone = 1;
	  t.list[j].h = i; 
	}
      if(inZone == 1 && *(histo + i) == 0)
	{
	  inZone = 0;
	  t.list[j].w = i;
	  ++j;
	}
      if(inZone == 1 && i+1 == lenH)
	t.list[j].w = i;
    }
  return t;
}

/*
Function that returns the index of the largest group of black pixels
*/
int max_tuples(tuples t)
{
  if (t.nb > 0)
    {
      int index = 0;
      int max = t.list[0].h - t.list[0].w;
      int tmp;
      for(int i = 0 ; i < t.nb ; i++)
	{
	  tmp = t.list[i].h - t.list[i].w;
	  if(max < tmp)
	    {
	      index = i;
	      max = tmp;
	    }
	}
      return index;
    }
  else
    return -1;
}

/*
Function that draws vertical lines in a given rectangle, 
in rows with no foreground pixels and in attributed columns.
*/
void hori_lines_define(SDL_Surface *image, int *vertHisto, coord rect, tuples t)
{
  double r = rect_ratio(rect);
  int gps = t.nb;
  printf("gps = %d\n", gps);
  printf("r = %f\n", r);
  if (gps == 1)
    {
      hori_lines(image, vertHisto, rect);
    }
  else
    {
      //if the ratio is like a i
      if (/*r > 5 &&*/ gps == 2)
	{
	  int highr = t.list[0].h;
	  int lowr = t.list[1].w;
	  for(int i = rect.topL.h ; i < rect.topL.h + highr ; i++)
	    {
	      trace_hori_red_line(image, i,
		rect.topL.w, i, rect.botR.w);
	    }
	  for(int i = rect.topL.h + lowr ; i < rect.botR.h ; i++)
	    {
	      trace_hori_red_line(image, i,
			      rect.topL.w, i, rect.botR.w);
	    }
	}
      else
	{
	  int indMax = max_tuples(t);
	  int highr = t.list[indMax+1].h;
	  int lowr = t.list[indMax+1].w;
	  for(int i = rect.topL.h ; i < rect.topL.h + highr ; i++)
	    {
	      trace_hori_red_line(image, i,
			      rect.topL.w, i, rect.botR.w);
	    }
	  for(int i = rect.topL.h + lowr ; i < rect.botR.h ; i++)
	    {
	      trace_hori_red_line(image, i,
			      rect.topL.w, i, rect.botR.w);
	    }
	}
    }
}
