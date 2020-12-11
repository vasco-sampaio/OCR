//pixel_functions.c file

#include <err.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
//#include "pixel_functions.h"
//#include "structures.h"
#include "../types.h"

/*
Regroups functions to manipulate the pixels of an image
to draw lines on an image and know if a pixel has a
specific color
*/

//------------------------------------------------------------------------------

//Functions from the SDL documentation
//They manipulate the pixels of an image
//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html

/*
Return the pixel value at (x, y)
NOTE: The surface must be locked before calling this!
 */
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}


/*
Set the pixel at (x, y) to the given value
NOTE: The surface must be locked before calling this!
 */
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


//------------------------------------------------------------------------------

//Functions that draws red lines

/*
Function that draws a horizontal red line from left to right
*/
void trace_hori_red_line(SDL_Surface *image_surface, int startH, int startW, int endH, int endW)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  //verifying if the coordinates are valid
  if(startH < height && endH == startH  && startW < width && endW < width)
    {
      //changing the color of the pixels of the line
      for(int i = startW ; i <= endW ; i++)
	{
	  Uint32 newPixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
	  put_pixel(image_surface, i, endH, newPixel);
	}
    }
}

/*
Function that draws a vertical red line from the top to bottom
*/
void trace_vert_red_line(SDL_Surface *image_surface, int startH, int startW, int endH, int endW)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  //verifying if the coordinates are valid
  if(startH < height && endH < height && startW < width && startW == endW)
    {
      //changing the color of the pixels of the line
      for(int i = startH ; i <= endH ; i++)
	{
	  Uint32 newPixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
	  put_pixel(image_surface, endW, i, newPixel);
	}
    }
}

//Functions that draws red lines if no black pixels in the row/column

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
  for(int i = 0 ; i < len ; i++)
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
  for(int i = 0 ; i < len ; i++)
    {
      if (*(hori_histo + i) == 0)
	trace_vert_red_line(image_surface, topLh, topLw + i, botRh, topLw + i);
    }
}

//------------------------------------------------------------------------------

//Functions that checks the color of a pixel

/*
Function that checks if whether or not, a pixel
that has the coordinates h and w, is black.
Returns an int : 
-1 if black
-0 if white
*/
int is_black(SDL_Surface *image_surface, int w, int h)
{
  Uint32 pixel = get_pixel(image_surface, w, h);
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format,&r, &g, &b);

  //checks if the pixel is black (only verifying red because
  //the image is binarized
  if (r == 0)
    return 1;
  return 0;
}

/*
Function that check whether or not a pixel is a
red pixel.
Returns an int :
-1 if red
-0 if not
*/
int is_red(SDL_Surface *image_surface, int w, int h)
{
  Uint32 pixel = get_pixel(image_surface, w, h);
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format,&r, &g, &b);

  //checks if the pixel is red
  //if it is, returns 1
  if (r == 255 && g == 0 && b == 0)
    return 1;
  return 0;
}
