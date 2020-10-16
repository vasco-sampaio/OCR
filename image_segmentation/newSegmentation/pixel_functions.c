//functions to manipulate pixels of an image
//is from the SDL documentation
//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideo.html

#include <err.h>
#include "pixel_functions.h"

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
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
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
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

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}


//function that traces a horizontal red line from left to right
void trace_hori_red_line(SDL_Surface *image_surface, int startH, int startW, int endH, int endW)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  if(startH < height && endH == startH  && startW < width && endW < width)
    {
      /*if(startW > endW)
	{
	  swap(startW, endW);
	  }*/
      for(int i = startW ; i <= endW ; i++)
	{
	  Uint32 newPixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
	  put_pixel(image_surface, i, endH, newPixel);
	}
    }
}

//function that traces a vertical red line from the top to bottom
void trace_vert_red_line(SDL_Surface *image_surface, int startH, int startW, int endH, int endW)
{
  //taking the dimensions of the image
  int width = image_surface->w;
  int height = image_surface->h;

  if(startH < height && endH < height && startW < width && startW == endW)
    {
      for(int i = startH ; i <= endH ; i++)
	{
	  Uint32 newPixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
	  put_pixel(image_surface, endW, i, newPixel);
	}
    }
}
