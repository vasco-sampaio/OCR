//utility.h file
#ifndef UTILITY_H_
#define UTILITY_H_

int avLenLetter(line l);
double avHeiLetter(line l);
void sepLetters(SDL_Surface *image, line l);

int avSpaceLetter(line *l);
void detectSpace(line *l);
void detectSpaceDoc(doc *image);

int r_sum(SDL_Surface *image_surface, int wMin, int wMax, int h);
int bot_sum(SDL_Surface *image_surface, int hMin, int hMax, int w);

int count_get_lines(SDL_Surface *image_surface, coord rect);
void get_lines(SDL_Surface *image_surface, coord rect, lineZones all);

int count_get_letters(SDL_Surface *image_surface, coord rect);
void get_letters(SDL_Surface *image_surface, coord rect, line l);

#endif 
