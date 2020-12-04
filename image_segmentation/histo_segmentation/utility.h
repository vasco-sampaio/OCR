//utility.h file
#ifndef UTILITY_H_
#define UTILITY_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
#include "pixel_functions.h"
#include "structures.h"

void verti_histo(SDL_Surface *image_surface, int *histo, coord rect);
void hori_histo(SDL_Surface *image_surface, int *histo, coord rect);

int avLenLetter(line l);
double avHeiLetter(line l);
void sepLetters(SDL_Surface *image, line l);

int avSpaceLetter(line *l);
void detectSpace(line *l);
void detectSpaceDoc(doc *image);

int r_sum_line(SDL_Surface *image_surface, int wMin, int wMax, int h);
int bot_sum_line(SDL_Surface *image_surface, int hMin, int hMax, int w);

int r_sum_letter(SDL_Surface *image_surface, int wMin, int wMax, int h);
int bot_sum_letter(SDL_Surface *image_surface, int hMin, int hMax, int w);

int count_get_lines(SDL_Surface *image_surface, coord rect);
void get_lines(SDL_Surface *image_surface, coord rect, lineZones all);

int count_get_letters(SDL_Surface *image_surface, coord rect);
void get_letters(SDL_Surface *image_surface, coord rect, line l);

double rect_ratio(coord r);
int count_groups(int *histo, int lenH);
tuples generate_groups(int *histo, int lenH);
int max_tuples(tuples t);
void hori_lines_define(SDL_Surface *image, int *vertHisto, coord rect, tuples t);

#endif 
