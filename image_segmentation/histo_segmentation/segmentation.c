//segmentation.c file

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "segmentation.h"

/*
Regroups the functions that do the segmentation.
This segmentation uses histograms
*/

//------------------------------------------------------------------------------

//Functions that creates the histograms

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

//Functions that draws line on the image to do the segmentation

/*
Function that draws vertical lines in a given rectangle, 
in columns with no foreground pixels.
*/
void vert_lines(SDL_Surface *image_surface, int *hori_histo, coord *rect)
{
  int len = rect->botR.w - rect->topL.w;
  for(int i = 0 ; i < len ; i++)
    {
      if (*(hori_histo + i) == 0)
	trace_vert_red_line(image_surface, rect->topL.h,
			    rect->topL.w + i, rect->botR.h, rect->topL.w + i);
    }
}

/*
Function that draws horizontal lines in a given rectangle, 
in rows with no foreground pixels.
 */
void hori_lines(SDL_Surface *image_surface, int *vertHisto, coord *rect)
{
  int len = rect->botR.h - rect->topLh;
  for(int i = 0 ; i < len ; i++)
    {
      if (*(vertHisto + i) == 0)
	  trace_hori_red_line(image_surface, rect->topL.h + i,
			      rect->topL.w, rect->topL.h + i, rect->botR.w);
    }
}

//------------------------------------------------------------------------------

//Functions that gets the coordinates of the lines and the letters in the image
//after the red lines drawn

/*
Function that goes to the summit of a rectangle,
at the right of a given point.
Returns the width of this given point
*/
int r_sum(SDL_Surface *image_surface, int wMin, int wMax, int h)
{
  int red = 1;
  int res = wMin;
  while (red == 1 && res < wMax)
    {
      red = is_red(image_surface, res, h);
      res++;
    }
  return res - 1;
}

/*
Function that goes to the summit of a rectangle,
at the bottom of a given point.
Returns the height of this given point
*/
int bot_sum(SDL_Surface *image_surface, int hMin, int hMax, int w)
{
  int red = 1;
  int res = hMin;
  while (red == 1 && res < hMax)
    {
      red = is_red(image_surface, w, res);
      res++;
    }
  return res - 1;
}

//------------------------------------------------------------------------------

