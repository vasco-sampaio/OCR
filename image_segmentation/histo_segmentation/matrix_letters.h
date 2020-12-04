//matrix_letters.h file

#ifndef MATRIX_LETTERS_H_
#define MATRIX_LETTERS_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "segmentation.h"
#include "matrix_letters.h"
#include "structures.h"

matrix buildMatrix(SDL_Surface *image, coord let);

lineMat buildLineMat(SDL_Surface *image, line l);

docMat buildDocMat(SDL_Surface *image, doc i);

#endif
