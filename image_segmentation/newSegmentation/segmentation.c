#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "segmentation.h"
/*
This file is in charge of the segmentation
*/


//Function that check whether or not a pixel is a foreground pixel (= information)
int is_foreground(SDL_Surface *image_surface, Uint32 pixel)
{
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format,&r, &g, &b);

  //checks if the pixel is black
  //if it is, returns 0
  if (r == 0)
    return 0;
  return 1;
}


/*
Function that creates a vertical histogram that counts the foreground pixels
of each row, of a given rectangle
*/
void verti_histo(SDL_Surface *image_surface, int *histo, int topLw, int topLh, int botRw, int botRh)
{ 
  for(int i = topLh ; i < botRh ; i++)
    {
      for(int j = topLw ; j < botRw ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  if (is_foreground(image_surface, pixel) == 0)
	    *(histo + i) += 1;
	}
    }
}


/*
Function that creates a horizontal histogram that counts the foreground pixels
of each column, of a given rectangle 
 */
void hori_histo(SDL_Surface *image_surface, int *histo, int topLw, int topLh, int botRw, int botRh)
{ 
  for(int j = topLw ; j < botRw ; j++)
    {
      for(int i = topLh ; i < botRh ; i++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  if(is_foreground(image_surface, pixel) == 0)
	    *(histo + j) += 1;
	}
    }
}

/*
Function that draws horizontal lines in a given rectangle, in rows with no foreground pixels
 */
void hori_lines(SDL_Surface *image_surface, int *vertHisto, int topLw, int topLh, int botRw, int botRh)
{
  int len = botRh - topLh;
  for(int i = 0 ; i < len ; i++)
    {
      if (*(vertHisto + i) == 0)
	  trace_hori_red_line(image_surface, topLh + i, topLw, topLh + i, botRw);
    }
}

/*
Function that draws vertical lines in a given rectangle, in columns with no foreground pixels
*/
void vert_lines(SDL_Surface *image_surface, int *hori_histo,int topLw, int topLh, int botRw, int botRh )
{
  int len = botRw - topLw;
  for(int i = 0 ; i < len ; i++)
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
      res.zones[i].topLeft.w = 0;
      res.zones[i].topLeft.h = 0;
      res.zones[i].botRight.w = 0;
      res.zones[i].botRight.h = 0;
    }
  return res;
}

//Function that check whether or not a pixel is a foreground pixel (= information)
int is_red(SDL_Surface *image_surface, Uint32 pixel)
{
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
Function that goes through the image to count the zones not touched by the lines drawn
*/
int count_get_lines(SDL_Surface *image_surface)
{
 int height = image_surface->h;
 int width = image_surface->w;

 int res = 0;

 int i = 0;
 while(i < height)
   {
     int j = 0;
     while(j < width)
       {
	 Uint32 pixel = get_pixel(image_surface, j, i);
	 int red = is_red(image_surface, pixel);
	 if(red == 1) //first encounter with a pixel not red
	   {
	     int k = j;
	     while(red == 1 && k < width)
	       {
		 pixel = get_pixel(image_surface, k, i);
		 red = is_red(image_surface, pixel);
		 k++;
	       }
	     k = i;
	     red = 1;
	     while(red == 1 && k < height)
	       {
		 pixel = get_pixel(image_surface, j, k);
		 red = is_red(image_surface, pixel);
		 k++;
	       }
	     i = k-1;
	     j = width;
	     res += 1;
	   }
	 j++;
       }
     i++;
   }
 return res;
}

/*
Function that goes through the image to get the zones not touched by the lines drawn
Gets 2 coordinates to define a rectangle
*/
void get_lines(SDL_Surface *image_surface, lineZones all)
{
  int height = image_surface->h;
  int width = image_surface->w;

  //int inZone = 1; //if in zone = 0
  int zone_i = 0;

  int i = 0;
  while(i < height)
    {
      int j = 0;
      while(j < width)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  int red = is_red(image_surface, pixel);
	  if(/*inZone  == 1 &&*/ red == 1) //first encounter with a pixel not red
	    {
	      //inZone == 0;
	      all.zones[zone_i].topLeft.w = j;
	      all.zones[zone_i].topLeft.h = i;
	      int k = j;
	      while(red == 1 && k < width)
		{
		  pixel = get_pixel(image_surface, k, i);
		  red = is_red(image_surface, pixel);
		  k++;
		}
	      all.zones[zone_i].botRight.w = k-1;
	      k = i;
	      red = 1;
	      while(red == 1 && k < height)
		{
		  pixel = get_pixel(image_surface, j, k);
		  red = is_red(image_surface, pixel);
		  k++;
		}
	      all.zones[zone_i].botRight.h = k-1;
	      zone_i += 1;
	      j = width;
	      i = k-1;
	    }
	  j++;
	}
      i++;
    }
}

/*
Function that goes through a given rectangle to count the zones were there are letters
*/
/*int count_get_letters(SDL_Surface *image_surface, coord rect)
{
  return 1;
  }*/


/*
Function that goes through a given rectangle to get the zones not touched by the red lines and where there are the letters.
*/
/*void get_letters(SDL_Surface *image_surface, coord rect, line l)
{
 
}*/

/*
Function that uses the others :
1)Makes a vertical histogram
2)Draws horizontal lines
3)Gets the nb of lines and the coordinates of the corners
4)makes horizontal histo for each line
5)draws vertical lines for each line of the pic
6)Gets the number of letters per line and the coordinates of the corners
7)makes vertical histo for each letter
8)draws horizontal lines for each letter
9)gets all the letter in each line
10)translates each letter in a binary matrix
*/
