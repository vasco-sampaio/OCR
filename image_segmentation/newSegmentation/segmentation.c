#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
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

//Function that changes the color of a pixel to blue
void to_blue(SDL_Surface *image_surface, int w, int h)
{
  Uint8 r, g, b;
  Uint32 newPixel = SDL_MapRGB(image_surface->format, 0, 0, 255);
  put_pixel(image_surface, w, h, newPixel);
}

/*
Function that creates a horizontal histogram that counts the foreground pixels
of each row, of a given rectangle
*/


/*
Function that creates a horizontal histogram that counts the foreground pixels
of each column, of a given rectangle 
 */


/*
Function that draws horizontal lines in a given rectangle, in rows with no foreground pixels
 */


/*
Function that draws vertical lines in a given rectangle, in columns with no foreground pixels
*/

/*
Function that goes through the image to get the zones not touched by the lines drawn
Gets 4 coordinates to define a rectangle
*/

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