//Functions that gets the coordinates of a line

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
	  if(red == 1) //first encounter with a pixel not red
	    {
	      int tmp =  bot_sum(image_surface, i, rect.botR.h, j);
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
	  if(red == 1) //first encounter with a pixel not red
	    {
	      all.zones[ind].topL.w = j;
	      all.zones[ind].topL.h = i;
	      all.zones[ind].botR.w = r_sum(image_surface, j, rect.botR.w, i);
	      int tmp =  bot_sum(image_surface, i, rect.botR.h, j);
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

//Functions that gets the coordinates of a letter

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
	  if (red == 1)//first encounter of a pixel not red
	    {
	      int tmp = r_sum(image_surface, i, rect.botR.w, j);
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
	  if (red == 1)//first encounter of a pixel not red
	    {
	      l.letters[ind].folBySpace = 0;
	      l.letters[ind].topL.w = i;
	      l.letters[ind].topL.h = j;
	      l.letters[ind].botR.h = bot_sum(image_surface, j, rect.botR.h, i);
	      int tmp = r_sum(image_surface, i, rect.botR.w, j);
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

/*
Function that counts the number of peaks in a given 
histogram.
*/
int count_peaks(int average, int *histo, int lenH)
{
  int isGoingDown = 1; //false, 0 if true
  int before = *histo;
  int res = 0;
  for(int i = 0 ; i < lenH ; i++)
    {
      if(before > *(histo + i) && isGoingDown == 1)
	{
	  isGoingDown = 0;
	  if (*(histo + i) > average)
	    res++;
	}
      else if(before <= *(histo + i) && isGoingDown == 0)
	isGoingDown = 1;
      before = *(histo+i);
    }
  return res;
}

//------------------------------------------------------------------------------

//Functions to separate two letters that are too close from each other

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
Function that separates letters of a same line if they are still in the
same rectangle of letter, even after segmentation
*/
void sepLetters(SDL_Surface *image, line l)
{
  int av = avLenLetter(l);
  int len;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      len = l.letters[i].botR.w - l.letters[i].topL.w;
      if (len > 1.75*av)
	{
	  int w = l.letters[i].topL.w + (len/2.4);
	  trace_vert_red_line(image, l.letters[i].topL.h, w, l.letters[i].botR.h, w);
	}
    }
}

//------------------------------------------------------------------------------

//Functions to detect the spaces between words

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
      if (len > av)
	{
	  l->nbSpaces +=1;
	  l->letters[i].folBySpace = 1;
	}
    }
}

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

//------------------------------------------------------------------------------

//Functions that initialize the structures

/*
Function that initialises a lineZone variable
*/
lineZones init_lineZones(int nbLines)
{
  lineZones res;
  res.nbZones = nbLines;
  res.zones = malloc(nbLines*sizeof(coord));
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
  //if (nbLetters != 0)
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
  res.allLines = malloc(nbLines*sizeof(line));
  return res;
}

//------------------------------------------------------------------------------

//Functions that free the structures

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

//------------------------------------------------------------------------------

//Functions that treats the image for the segmentation, uses the previous functions

/*
Function that does the vertical histogram and
draws the lines to show the lines of the text.
*/
void marking_lines(SDL_Surface *image, int h, int w)
{
  int *histo = calloc(h, sizeof(int));
  coord rect;
  rect.topL.h = 0;
  rect.topL.w = 0;
  rect.botR.w = width-1;
  rect.botR.h = height-1;
  verti_histo(image_surface, histo, rect);
  hori_lines(image_surface, histo, rect);
  free(histo);
}

/*
Function that gets the nb of lines and for each line, 
makes the horizontal histogram to separate the
letters, and draws the lines.
*/
lineZones marking_letters(SDL_Surface *image_surface, int w, int h)
{
  coord rect;
  rect.topL.h = 0;
  rect.topL.w = 0;
  rect.botR.w = w;
  rect.botR.h = h;
  int nbLines = count_get_lines(image_surface, rect);
  lineZones all = init_lineZones(nbLines);
  get_lines(image_surface, rect, all);
  for(int i = 0 ; i < nbLines ; i++)
    {
      int topLw = all.zones[i].topL.w;
      int botRw = all.zones[i].botR.w;
      int *histo2 = calloc(botRw - topLw, sizeof(int));
      hori_histo(image_surface, histo2, all.zones[i]);
      vert_lines(image_surface, histo2, all.zones[i]);
      free(histo2);
    }
  return all;
}


/*
Function that takes all the coordinates of the rectangles
that contains each letter.
*/
doc keep_letters(SDL_Surface *image_surface, lineZones all)
{
  doc image = init_doc(all.nbZones);
  for(int i = 0 ; i < all.nbZones ; i++)
    {
      int nbLetters = count_get_letters(image_surface, all.zones[i]);
      image.allLines[i] = init_line(nbLetters);
      get_letters(image_surface, all.zones[i], image.allLines[i]);
    }
  return image;
}

//-------------------------
//to keep point on the i and take off accents

/*
Calculates the ratio w/h of a rectangle
*/
int rect_ratio(coord r)
{
  int w = r.botR.w - r.topL.w;
  int h = r.botR.h - r.topL.h;
  if (h == 0)
    return 0;
  return w/h;
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



/*
Function that resizes the rectangles arround the letters.
*/
void resize_letter(SDL_Surface *image_surface, doc image)
{
  for(int i = 0 ; i < image.nbLines ; i++)
    {
      sepLetters(image_surface, image.allLines[i]);
      
      for(int j = 0 ; j < image.allLines[i].nbLetters ; j++)
	{
	  int botRh = image.allLines[i].letters[j].botR.h;
	  int topLh = image.allLines[i].letters[j].topL.h;
	  int *histo3 = calloc(botRh - topLh, sizeof(int));
	  verti_histo(image_surface, histo3, image.allLines[i].letters[j]);
	  hori_lines(image_surface, histo3, image.allLines[i].letters[j]);
	  free(histo3);
	}
    }
  detectSpaceDoc(&image);
}
