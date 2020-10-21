#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "segmentation.h"

/*
This file is in charge of the segmentation
using histograms.
*/

/*
Function that checks if whether or not, a pixel
that has the coordinates h and w, is black.
Returns an int : 
-0 if black
-1 if white
*/
int is_black(SDL_Surface *image_surface, int w, int h)
{
  Uint32 pixel = get_pixel(image_surface, w, h);
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format,&r, &g, &b);

  //checks if the pixel is black
  printf("r = %d\n", r);
  if (r == 0)
    return 0;
  return 1;
}


/*
Function that check whether or not a pixel is a
red pixel.
Returns an int :
-0 if red
-1 if not
*/
int is_red(SDL_Surface *image_surface, int w, int h)
{
  Uint32 pixel = get_pixel(image_surface, w, h);
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format,&r, &g, &b);

  //checks if the pixel is red
  //if it is, returns 0
  if (r == 255 && g == 0 && b == 0)
    return 0;
  return 1;
}

/*
Function that creates a vertical histogram that counts the foreground 
pixels (black pixels) of each row, of a given rectangle.
*/
void verti_histo(SDL_Surface *image_surface, int *histo, coord rect)
{
   for(int i = 0 ; i < botRh - topLh ; i++)
     {
       printf("histo = %d\n", *(histo3+i));
     }
  for(int i = rect.topL.h ; i <= rect.botR.h ; i++)
    {
      for(int j = rect.topL.w ; j <= rect.botR.w ; j++)
	{
	  printf("is black = %d\n\n", is_black(image_surface, j, i));
	  if (is_black(image_surface, j, i) == 0)
	    {
	      printf("+1\n");
	      *(histo + i) += 1;
	      printf("histo + %d = %d\n", i, *(histo +i));
	    }
	}
    }
}


/*
Function that creates a horizontal histogram that counts the foreground 
pixels of each column, of a given rectangle.
 */
void hori_histo(SDL_Surface *image_surface, int *histo, coord rect)
{ 
  for(int j = rect.topL.w ; j <= rect.botR.w ; j++)
    {
      for(int i = rect.topL.h ; i <= rect.botR.h ; i++)
	{
	  if(is_black(image_surface, j, i) == 0)
	    *(histo + j) += 1;
	}
    }
}


/*
Function that draws horizontal lines in a given rectangle, 
in rows with no foreground pixels.
 */
void hori_lines(SDL_Surface *image_surface, int *vertHisto, coord rect)
{
  int topLh = rect.topL.h;
  int topLw = rect.topL.w;
  int botRw = rect.botR.w;
  int botRh = rect.botR.h;
  int len = botRh - topLh;
  for(int i = 0 ; i < len+1 ; i++)
    {
      if (*(vertHisto + i) == 0)
	  trace_hori_red_line(image_surface, topLh + i, topLw, topLh + i, botRw);
    }
}


/*
Function that draws vertical lines in a given rectangle, 
in columns with no foreground pixels.
*/
void vert_lines(SDL_Surface *image_surface, int *hori_histo, coord rect)
{
  int topLh = rect.topL.h;
  int topLw = rect.topL.w;
  int botRw = rect.botR.w;
  int botRh = rect.botR.h;
  int len = botRw - topLw;
  for(int i = 0 ; i < len+1 ; i++)
    {
      if (*(hori_histo + i) == 0)
	trace_vert_red_line(image_surface, topLh, topLw + i, botRh, topLw + i);
    }
}


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
Function that initialises a structure doc
*/
doc init_doc(int nbLines)
{
  doc res;
  res.nbLines = nbLines;
  res.allLines = calloc(nbLines, sizeof(line));
  return res;
}


/*
Function that initialises a structure line
*/
line init_line(int nbLetters)
{
  line res;
  res.nbLetters = nbLetters;
  if (nbLetters != 0)
    res.letters = calloc(nbLetters, sizeof(coord));
  return res;
}


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
  return res -1;
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
  return res -1;
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


/*
Function that does the vertical histogram and
draws the lines to show the lines of the text.
*/
void marking_lines(SDL_Surface *image_surface, int height, int width)
{
  int *histo;
  histo = calloc(height, sizeof(int));
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
  printf("nbLines = %d\n", nbLines);
  lineZones all = init_lineZones(nbLines);
  get_lines(image_surface, rect, all);
  for(int i = 0 ; i < nbLines ; i++)
    {
      printf("i = %d\n", i);
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
      printf("nbLetters = %d\n", nbLetters);
      image.allLines[i] = init_line(nbLetters);
      get_letters(image_surface, all.zones[i], image.allLines[i]);
    }
  return image;
}

/*
Function that resizes the rectangles arround the letters
*/
void resize_letter(SDL_Surface *image_surface, doc image)
{
  for(int i = 0 ; i < image.nbLines ; i++)
    {
      for(int j = 0 ; j < image.allLines[i].nbLetters ; j++)
	{
	  int botRh = image.allLines[i].letters[j].botR.h;
	  int botRw = image.allLines[i].letters[j].botR.w;
	  int topLh = image.allLines[i].letters[j].topL.h;
	  int topLw = image.allLines[i].letters[j].topL.w;
	  printf("botRh = %d\n", botRh);
	  printf("topLh = %d\n", topLh);
	  printf("botRw = %d\n", botRw);
	  printf("topLw = %d\n", topLw);
	  int *histo3 = calloc(botRh - topLh, sizeof(int));
	  verti_histo(image_surface, histo3, image.allLines[i].letters[j]);
	  /*for(int i = 0 ; i < botRh - topLh ; i++)
	    {
	      printf("histo = %d\n", *(histo3+i));
	      }*/
	  hori_lines(image_surface, histo3, image.allLines[i].letters[j]);
	  free(histo3);
	}
    }
}
