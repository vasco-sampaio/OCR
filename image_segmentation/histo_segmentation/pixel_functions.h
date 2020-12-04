//pixel_functions.h file

#ifndef PIXEL_FUNCTIONS_H_
#define PIXEL_FUNCTIONS_H_

Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void trace_hori_red_line(SDL_Surface *surface, int startH, int startW, int endH, int endW);
void trace_vert_red_line(SDL_Surface *surface, int startH, int startW, int endH, int endW);

void hori_lines(SDL_Surface *image_surface, int *vertHisto, coord rect);
void vert_lines(SDL_Surface *image_surface, int *hori_histo, coord rect);

int is_black(SDL_Surface *image_surface, int w, int h);
int is_red(SDL_Surface *image_surface, int w, int h);

#endif
