//segmentation.h file

#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "segmentation.h"
#include "structures.h"
#include "utility.h"

#include "../neunet_HD/neunet.h"

void marking_lines(SDL_Surface *image_surface, int height, int width);
lineZones marking_letters(SDL_Surface *image_surface, int w, int h);
doc keep_letters(SDL_Surface *image_surface, lineZones all);
void resize(SDL_Surface *image, coord letter);
void resize_letter(SDL_Surface *image_surface, doc image);

void segmentation(char *path, neunet_t *nn);
char* segmentation_SDL(SDL_Surface * image_surface, neunet_t *nn);

char* ocr(char * image_path, char* neunet_path);
double* dataset(char *path);

#endif
