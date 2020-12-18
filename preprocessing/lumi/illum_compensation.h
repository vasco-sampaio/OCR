#ifndef ILLUM_COMPENSATION_H_
#define ILLUM_COMPENSATION_H_

#include <stdlib.h>
#include <SDL.h>


typedef struct{
	int min;
	int max;
	int* grid;
} extrm;

void toGrayscale(SDL_Surface *image_surface, int* grayscale, int w, int h);
int SOB_kernel(int* mat);
extrm sobelEdge(SDL_Surface* image, int* grayscale);
void binarize(SDL_Surface* image, int* grid, int min, int max);
void contrastBin(SDL_Surface *image, int lvlc, int w, int h);
void mergeSurfaces(SDL_Surface* ei, SDL_Surface* cei);
Uint8 EM_kernel(int* mat);
void erosionMask(SDL_Surface* image);

#endif

