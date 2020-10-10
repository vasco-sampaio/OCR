#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"

/* 
This file is in charge of the line segmentation
*/


//function that checks if a pixel is black in a binarized image
int isBlack(SDL_Surface *image_surface, Uint32 pixel)
{
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
  
  //checks if the pixel is black
  //if it is, returns 0
  if (r == 0) 
    return 0;
  return 1;
}


/*
Function that builds the horizontal histogram of a binarized image.

The array is of the size of the number of columns in
the image (width).

This histogram will count the nb of black pixels in each column.
*/
void horiHistogram(SDL_Surface *image_surface, long *histo)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  if (isBlack(image_surface, pixel) == 0)
	    *(histo + j) +=1;
	}
    }  
}


/*
Function that builds the vertical histogram of a binarized image.

The array is of the size of the number of rows in
the image (height).

This histogram will count the nb of black pixels in each row.
*/
void vertHistogram(SDL_Surface *image_surface, long *histo)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i);
	  if (isBlack(image_surface, pixel) == 0)
	    *(histo + i) +=1;
	}
    }  
}


//function that smoothes the projection profile histogram
void moving_average(int len, long *histo, int window)
{
  int sum;
  int average;
  
  for(int i = 0 ; i < len ; i++)
    {
      //reseting the sum value
      sum = 0;

      if(i + window < len)
	{
	  //adding the values of the histo
	  for(int j = i ; j < i + window ; j++)
	    {
	      sum += *(histo + j);
	    }
	  average = sum / window;
	  
	  //changing the values of the histo
	  for(int j = i ; j < i + window ; j++)
	    {
	      *(histo + j) = average;
	    }
	}
    }
}


/*
Function that counts the number of valleys in the histogram.
Each valley correspond to the place of a potential line.
*/
size_t line_seg_count(long *histo, int lenH)
{ 
  int isGoingUp = 1; //false, 0 is true
  
  long before = *histo;
  size_t nbLines = 0;

  for(int i = 1 ; i < lenH ; i++)
    {
      if (before < *(histo+i) && isGoingUp == 1)
	{
	  isGoingUp = 0;
	  nbLines++;
	}
      else if (before >= *(histo+i) && isGoingUp == 0)
	{
	  isGoingUp = 1;
	}
      
      before = *(histo+i); 
      }
  return nbLines;
}


/*
Function that gives the height of the pixels that can be used
to separate lines of the text.
*/
void line_seg(long *histo, int lenH, int *lines)
{ 
  int isGoingUp = 1; //false, 0 is true
  
  long before = *histo;
  int rankLines = 0;

  for(int i = 1 ; i < lenH ; i++)
    {
      if (before < *(histo+i) && isGoingUp == 1)
	{
	  isGoingUp = 0;
	  *(lines + rankLines) = i;
	  rankLines++;
	}
      else if (before >= *(histo+i) && isGoingUp == 0)
	isGoingUp = 1;
      
      before = *(histo+i); 
      }
}


/*
Function that draws red lines to show
the line segmentation
*/
void line_seg_drawing(SDL_Surface *image_surface, int *lines, size_t lenL)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  
  for(size_t i = 0 ; i < lenL ; i++)
    {
      trace_hori_red_line(image_surface, *(lines + i), 0, *(lines + i), width-1); 
    }

  SDL_SaveBMP(image_surface, "line_seg.bmp");
}
