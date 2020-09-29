#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "grayscale.h"
#include "pixel_functions.h"

int toGrayscale(SDL_Surface *image_surface)
{
  //applying grayscale
  
  //code for each pixel of the image
  int width = image_surface->w;
  int height = image_surface->h;

  for(int i = 0 ; i < height ; i++)
    {
      for(int j = 0 ; j < width ; j++)
	{
	  Uint32 pixel = get_pixel(image_surface, j, i); //getting the pixel value
	  Uint8 r, g, b;
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b); //getting the RGB values
	  double average = 0.3*r + 0.59*g + 0.11*b;
	  r = average;
	  g = average;
	  b = average; //updating rgb values
	  Uint32 newPixel = SDL_MapRGB(image_surface->format, r, g, b); //getting the new pixel value
	  put_pixel(image_surface, j, i, newPixel); //putting new pixel value on the surface
	    
	}
    }

  //saving the image
  SDL_SaveBMP(image_surface, "test.bmp");
  
  //freeing the image surface
  SDL_FreeSurface(image_surface);

  return 0;
}
